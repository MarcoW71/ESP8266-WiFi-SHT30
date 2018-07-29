#ifndef __SHT3X_HANDLER_H__
#define __SHT3X_HANDLER_H__

#include <Arduino.h>

#define UPDATE_INTERVAL 5000l
#define AVG_NUM_INTERVALS 24  // 2min, 120s = 5s * 24

#define STATE_SEND_COMMAND 0
#define STATE_READ_DATA 1
#define TEMP_CAL 0.91f

class SHT3xHandler
{
private:
  bool initialized;
  uint8_t i2cAddress;
  time_t intervalTimestamp;
  uint8_t data[6];
  float tmpAvgCTemp;
  float tmpAvgHumidity;
  int state;

  int updateSHT3xValues();

public:
  int avgCounter;
  float cTemp=0.0f;
  float humidity=0.0f;
  float avgCTemp=0.0f;
  float avgHumidity=0.0f;
  bool avgValuesAvailable=false;
  time_t lastValidDataTimestamp;

  SHT3xHandler(uint8_t _address=0x45);
  void handle(time_t timestamp);
};

extern SHT3xHandler sht3xHandler;

#endif
