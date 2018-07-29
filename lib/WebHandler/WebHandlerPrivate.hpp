#ifndef __WEB_HANDLER_PRIVATE_HPP__
#define __WEB_HANDLER_PRIVATE_HPP__

#include <ESPAsyncWebServer.h>

extern void handlePageNotFound( AsyncWebServerRequest *request );

extern void prLegend( const char *name );
extern void prGroupLabel( int id, const char *label );
extern void prTextGroup( int id, const char *label,const char *name, const char *value );
extern void prTextGroupReadOnly( int id, const char *label,const char *value );
extern void prTextGroup( int id, const char *label,const char *name, int value );
extern void prCheckBoxGroup( int id, const char *label,const char *name, bool value );
extern void prSelectStart( int id, const char *label, const char *name );
extern void prSelectEnd();
extern void prOption( int value, const char *name, bool selected );
extern void prOption( const char *value, const char *name, bool selected );

extern void paramChars( AsyncWebServerRequest *request, char *dest,const char* paramName, const char* defaultValue, bool methodPOST = true );
extern int paramInt( AsyncWebServerRequest *request, const char* paramName, int defaultValue );
extern bool paramBool( AsyncWebServerRequest *request, const char* paramName );

extern void handleSetupPage( AsyncWebServerRequest *request );
extern void handleSavePage( AsyncWebServerRequest *request );
extern void handleRootPage( AsyncWebServerRequest *request );

extern void setupRestApi( AsyncWebServer& server );

#endif
