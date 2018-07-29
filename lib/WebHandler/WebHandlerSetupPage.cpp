#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESPAsyncWebServer.h>
#include <WifiHandler.hpp>
#include <BufferedPrinter.hpp>
#include "WebHandler.hpp"
#include "WebHandlerPrivate.hpp"
#include "pure-min-css-gz.h"
#include "layout-css-gz.h"
#include "template-html.h"

void handleSetupPage( AsyncWebServerRequest *request )
{
  if(!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  int id = 0;

  printer.reset();
  printer.printf( TEMPLATE_HEADER, "", APP_NAME " - Setup");
  printer.println("<form class=\"pure-form pure-form-aligned\" action='/savecfg' method=POST><fieldset>");

  // Setup
  prLegend( "Setup 'admin' user" );
  prTextGroup( id++, "Password", "admin_password", appcfg.admin_password );


  // WiFi
  prLegend( "WiFi Network Scan" );
  printer.printf("<pre>%s</pre>\n", wifiHandler.getScannedNetworks() );

  prLegend( "WiFi" );

  prSelectStart( id++, "Mode", "wifi_mode" );
  prOption( WIFI_AP, "Access Point", appcfg.wifi_mode == WIFI_AP );
  prOption( WIFI_STA, "Station", appcfg.wifi_mode == WIFI_STA );
  prSelectEnd();

  prTextGroup( id++, "SSID", "wifi_ssid", appcfg.wifi_ssid );
  prTextGroup( id++, "Password", "wifi_password", appcfg.wifi_password );

  // OTA (Over The Air - firmware update)
  prLegend( "Over The Air - firmware update (OTA)");
  prTextGroup( id++, "Hostname", "ota_hostname", appcfg.ota_hostname );
  prTextGroup( id++, "Password", "ota_password", appcfg.ota_password );

  // OpenHAB
  prLegend( "OpenHAB");
  prCheckBoxGroup( id++, "Callback Enabled", "ohab_enabled", appcfg.ohab_enabled );

  prSelectStart( id++, "OpenHAB Version", "ohab_version" );
  prOption( 1, "1.8", appcfg.ohab_version == 1 );
  prOption( 2, "&gt;=2.0", appcfg.ohab_version == 2 );
  prSelectEnd();

  prTextGroup( id++, "Item Name - Temperature", "ohab_itemname_temp", appcfg.ohab_itemname_temp );
  prTextGroup( id++, "Item Name - Humidity", "ohab_itemname_humi", appcfg.ohab_itemname_humi );
  prTextGroup( id++, "Host", "ohab_host", appcfg.ohab_host );
  prTextGroup( id++, "Port", "ohab_port", appcfg.ohab_port );
  prCheckBoxGroup( id++, "Use Authentication", "ohab_useauth", appcfg.ohab_useauth );
  prTextGroup( id++, "User", "ohab_user", appcfg.ohab_user );
  prTextGroup( id++, "Password", "ohab_password", appcfg.ohab_password );

  // MQTT
  prLegend( "MQTT");
  prCheckBoxGroup( id++, "Enabled", "mqtt_enabled", appcfg.mqtt_enabled );
  prTextGroup( id++, "Client ID", "mqtt_clientid", appcfg.mqtt_clientid );
  prTextGroup( id++, "Host", "mqtt_host", appcfg.mqtt_host );
  prTextGroup( id++, "Port", "mqtt_port", appcfg.mqtt_port );
  prCheckBoxGroup( id++, "Use Authentication", "mqtt_useauth", appcfg.mqtt_useauth );
  prTextGroup( id++, "User", "mqtt_user", appcfg.mqtt_user );
  prTextGroup( id++, "Password", "mqtt_password", appcfg.mqtt_password );
  prTextGroup( id++, "Topic OUT", "mqtt_topic_out", appcfg.mqtt_topic_out );

  // NTP
  prLegend( "NTP");
  prTextGroup( id++, "Host", "ntp_host", appcfg.ntp_host );

  printer.println("<p><input class='pure-button pure-button-primary' type='submit' value='Save Configuration'></p>");
  printer.println("</fieldset></form>");

  // printer.printf( "<font size='5' color='grey'>%ld</font>", millis());
  printer.print( TEMPLATE_FOOTER );

  sendPrinterPage( request );
}


void handleSavePage( AsyncWebServerRequest *request )
{
  if(!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  printer.reset();
  printer.printf( TEMPLATE_HEADER, "", APP_NAME " - Save Configuration");
  printer.print("<pre>");

  int params = request->params();

  for(int i=0; i<params; i++)
  {
    AsyncWebParameter* p = request->getParam(i);
    printer.printf("%s = '%s'\n", p->name().c_str(), p->value().c_str());
  }

  // Security
  paramChars( request, appcfg.admin_password, "admin_password", DEFAULT_ADMIN_PASSWORD );

  // WIFI
  appcfg.wifi_mode = paramInt( request, "wifi_mode", DEFAULT_WIFI_MODE);
  paramChars( request, appcfg.wifi_ssid, "wifi_ssid", DEFAULT_WIFI_SSID );
  paramChars( request, appcfg.wifi_password, "wifi_password", DEFAULT_WIFI_PASSWORD );

  // OTA
  paramChars( request, appcfg.ota_hostname, "ota_hostname", DEFAULT_OTA_HOSTNAME );
  paramChars( request, appcfg.ota_password, "ota_password", DEFAULT_OTA_PASSWORD );

  // OpenHAB
  appcfg.ohab_enabled = paramBool( request, "ohab_enabled" );
  appcfg.ohab_version = paramInt( request, "ohab_version", DEFAULT_OHAB_VERSION );
  paramChars( request, appcfg.ohab_itemname_temp, "ohab_itemname_temp", DEFAULT_OHAB_ITEMNAME_TEMP );
  paramChars( request, appcfg.ohab_itemname_humi, "ohab_itemname_humi", DEFAULT_OHAB_ITEMNAME_HUMI );
  paramChars( request, appcfg.ohab_host, "ohab_host", DEFAULT_OHAB_HOST );
  appcfg.ohab_port = paramInt( request, "ohab_port", DEFAULT_OHAB_PORT );
  appcfg.ohab_useauth = paramBool( request, "ohab_useauth" );
  paramChars( request, appcfg.ohab_user, "ohab_user", DEFAULT_OHAB_USER );
  paramChars( request, appcfg.ohab_password, "ohab_password", DEFAULT_OHAB_PASSWORD );

  // MQTT
  appcfg.mqtt_enabled = paramBool( request, "mqtt_enabled" );
  paramChars( request, appcfg.mqtt_clientid, "mqtt_clientid", DEFAULT_MQTT_CLIENTID );
  paramChars( request, appcfg.mqtt_host, "mqtt_host", DEFAULT_MQTT_HOST );
  appcfg.mqtt_port = paramInt( request, "mqtt_port", DEFAULT_MQTT_PORT );
  appcfg.mqtt_useauth = paramBool( request, "mqtt_useauth" );
  paramChars( request, appcfg.mqtt_user, "mqtt_user", DEFAULT_MQTT_USER );
  paramChars( request, appcfg.mqtt_password, "mqtt_password", DEFAULT_MQTT_PASSWORD );
  paramChars( request, appcfg.mqtt_topic_out, "mqtt_topic_out", DEFAULT_MQTT_TOPIC_OUT );

  // NTP
  paramChars( request, appcfg.ntp_host, "ntp_host", DEFAULT_NTP_HOST );

  printer.println("</pre>");
  printer.println("<h2 style='color: red'>Restarting System in 5sec</h2>");
  printer.print( TEMPLATE_FOOTER );

  sendPrinterPage(request);

  app.delayedSystemRestart();
}
