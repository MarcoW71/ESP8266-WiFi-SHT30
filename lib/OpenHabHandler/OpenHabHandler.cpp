#include <Arduino.h>
#include <App.hpp>
#include <WifiHandler.hpp>
#include <ESP8266HTTPClient.h>
#include <SHT3xHandler.hpp>
#include "OpenHabHandler.hpp"

OpenHabHandler openHabHandler;

OpenHabHandler::OpenHabHandler()
{
  intervalTimestamp = 0;
}

void OpenHabHandler::sendValueV1( const char* itemName, float value )
{
  HTTPClient http;
  char urlBuffer[256];

  sprintf( urlBuffer, "http://%s:%d/CMD?%s=%.02f", appcfg.ohab_host,
    appcfg.ohab_port, itemName, value );

  LOG1("URL=%s\n",urlBuffer);

  http.begin( urlBuffer );
  if ( appcfg.ohab_useauth )
  {
    http.setAuthorization(appcfg.ohab_user, appcfg.ohab_password);
  }

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    LOG1("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK)
    {
      http.getString();
    }
  }
  else
  {
    LOG1("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void OpenHabHandler::sendValueV2( const char* itemName, float value )
{
  HTTPClient http;
  char urlBuffer[256];
  char valueBuffer[32];

  sprintf( urlBuffer, "http://%s:%d/rest/items/%s", appcfg.ohab_host,
    appcfg.ohab_port, itemName );

  LOG1("URL=%s\n",urlBuffer);

  http.begin( urlBuffer );
  http.addHeader("Cache-Control", "no-cache");
  http.addHeader("Accept", "application/json");
  http.addHeader("Content-Type", "text/plain");

  if ( appcfg.ohab_useauth )
  {
    http.setAuthorization(appcfg.ohab_user, appcfg.ohab_password);
  }

  sprintf( valueBuffer, "%.02f", value );

  int httpCode = http.POST(valueBuffer);

  if (httpCode > 0)
  {
    LOG1("[HTTP] POST... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK)
    {
      http.getString();
    }
  }
  else
  {
    LOG1("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void OpenHabHandler::sendValue( const char* itemName, float value )
{
  if( appcfg.ohab_enabled && wifiHandler.isReady() )
  {
    if( appcfg.ohab_version == 1 )
    {
      sendValueV1( itemName, value );
    }
    else
    {
      sendValueV2( itemName, value );
    }
  }
}

void OpenHabHandler::handle(time_t timestamp)
{
  if ( sht3xHandler.avgValuesAvailable && ( timestamp - intervalTimestamp ) >= 120000 )
  {
    intervalTimestamp = timestamp;
    sendValue( appcfg.ohab_itemname_temp, sht3xHandler.avgCTemp );
    sendValue( appcfg.ohab_itemname_humi, sht3xHandler.avgHumidity );
  }
}
