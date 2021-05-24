/*
  Based on HelloServerSecure from https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples

  Lights will flash on startup. For info about GPIO pins:
  https://rabbithole.wwwdotorg.org/2017/03/28/esp8266-gpio.html
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#include "passwords.h"

// USE_HTTPS => 0 for HTTP, 1 for HTTPS
#define USE_HTTPS 1

const char* ssid = STASSID;
const char* password = STAPSK;

IPAddress local_IP(STA_IPAddress);
IPAddress gateway(STA_gateway);
IPAddress subnet(STA_subnet);

const char* www_username = STA_WWW_USER;
const char* www_password = STA_WWW_PASSWORD;

#if USE_HTTPS == 1
  ESP8266WebServerSecure server(443);
#else
  ESP8266WebServer server(80);
#endif
Adafruit_INA219 ina219;

// D7 and D8 seem to mapped incorrectly for the Wemos D1 mini pro board - so remap them to the correct GPIOs
#undef D7
#define D7 (13)
#undef D8
#define D8 (15)

// See: https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int pinMap[][2] = { {1,D3}, {2,D4}, {3,D5}, {4,D6}, {5,D7}, {6,D8} };
const int NUM_LED_PINS = sizeof(pinMap)/sizeof(pinMap[0]);
bool pinOnValue[NUM_LED_PINS];
int pinPWMValue[NUM_LED_PINS];
int numAuthenticationFails = 0;
int authenticateDelay = 0;
unsigned long lastInaReportMillis = 0;
unsigned long lastAuthFailMillis = 0;

bool checkAuthenticated(void(*successFn)(), void(*failureFn)())
{
  if (authenticateDelay > 0)
  {
    (*failureFn)();
  }
  if (server.authenticate(www_username, www_password))
  {
    numAuthenticationFails = 0;
    (*successFn)();
  }
  else
  {
    numAuthenticationFails++;
    Serial.print("Login fails: ");
    Serial.println(numAuthenticationFails);
    if (numAuthenticationFails > 5)
    {
      // Slow potential brute force attacks
      Serial.println("wait 5s start");
      authenticateDelay = 5000;
    }
    server.requestAuthentication();
    (*failureFn)();
  }
}

void forceLogin()
{
    server.requestAuthentication();
}

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D0, OUTPUT);
  for (int i=0; i<NUM_LED_PINS; i++)
  {
    pinMode(pinMap[i][1], OUTPUT);
    pinOnValue[i] = false;
    pinPWMValue[i] = 0;
  }
  Serial.begin(115200);
  Serial.println("");
  if (! ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");  Serial.println(ssid);
  Serial.print("IP address: ");  Serial.println(WiFi.localIP());

  if (MDNS.begin(STACNAME))
  {
    Serial.println("MDNS responder started");
  }

#if USE_HTTPS == 1
  server.getServer().setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
#endif

  SetupApi();
  server.begin();

#if USE_HTTPS == 1
  Serial.println("HTTPS server started");
#else
  Serial.println("HTTP server started");
#endif
}

int mCount = 0;
float maxCurrent = 0;

void loop(void)
{
  server.handleClient();
  MDNS.update();

  unsigned long newMillis = millis();
  unsigned long inaMilliDiff = (newMillis >= lastInaReportMillis) ? newMillis - lastInaReportMillis : lastInaReportMillis - newMillis;
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA() * 1.70f;  // Modified INA219 board that can measure up to 5.4A
  mCount++;
  if (current_mA > maxCurrent) maxCurrent = current_mA;
  
  if (inaMilliDiff > 5000UL)
  {
    Serial.print("Samples: "); Serial.print(mCount); Serial.print(", max current: "); Serial.print(maxCurrent); Serial.print(", ");
    Serial.print("Bus Voltage: "); Serial.print(busvoltage); Serial.print(" V, ");
    Serial.print("Current: "); Serial.print(current_mA); Serial.println(" mA");
    lastInaReportMillis = newMillis;
    mCount = 0;
    maxCurrent = 0.0f;
  }
  if (authenticateDelay > 0)
  {
    unsigned long authFailMilliDiff = (newMillis >= lastAuthFailMillis) ? newMillis - lastAuthFailMillis : lastAuthFailMillis - newMillis;
    if (authFailMilliDiff > authenticateDelay)
    {
      authenticateDelay = 0;
      Serial.println("wait 5s end");
      authenticateDelay = 0;
      numAuthenticationFails = 0;
    }
  }
  else
  {
    lastAuthFailMillis = newMillis;
  }
}
