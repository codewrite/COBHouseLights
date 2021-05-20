/*
 * Research links:
 * https://www.postman.com/
 * https://insomnia.rest/
 * 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

#include "passwords.h"
#include "swagger.h"


void handleNotFound()
{
  server.send(404);
}

void SendNotAuthorized()
{
  server.send(200, "application/json", "{ \"reason\": \"Not authorized\" }");
}

void SendOK()
{
  server.send(200, "application/json");
}

void SendOptionsResponseForCORS(char* methods)
{
    server.sendHeader(String(F("Access-Control-Allow-Methods")), String(methods) + F(",OPTIONS"));
    server.sendHeader(String(F("Access-Control-Allow-Headers")), String(F("Content-Type, Access-Control-Allow-Headers, Authorization")));
    server.send(200);
}

void SetupApi()
{
  server.enableCORS(true);

  server.on("/", HTTP_GET, []()
  {
    server.send(200, "text/html", swaggerUI);
  });
    
  server.on("/swagger.yaml", HTTP_GET, []()
  {
    server.send(200, "application/x-yaml", swaggerYAML);
  });

  server.on("/forcelogin", HTTP_GET, []()   // This is so we can logout. Otherwise the browser tends to remember the login credentials.
  {
    forceLogin();
  });

  server.on("/leds", HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET"); });
  server.on("/leds", HTTP_GET, []()
  {
    checkAuthenticated([]()
    {
      String brightnessArrayStr = "[ ";
      for (int i=0; i<NUM_LED_PINS; i++)
      {
        brightnessArrayStr += pinPWMValue[i];
        if (i < NUM_LED_PINS-1)
        {
          brightnessArrayStr += ", ";
        }
      }
      brightnessArrayStr += " ]";
      server.send(200, "application/json", brightnessArrayStr);
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.on(UriBraces("/leds/{}"), HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET"); });
  server.on(UriBraces("/leds/{}"), HTTP_GET, []()
  {
    checkAuthenticated([]()
    {
      int led = server.pathArg(0).toInt();
      char brightnessStr[6];
      sprintf(brightnessStr, "%d", pinPWMValue[led]);
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      server.send(200, "application/json", "{ \"brightness\": ");
      server.sendContent(brightnessStr);
      server.sendContent(" }");
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.on(UriBraces("/leds/{}/{}"), HTTP_OPTIONS, []() { SendOptionsResponseForCORS("PUT"); });
  server.on(UriBraces("/leds/{}/{}"), HTTP_PUT, []()
  {
    checkAuthenticated([]()
    {
      int led = server.pathArg(0).toInt();
      int ledBrightness = server.pathArg(1).toInt();
      int pin = LED_BUILTIN;
      for (int i=0; i<NUM_LED_PINS; i++)
      {
        if (pinMap[i][0] == led)
        {
          pin = pinMap[i][1];
          break;
        }
      }
      Serial.print("LED");  Serial.print(led);  Serial.print(", pin");  Serial.print(pin);
      Serial.print(" brightness: ");  Serial.println(ledBrightness);
      analogWrite(pin, ledBrightness);
      pinPWMValue[led] = ledBrightness;
      SendOK();
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.on("/pwr", HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET"); });
  server.on("/pwr", HTTP_GET, []()
  {
    checkAuthenticated([]()
    {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      server.send(200, "application/json", "{ \"power\": ");
      server.sendContent((digitalRead(D0) != 0) ? "true" : "false");
      server.sendContent(" }");
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.on(UriBraces("/pwr/{}"), HTTP_OPTIONS, []() { SendOptionsResponseForCORS("PUT"); });
  server.on(UriBraces("/pwr/{}"), HTTP_PUT, []()
  {
    checkAuthenticated([]()
    {
      int pwrOn = server.pathArg(0).toInt();
      digitalWrite(D0, pwrOn);
      SendOK();
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.onNotFound(handleNotFound);
}
