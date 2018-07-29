#include <Arduino.h>
#include <time.h>
#include <App.hpp>
#include <WifiHandler.hpp>
#include <OtaHandler.hpp>
#include <WebHandler.hpp>
#include <MqttHandler.hpp>
#include <LocalTimeHandler.hpp>
#include <SHT3xHandler.hpp>
#include <OpenHabHandler.hpp>
#include <DisplayHandler.hpp>

////////////////////////////////////////////////////////////////////////////////

time_t lifeTicker;
time_t maxLoopTime;
time_t lastLoopTimestamp;
time_t thisLoopTimestamp;

void setup()
{
  app.setup();
  app.writeConfig();
  app.printConfig();
  wifiHandler.setup();
  maxLoopTime = 0l;
  lifeTicker = lastLoopTimestamp = millis();
}

void loop()
{
   thisLoopTimestamp = millis();
   maxLoopTime = max( maxLoopTime, thisLoopTimestamp - lastLoopTimestamp );
   lastLoopTimestamp = thisLoopTimestamp;

   if (( thisLoopTimestamp - lifeTicker ) >= 10000)
   {
     LOG1( "max loop time = %ld\n", maxLoopTime );
     // LOG1( "wifi is connected %d\n", wifiHandler.isConnected());
     maxLoopTime = 0l;
     lifeTicker = thisLoopTimestamp;
   }

   if ( wifiHandler.handle( thisLoopTimestamp ))
   {
     localTimeHandler.handle();
     otaHandler.handle();
     webHandler.handle();
     sht3xHandler.handle(thisLoopTimestamp);
     displayHandler.handle(thisLoopTimestamp, maxLoopTime );
     openHabHandler.handle(thisLoopTimestamp);
     mqttHandler.handle(thisLoopTimestamp);
   }

   app.handle();
}
