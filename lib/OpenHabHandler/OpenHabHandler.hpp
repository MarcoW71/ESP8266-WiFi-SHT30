#ifndef __OPENHAB_HANDLER_H__
#define __OPENHAB_HANDLER_H__

#include <Arduino.h>

class OpenHabHandler
{
private:
  void sendValueV1( const char* itemName, float value );
  void sendValueV2( const char* itemName, float value );
  time_t intervalTimestamp = 0;

public:
  OpenHabHandler();
  void sendValue( const char* itemName, float value );
  void handle(time_t timestamp);

};

extern OpenHabHandler openHabHandler;

#endif
