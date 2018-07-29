#include <Arduino.h>
#include <time.h>
#include <App.hpp>
#include <SSD1306Brzo.h>
#include <TimeLib.h>
#include <LocalTimeHandler.hpp>
#include <SHT3xHandler.hpp>
#include <WiFiHandler.hpp>
#include <MqttHandler.hpp>
#include "DisplayHandler.hpp"

DisplayHandler displayHandler;

static SSD1306Brzo display( 0x3c, D2, D1, GEOMETRY_64_48 ); // WEMOS OLED Shield
static char buffer[256];
static time_t localTime;

DisplayHandler::DisplayHandler()
{
  intervalTimestamp = 0l;
  counter = 0;
  state = 0;
}

void DisplayHandler::handle( time_t timestamp, time_t maxLoopTime )
{
  if ( state == 0 && ( timestamp - intervalTimestamp ) >= DISPLAY_UPDATE_INTERVAL )
  {
    state = 1;
  }

  switch ( state ) // an ultra primitive scheduler ;-)
                   // but it reduces the max loop time from 7-8ms to 1-2ms
                   // and this leads to a rock solid WiFi connection ;-)
  {
  case 0:
    break;

  case 1:
    localTime = localTimeHandler.getLocalTime();
    display.setFont(ArialMT_Plain_10);
    display.clear();
    state++;
    break;

  case 2:
    display.drawStringf( 0, 0, buffer, "%04d-%02d-%02d", year(localTime), month(localTime), day(localTime));
    state++;
    break;

  case 3:
    display.drawStringf( 0, 9, buffer, "%02d:%02d:%02d", hour(localTime), minute(localTime), second(localTime));
    state++;
    break;

  case 4:
    display.drawStringf( 0, 18, buffer, "%0.1f°C %0.1f%%", sht3xHandler.cTemp, sht3xHandler.humidity );
    state++;
    break;

  case 5:
    if ( sht3xHandler.avgValuesAvailable )
    {
      display.drawStringf( 0, 27, buffer, "%0.1f°C %0.1f%%", sht3xHandler.avgCTemp, sht3xHandler.avgHumidity );
    }
    state++;
    break;

  case 6:
    display.drawStringf( 0, 36, buffer, "W%ld M%ld L%ld", wifiHandler.getConnectCounter(), mqttHandler.reconnectCounter, maxLoopTime );
    state++;
    break;

  case 7:
    display.display();
    state++;
    break;

  default:
    intervalTimestamp = timestamp;
    state = 0;
  }
}

void DisplayHandler::showBooting()
{
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString( 0, 0, "boot system" );
  display.drawString( 0, 10, "V." APP_VERSION );
  display.drawString( 0, 20, APP_AUTHOR );
  display.display();
}

void DisplayHandler::showWifiConnecting()
{
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString( 0, 0, "Connecting\nWiFi" );

  counter++;
  counter %= 4;

  display.fillCircle( 32, 32, ( counter * 2 ) + 3 );

  display.display();
}

void DisplayHandler::showUploadProgress(int value)
{
  char buffer[64];
  display.setFont(ArialMT_Plain_16);
  display.clear();
  display.drawString( 0, 0, "Upload" );
  display.drawStringf( 0, 20, buffer, "%d%%", value );
  display.drawProgressBar( 0, 41, 63, 6, value );
  display.display();
}

void DisplayHandler::showMessage( String message )
{
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString( 0, 0, message );
  display.display();
}

void DisplayHandler::showResetSystem()
{
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString( 0, 0, "*RESTART*" );
  display.display();
}
