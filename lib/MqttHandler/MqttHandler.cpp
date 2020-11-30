#include <Arduino.h>
#include <App.hpp>
#include <WifiHandler.hpp>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <LocalTimeHandler.hpp>
#include <SHT3xHandler.hpp>
#include <time.h>

#include "MqttHandler.hpp"

MqttHandler mqttHandler;

static WiFiClient wifiClient;
static PubSubClient client(wifiClient);
static long lastReconnectAttempt = 0;

MqttHandler::MqttHandler()
{
  initialized = false;
}

bool MqttHandler::reconnect()
{
  if (( appcfg.mqtt_useauth && client.connect(appcfg.mqtt_clientid, appcfg.mqtt_user, appcfg.mqtt_password ))
      || ( !appcfg.mqtt_useauth && client.connect(appcfg.mqtt_clientid)))
  {
    LOG0( "MQTT broker connected\n" );
    reconnectCounter++;
    // client.subscribe( appcfg.mqtt_intopic );
  }

  return client.connected();
}

void MqttHandler::setup()
{
  LOG0("MQTT Setup...\n");
  client.setServer( appcfg.mqtt_host, appcfg.mqtt_port );
  initialized = true;
  intervalTimestamp = 0;
  reconnectCounter = 0;
  LOG1("MQTT enabled=%s\n", (appcfg.mqtt_enabled) ? "true" : "false" );
}

void MqttHandler::handle(time_t timestamp)
{
  if ( appcfg.mqtt_enabled && wifiHandler.isReady())
  {
    if ( initialized == false )
    {
      setup();
    }

    if (!client.connected())
    {
      long now = millis();

      if (now - lastReconnectAttempt > 5000l)
      {
        LOG0( "MQTT client not connected\n" );
        lastReconnectAttempt = now;

        if (reconnect())
        {
          lastReconnectAttempt = 0;
          LOG1( "MQTT client state=%d\n", client.state());
        }
      }
    }
    else
    {
      client.loop();

      if (( timestamp - intervalTimestamp ) >= 15000 )
      {
        intervalTimestamp = timestamp;

        time_t now = 0;
        time(&now);

        String message = "{";
        message += "\"temperature_C\":";
        message += sht3xHandler.cTemp;
        message += ",\"humidity\":";
        message += sht3xHandler.humidity;
        message += "}\r\n";

        sendValue(message.c_str());
      }
    }
  }
}

void MqttHandler::sendValue( const char* value )
{
  if( appcfg.mqtt_enabled && wifiHandler.isReady() && client.connected())
  {
    LOG1("MqttHandler::sendValue topic=%s\n", appcfg.mqtt_topic_out  );
    LOG1("MqttHandler::sendValue msg=%s", value );
    client.publish( appcfg.mqtt_topic_out, value );
  }
}
