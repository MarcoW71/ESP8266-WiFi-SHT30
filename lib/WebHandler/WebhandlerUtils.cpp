#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <BufferedPrinter.hpp>
#include "WebHandler.hpp"
#include "WebHandlerPrivate.hpp"

void handlePageNotFound( AsyncWebServerRequest *request )
{
  request->send(404);
}

void prLegend( const char *name )
{
  printer.printf( "<legend>%s</legend>", name );
}

void prGroupLabel( int id, const char *label )
{
  printer.printf(
    "<div class='pure-control-group'>"
    "<label for='pgid%d'>%s</label>", id, label );
}

void prTextGroup( int id, const char *label,const char *name, const char *value )
{
  prGroupLabel( id, label );
  printer.printf(
    "<input id='pgid%d' type='text' name='%s' maxlength='64' value='%s'>"
    "</div>", id, name, value );
}

void prTextGroupReadOnly( int id, const char *label,const char *value )
{
  prGroupLabel( id, label );
  printer.printf(
    "<input id='pgid%d' type='text' maxlength='64' readonly value='%s'>"
    "</div>", id, value );
}

void prTextGroup( int id, const char *label,const char *name, int value )
{
  prGroupLabel( id, label );
  printer.printf(
    "<input id='pgid%d' type='text' name='%s' maxlength='64' value='%d'>"
    "</div>", id, name, value );
}

void prCheckBoxGroup( int id, const char *label,const char *name, bool value )
{
  prGroupLabel( id, label );
  printer.printf(
    "<input id='pgid%d' type='checkbox' name='%s' value='true' %s>"
    "</div>", id, name, value ? "checked" : "" );
}

void prSelectStart( int id, const char *label, const char *name )
{
  prGroupLabel( id, label );
  printer.printf("<select id='pgid%d' name='%s'>", id, name );
}

void prSelectEnd()
{
  printer.print("</select></div>" );
}

void prOption( int value, const char *name, bool selected )
{
  printer.printf("<option %s value='%d'>%s</option>", selected ? "selected" : "", value, name );
}

void prOption( const char *value, const char *name, bool selected )
{
  printer.printf("<option %s value='%s'>%s</option>", selected ? "selected" : "", value, name );
}

void paramChars( AsyncWebServerRequest *request, char *dest,const char* paramName, const char* defaultValue, bool methodPOST )
{
  const char *value = defaultValue;

  if(request->hasParam( paramName, methodPOST ))
  {
    AsyncWebParameter* p = request->getParam(paramName, methodPOST );
    value = p->value().c_str();

    if ( value == 0 || strlen( value ) == 0 )
    {
      value = defaultValue;
    }
  }

  strncpy( dest, value, 63 );
  dest[63] = 0;
}

int paramInt( AsyncWebServerRequest *request, const char* paramName, int defaultValue )
{
  int value = defaultValue;

  if(request->hasParam( paramName, true ))
  {
    AsyncWebParameter* p = request->getParam(paramName, true);
    const char *pv = p->value().c_str();

    if ( pv != 0 && strlen( pv ) > 0 )
    {
      value = atoi( pv );
    }
  }

  return value;
}

bool paramBool( AsyncWebServerRequest *request, const char* paramName )
{
  bool value = false;

  if(request->hasParam( paramName, true ))
  {
    AsyncWebParameter* p = request->getParam(paramName, true);
    const char *pv = p->value().c_str();

    if ( pv != 0 && strlen( pv ) > 0 )
    {
      value = strcmp( "true", pv ) == 0;
    }
  }

  return value;
}
