#include <App.hpp>
#include <DefaultAppConfig.h>
#include <SHT3xHandler.hpp>
#include <LocalTimeHandler.hpp>
#include <BufferedPrinter.hpp>
#include "WebHandlerPrivate.hpp"
#include "template-html.h"

void handleRootPage( AsyncWebServerRequest *request )
{
  char titleBuffer[100];
  sprintf( titleBuffer, APP_NAME " - %s", appcfg.ota_hostname );
  printer.reset();
  printer.printf( TEMPLATE_HEADER, "", titleBuffer );
  printer.println("<form class=\"pure-form pure-form-aligned\"><fieldset>");

  prLegend( "Sensor");

  int rid=0;

  char valueBuffer[32];

  sprintf( valueBuffer, "%0.2f°C", sht3xHandler.cTemp );
  prTextGroupReadOnly( rid++, "Temperature", valueBuffer );
  sprintf( valueBuffer, "%0.2f%%", sht3xHandler.humidity );
  prTextGroupReadOnly( rid++, "Humidity", valueBuffer );
  sprintf( valueBuffer, "%0.2f°C", sht3xHandler.avgCTemp );
  prTextGroupReadOnly( rid++, "Avg Temperature", valueBuffer );
  sprintf( valueBuffer, "%0.2f%%", sht3xHandler.avgHumidity );
  prTextGroupReadOnly( rid++, "Avg Humidity", valueBuffer );

  prLegend( "NTP");
  prTextGroupReadOnly( rid++, "Local Time", localTimeHandler.getLocalTimeText() );

  printer.println("</fieldset></form>");

  printer.print( "<script>function getRestSensorState(){var t=document.getElementById('pgid0'),h=document.getElementById('pgid1'),at=document.getElementById('pgid2'),ah=document.getElementById('pgid3');fetch('/sensor').then(resp=>resp.json()).then(function(o){t.value=o.temperature.toFixed(2)+'°C',h.value=o.humidity.toFixed(2)+'%',at.value=o.avgTemperature.toFixed(1)+'°C',ah.value=o.avgHumidity.toFixed(1)+'%'})}setInterval(getRestSensorState,5e3);</script>" );
  printer.print( "<script>function getRestLocalTime(){var ntp=document.getElementById('pgid4');fetch('/time').then(resp=>resp.json()).then(function(o){ntp.value=o.localTime})}setInterval(getRestLocalTime,2e3);</script>" );
  printer.printf( TEMPLATE_FOOTER );
  sendPrinterPage(request);
}
