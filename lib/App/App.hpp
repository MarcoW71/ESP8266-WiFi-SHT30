#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>

#define LOG0( format ) Serial.printf( "(%ld) " format, millis())
#define LOG1( format, x) Serial.printf( "(%ld) " format, millis(), x )

#define APP_NAME "WiFi SHT30"
#define APP_VERSION "1.0.0"
#define APP_AUTHOR "Dr. Thorsten Ludewig <t.ludewig@gmail.com>"
#define APP_CONFIG_FILE "/config.bin"

typedef struct appconfig
{
  char wifi_ssid[64];
  char wifi_password[64];
  int  wifi_mode;

  char ota_hostname[64];
  char ota_password[64];

  char admin_password[64];

  bool ohab_enabled;
  int  ohab_version;
  char ohab_host[64];
  int  ohab_port;
  bool ohab_useauth;
  char ohab_user[64];
  char ohab_password[64];
  char ohab_itemname_temp[64];
  char ohab_itemname_humi[64];

  bool mqtt_enabled;
  char mqtt_clientid[64];
  char mqtt_host[64];
  int  mqtt_port;
  bool mqtt_useauth;
  char mqtt_user[64];
  char mqtt_password[64];
  char mqtt_topic_out[64];

  char ntp_host[64];

} AppConfig;

class App
{
private:
  bool initialized = false;
  bool doSystemRestart = false;
  time_t systemRestartTimestamp;

  void loadConfig();
  void restartSystem();

public:
  App();

  void setup();
  void writeConfig();
  void printConfig();
  void delayedSystemRestart();
  void handle();
  unsigned long secTimestamp();
};

extern App app;
extern AppConfig appcfg;

#endif
