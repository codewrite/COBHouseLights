/*
 * Research links:
 * https://www.postman.com/
 * https://insomnia.rest/
 * 
 */

#include <ArduinoJson.h>
#if USE_ESP32 == 1
  #include "analogWrite.h"      // See: https://github.com/Dlloydev/ESP32-ESP32S2-AnalogWrite
#else
  #include <uri/UriBraces.h>
  #include <uri/UriRegex.h>
#endif

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
      String brightnessArrayStr = "[";
      for (int i=0; i<NUM_LED_PINS; i++)
      {
        brightnessArrayStr += "{\"on\":";
        brightnessArrayStr += pinOnValue[i] ? "true" : "false";
        brightnessArrayStr += ",\"brightness\":";
        brightnessArrayStr += pinPWMValue[i];
        brightnessArrayStr += "}";
        if (i < NUM_LED_PINS-1)
        {
          brightnessArrayStr += ",";
        }
      }
      brightnessArrayStr += "]";
      server.send(200, "application/json", brightnessArrayStr);
    },[]()
    {
      SendNotAuthorized();
    });
  });

#if USE_ESP32 == 1
  server.on("/leds/*", HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET,PUT"); });
  server.on("/leds/*", HTTP_GET, []()
#else
  server.on(UriBraces("/leds/{}"), HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET,PUT"); });
  server.on(UriBraces("/leds/{}"), HTTP_GET, []()
#endif
  {
    checkAuthenticated([]()
    {
      int ledIndex = server.pathArg(0).toInt() - 1;
      if (ledIndex >=0 && ledIndex < NUM_LED_PINS)
      {
        String brightnessStr = "{\"on\":";
        brightnessStr += pinOnValue[ledIndex] ? "true" : "false";
        brightnessStr += ",\"brightness\":";
        brightnessStr += pinPWMValue[ledIndex];
        brightnessStr += "}";
        //server.setContentLength(CONTENT_LENGTH_UNKNOWN);
        server.send(200, "application/json", brightnessStr);
        //server.sendContent(brightnessStr);  //this is slow! - so construct string first instead
      }
    },[]()
    {
      SendNotAuthorized();
    });
  });

  // For HTTP_OPTIONS see GET method
#if USE_ESP32 == 1
  server.on("/leds/*", HTTP_PUT, []()
#else
  server.on(UriBraces("/leds/{}"), HTTP_PUT, []()
#endif
  {
    checkAuthenticated([]()
    {
      int ledIndex = server.pathArg(0).toInt() - 1;
      if (ledIndex >=0 && ledIndex < NUM_LED_PINS)
      {
        DynamicJsonDocument doc(200);
        deserializeJson(doc, server.arg("plain"));
        int ledOn = doc.containsKey("on") ? doc["on"] : pinOnValue[ledIndex];
        int ledBrightness = doc.containsKey("brightness") ? doc["brightness"] : pinPWMValue[ledIndex];
        int pin = LED_BUILTIN;
        for (int i=0; i<NUM_LED_PINS; i++)
        {
          if (pinMap[i][0] == ledIndex + 1)
          {
            pin = pinMap[i][1];
            break;
          }
        }
        analogWrite(pin, ledOn ? ledBrightness : 0);
        pinOnValue[ledIndex] = ledOn;
        pinPWMValue[ledIndex] = ledBrightness;
      }
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
      server.sendContent((digitalRead(PWR_PIN) != 0) ? "true" : "false");
      server.sendContent(" }");
    },[]()
    {
      SendNotAuthorized();
    });
  });

#if USE_ESP32 == 1
  server.on("/pwr/*", HTTP_OPTIONS, []() { SendOptionsResponseForCORS("PUT"); });
  server.on("/pwr/*", HTTP_PUT, []()
#else
  server.on(UriBraces("/pwr/{}"), HTTP_OPTIONS, []() { SendOptionsResponseForCORS("PUT"); });
  server.on(UriBraces("/pwr/{}"), HTTP_PUT, []()
#endif
  {
    checkAuthenticated([]()
    {
      int pwrOn = server.pathArg(0).toInt();
      digitalWrite(PWR_PIN, pwrOn);
      SendOK();
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.on("/status", HTTP_OPTIONS, []() { SendOptionsResponseForCORS("GET"); });
  server.on("/status", HTTP_GET, []()
  {
    checkAuthenticated([]()
    {
        String statusStr = "{\"samples\":";
        statusStr += mCount;
        statusStr += ",\"millis\":";
        statusStr += inaMilliDiff;
        statusStr += ",\"busvoltage\":";
        statusStr += busvoltage;
        statusStr += ",\"current\":";
        statusStr += current_mA;
        statusStr += ",\"maxcurrent\":";
        statusStr += maxCurrent;
        statusStr += ",\"lastMessage\":\"";
        statusStr += lastMessage;
        statusStr += "\"}";
        currentRead = true;
        server.send(200, "application/json", statusStr);
    },[]()
    {
      SendNotAuthorized();
    });
  });

  server.onNotFound(handleNotFound);
}
