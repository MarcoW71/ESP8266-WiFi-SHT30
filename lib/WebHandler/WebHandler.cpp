#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WifiHandler.hpp>
#include <BufferedPrinter.hpp>
#include "WebHandler.hpp"
#include "WebHandlerPrivate.hpp"
#include "pure-min-css-gz.h"
#include "layout-css-gz.h"
#include "template-html.h"

WebHandler webHandler;

static AsyncWebServer server(80);

WebHandler::WebHandler()
{
  initialized = false;
}

void WebHandler::setup()
{
  LOG0("HTTP server setup...\n");

  server.on( "/", HTTP_GET, handleRootPage );
  server.on( "/setup.html", HTTP_GET, handleSetupPage );
  server.on( "/savecfg", HTTP_POST, handleSavePage );
  server.onNotFound( handlePageNotFound );

  setupRestApi(server);

  server.on("/pure-min.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css",
                                                                PURE_MIN_CSS_GZ, PURE_MIN_CSS_GZ_LEN);
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Cache-Control", "max-age=86400");
    request->send(response);
  }
            );

  server.on("/layout.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css",
                                                                LAYOUT_CSS_GZ, LAYOUT_CSS_GZ_LEN);
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Cache-Control", "max-age=86400");
    request->send(response);
  }
            );

  server.on("/info.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    printer.reset();
    printer.printf( TEMPLATE_HEADER, "", APP_NAME " - Info" );

    printer.print("<form class='pure-form'>");

    prLegend( "Application");

    printer.print(
      "<p>Name: " APP_NAME "</p>"
      "<p>Version: " APP_VERSION "</p>"
      "<p>Author: Dr. Thorsten Ludewig &lt;t.ludewig@gmail.com></p>");

    prLegend( "RESTful API");
    char ipAddress[16];
    strcpy( ipAddress, WiFi.localIP().toString().c_str());

    printer.printf(
      "<p><a href='http://%s/state'>http://%s/state</a> - JSON status</p>",
      ipAddress, ipAddress );

    printer.printf(
      "<p><a href='http://%s/time'>http://%s/time</a> - JSON time</p>",
      ipAddress, ipAddress );

    printer.printf(
      "<p><a href='http://%s/sensor'>http://%s/sensor</a> - JSON sensor</p>",
      ipAddress, ipAddress );

    prLegend( "Build");
    printer.print(
      "<p>Date: " __DATE__ "</p>"
      "<p>Time: " __TIME__ "</p>");

    prLegend( "Services");
    printer.printf( "<p>OpenHAB Enabled: %s</p>", (appcfg.ohab_enabled) ? "true" : "false" );
    printer.printf( "<p>MQTT Enabled: %s</p>", (appcfg.mqtt_enabled) ? "true" : "false" );

    printer.print("</form>");

    printer.printf( TEMPLATE_FOOTER );
    sendPrinterPage(request);
  }
            );

  MDNS.addService("http", "tcp", 80);
  MDNS.addServiceTxt( "http", "tcp", "path", "/" );
  MDNS.addServiceTxt( "http", "tcp", "fw_name", APP_NAME );
  MDNS.addServiceTxt( "http", "tcp", "fw_version", APP_VERSION );

  server.begin();

  LOG0("HTTP server started\n");
  initialized = true;
}

void WebHandler::handle()
{
  if( !initialized )
  {
    setup();
  }
}
