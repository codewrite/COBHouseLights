/*
  Based on HelloServerSecure from https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/examples

  Lights will flash on startup. For info about GPIO pins:
  https://rabbithole.wwwdotorg.org/2017/03/28/esp8266-gpio.html
*/

// USE_ESP32 => 0 for ESP8266, 1 for ESP32
#define USE_ESP32 0
// USE_HTTPS => 0 for HTTP, 1 for HTTPS
#define USE_HTTPS 1

#if USE_ESP32 == 1
  #include <WiFi.h>
  #if USE_HTTPS == 1
    #include <ESPWebServerSecure.hpp>
  #else
    #include <ESPWebServer.hpp>
  #endif
  #include <ESPmDNS.h>
#else
  #include <ESP8266WiFi.h>
  #if USE_HTTPS == 1
    #include <ESP8266WebServerSecure.h>
  #else
    #include <ESP8266WebServer.h>
  #endif
  #include <ESP8266mDNS.h>
#endif

#include <Ticker.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#include "passwords.h"

const char* ssid = STASSID;
const char* password = STAPSK;

IPAddress local_IP(STA_IPAddress);
IPAddress gateway(STA_gateway);
IPAddress subnet(STA_subnet);

const char* www_username = STA_WWW_USER;
const char* www_password = STA_WWW_PASSWORD;

#if USE_ESP32 == 1
  #if USE_HTTPS == 1
    ESPWebServerSecure server(443);
  #else
    ESPWebServer server(80);
  #endif
#else
  #if USE_HTTPS == 1
    ESP8266WebServerSecure server(443);
  #else
    ESP8266WebServer server(80);
  #endif
#endif

#if USE_ESP32 != 1
  // D7 and D8 seem to mapped incorrectly for the Wemos D1 mini pro board - so remap them to the correct GPIOs
  #undef D7
  #define D7 (13)
  #undef D8
  #define D8 (15)
#endif

Adafruit_INA219 ina219;

#if USE_ESP32 == 1
  #define PWR_PIN 12
  const int pinMap[][2] = { {1,13}, {2,14}, {3,15}, {4,16}, {5,17}, {6,18} };
#else
  #define PWR_PIN 0
  const int pinMap[][2] = { {1,D3}, {2,D4}, {3,D5}, {4,D6}, {5,D7}, {6,D8} };
#endif
const int NUM_LED_PINS = sizeof(pinMap)/sizeof(pinMap[0]);
bool pinOnValue[NUM_LED_PINS];
int pinPWMValue[NUM_LED_PINS];
int numAuthenticationFails = 0;
int authenticateDelay = 0;
bool inaInitialized = false;
unsigned long lastInaReportMillis = 0;
unsigned long lastAuthFailMillis = 0;

bool checkAuthenticated(void(*successFn)(), void(*failureFn)())
{
  if (authenticateDelay > 0)
  {
    (*failureFn)();
  }
  else if (server.authenticate(www_username, www_password))
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
      (*failureFn)();
    }
    else
    {
      Serial.println("auth fail");
      server.requestAuthentication();
    }
  }
}

void forceLogin()
{
    server.requestAuthentication();
}

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWR_PIN, OUTPUT);
  for (int i=0; i<NUM_LED_PINS; i++)
  {
    pinMode(pinMap[i][1], OUTPUT);
    pinOnValue[i] = false;
    pinPWMValue[i] = 0;
  }
  Serial.begin(115200);
  Serial.println("");
  inaInitialized = ina219.begin();
  if (!inaInitialized)
  {
    Serial.println("Failed to find INA219 chip");
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
#if USE_ESP32 != 1
  MDNS.update();
#endif
  unsigned long newMillis = millis();
  unsigned long inaMilliDiff = (newMillis >= lastInaReportMillis) ? newMillis - lastInaReportMillis : lastInaReportMillis - newMillis;
  float busvoltage = inaInitialized ? ina219.getBusVoltage_V() : 0.0f;
  float current_mA = inaInitialized ? ina219.getCurrent_mA() * 1.70f : 0.0f;  // Modified INA219 board that can measure up to 5.4A
  mCount++;
  if (current_mA > maxCurrent) maxCurrent = current_mA;
  
  if (inaMilliDiff > 5000UL && inaInitialized)
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
