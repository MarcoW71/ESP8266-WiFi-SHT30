#ifndef __DISPLAY_HANDLER_H__
#define __DISPLAY_HANDLER_H__

#include <Arduino.h>

#define DISPLAY_UPDATE_INTERVAL 1000l

class DisplayHandler
{
private:
  bool initialized;
  uint8_t i2cAddress;
  time_t intervalTimestamp;
  int state = 0;
  int counter;

public:
  DisplayHandler();
  void handle( time_t timestamp, time_t maxLoopTime );
  void showBooting();
  void showWifiConnecting();
  void showUploadProgress(int value);
  void showMessage(String message);
  void showResetSystem();
};

extern DisplayHandler displayHandler;

#endif
