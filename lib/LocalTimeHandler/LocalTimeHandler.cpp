#include <Arduino.h>
#include <time.h>
#include <App.hpp>

#include <TimeLib.h>  //by Paul Stoffregen, not included in the Arduino IDE !!!
#include <Timezone.h> //by Jack Christensen, not included in the Arduino IDE !!!

#include <WifiHandler.hpp>
#include "LocalTimeHandler.hpp"

////////////////////////////////////////////////////////////////////////////////

//Timezone
//Central European Time (Frankfurt, Paris)
static TimeChangeRule CEST = { "CEST", Last, Sun, Mar, 2, 120 };     //Central European Summer Time
static TimeChangeRule CET = { "CET ", Last, Sun, Oct, 3, 60 };       //Central European Standard Time
static Timezone CE(CEST, CET);
static TimeChangeRule *tcr; //pointer to the time change rule, use to get the TZ abbrev
static time_t localTime;

////////////////////////////////////////////////////////////////////////////////

LocalTimeHandler localTimeHandler;

void printLocalTime()
{
  time_t now = 0;
  time(&now);
  localTime = CE.toLocal( now, &tcr );
  Serial.printf( "Local Time = %04d-%02d-%02d %02d:%02d:%02d %s\n",
                 year(localTime), month(localTime), day(localTime),
                 hour(localTime), minute(localTime), second(localTime),
                 tcr->abbrev );
}

LocalTimeHandler::LocalTimeHandler()
{
  initialized = false;
}

void LocalTimeHandler::handle()
{
  if ( !initialized && wifiHandler.isInStationMode() )
  {
    LOG0("Local Time Handler Setup...\n");

    time_t now = 0;
    configTime( 0, 0, appcfg.ntp_host );
    LOG0( "Wait for NTP sync " );
    while(!now)
    {
      time(&now);
      Serial.print(".");
      delay(100);
    }

    Serial.println( " done." );
    Serial.println(ctime(&now));
    printLocalTime();

    initialized = true;
  }
}

time_t LocalTimeHandler::getLocalTime()
{
  time_t now = 0;
  time(&now);
  localTime = CE.toLocal( now, &tcr );
  return localTime;
}

char *LocalTimeHandler::getLocalTimeText()
{
  getLocalTime();
  sprintf( timeBuffer, "%04d-%02d-%02d %02d:%02d:%02d %s",
                 year(localTime), month(localTime), day(localTime),
                 hour(localTime), minute(localTime), second(localTime),
                 tcr->abbrev );
  return timeBuffer;
}
