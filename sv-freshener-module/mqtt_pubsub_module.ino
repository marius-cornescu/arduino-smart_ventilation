//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Common.h"
#include "Secrets.h"
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

unsigned long PUBLISH_COLLDOWN_TIME = 5 * SEC;  // value in milliseconds

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);
unsigned long lastMqttPublish = 0;

//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
  debugPrintln(F("Mqtt:Setup >>>"));
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_Callback);
  //..............................
  delay(10 * TIME_TICK);
  debugPrintln(F("Mqtt:Setup <<<"));
}
//**************************************************************************************************
//==================================================================================================
void mqtt_MaintainConnection() {
  byte retry = 5;
  while (!mqttClient.connected() && retry > 0) {
    mqtt_Reconnect();
    retry--;
  }
  mqttClient.loop();
}
//==================================================================================================
void mqtt_Reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    debugPrint(F("Attempting MQTT connection..."));
    // Attempt to connect
    if (mqttClient.connect(HOST_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      debugPrintln("connected");
      // Subscribe
      mqttClient.subscribe(SUBSCRIBE_TOPIC);
    } else {
      debugPrint("failed, rc=");
      debugPrint(mqttClient.state());
      debugPrintln(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      delay(5 * SEC);
    }
  }
}
//==================================================================================================
void mqtt_Callback(char* topic, byte* message, unsigned int length) {
  debugPrint(F("Message arrived on topic: "));
  debugPrint(topic);
  debugPrint(F(". Message: "));

  String messageTemp;

  for (int i = 0; i < length; i++) {
    debugPrint((char)message[i]);
    messageTemp += (char)message[i];
  }
  debugPrintln();

  // if (String(topic).endsWith("/speed")) {
  //   byte newVentSpeed = messageTemp.toInt();
  //   onVentilationSpeedChanged(newVentSpeed, true);
  // } else if (String(topic).endsWith("/actionCode")) {
  //   byte newActionCode = messageTemp.toInt();
  //   onActionCodeChanged(newActionCode, true);
  // }
}
//==================================================================================================
void mqtt_PublishInt(const char* topic, int value) {
  // Convert the value to a char array
  char valueString[4];
  utoa((unsigned)value, valueString, 10);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishFloat(const char* topic, float value) {
  // Convert the value to a char array
  char valueString[8];
  dtostrf(value, 1, 2, valueString);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishString(const char* topic, const char* value) {
  mqttClient.publish(topic, value);
}
//==================================================================================================
bool mqtt_ShouldPublish() {
  if (millis() - lastMqttPublish <= PUBLISH_COLLDOWN_TIME) {
    return false;
  } else {
    lastMqttPublish = millis();
    return true;
  }
}
//==================================================================================================