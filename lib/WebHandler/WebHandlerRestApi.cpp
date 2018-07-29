#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiHandler.hpp>
#include <LocalTimeHandler.hpp>
#include <SHT3xHandler.hpp>

#include "WebHandler.hpp"
#include "WebHandlerPrivate.hpp"

#include <time.h>

static String jsonStatus( String addon )
{
  String message = "{";
  message += addon;

  message += "\"appName\":";
  message += "\"" APP_NAME "\"";

  message += ",\"appVersion\":";
  message += "\"" APP_VERSION "\"";

  message += ",\"appAuthor\":";
  message += "\"" APP_AUTHOR "\"";

  message += ",\"buildDate\":";
  message += "\"" __DATE__ "\"";
  
  message += ",\"buildTime\":";
  message += "\"" __TIME__ "\"";

  message += ",\"currentTimestamp\":";
  message += app.secTimestamp();
  message += ",\"wifiConnectCounter\":";
  message += wifiHandler.getConnectCounter();

  message += ",\"hostname\":";
  message += "\"";
  message += WiFi.hostname();
  message += "\"";

  message += ",\"ipAddress\":";
  message += "\"";
  message += WiFi.localIP().toString();
  message += "\"";

  message += ",\"netmask\":";
  message += "\"";
  message += WiFi.subnetMask().toString();
  message += "\"";

  message += ",\"gateway\":";
  message += "\"";
  message += WiFi.gatewayIP().toString();
  message += "\"";

  message += ",\"wifiConnectTimestamp\":";
  message += wifiHandler.getConnectTimestamp();

  message += "}\r\n";
  return message;
}

static String jsonTime()
{
  time_t now = 0;
  time(&now);

  String message = "{";

  message += "\"time\":";
  message += now;

  message += ",\"localTime\":";
  message += "\"";
  message += localTimeHandler.getLocalTimeText();
  message += "\"";

  message += "}\r\n";
  return message;
}

static String jsonSensor()
{
  String message = "{";

  message += "\"temperature\":";
  message += sht3xHandler.cTemp;

  message += ",\"humidity\":";
  message += sht3xHandler.humidity;

  message += ",\"avgValid\":";
  message += sht3xHandler.avgValuesAvailable ? "true" : "false";

  message += ",\"avgTemperature\":";
  message += sht3xHandler.avgCTemp;

  message += ",\"avgHumidity\":";
  message += sht3xHandler.avgHumidity;

  message += ",\"avgCounter\":";
  message += sht3xHandler.avgCounter;

  message += "}\r\n";
  return message;
}

void setupRestApi( AsyncWebServer& server )
{
  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonStatus("") );
    response->addHeader( "Access-Control-Allow-Origin", "*" );
    response->addHeader("Cache-Control", "no-cache");
    request->send(response);
  });
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonTime() );
    response->addHeader( "Access-Control-Allow-Origin", "*" );
    response->addHeader("Cache-Control", "no-cache");
    request->send(response);
  });
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonSensor() );
    response->addHeader( "Access-Control-Allow-Origin", "*" );
    response->addHeader("Cache-Control", "no-cache");
    request->send(response);
  });
}
