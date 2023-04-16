//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

unsigned long CONNECTION_COLLDOWN_TIME = 20 * SEC;  // value in milliseconds
unsigned long PUBLISH_COLLDOWN_TIME = 5 * SEC;      // value in milliseconds

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);

unsigned long lastMqttConnection = 0;
unsigned long lastMqttPublish = 0;

//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
#ifdef DEBUG
  Serial.println("Mqtt:Setup >>>");
#endif
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  //mqttClient.setCallback(callback);
  //..............................
  delay(10 * TIME_TICK);
#ifdef DEBUG
  Serial.println("Mqtt:Setup <<<");
#endif
}
//**************************************************************************************************
//==================================================================================================
void mqtt_MaintainConnection() {
  if (__SkipConnect()) {
    return;
  }

  if (!mqttClient.connected()) {
    mqtt_Reconnect();
  }
  mqttClient.loop();
}
//==================================================================================================
void mqtt_Reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
#ifdef DEBUG
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    if (mqttClient.connect(HOST_NAME)) {
#ifdef DEBUG
      Serial.println("connected");
#endif
      // Subscribe
      mqttClient.subscribe("home/ventilation/command");
    } else {
#ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
#endif
      // Wait 5 seconds before retrying
      delay(500 * TIME_TICK);
    }
  }
}
//==================================================================================================
void mqtt_PublishInt(const char* topic, int value) {
  if (__SkipPublish()) {
    return;
  }
  // Convert the value to a char array
  char valueString[4];
  utoa((unsigned)value, valueString, 10);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishFloat(const char* topic, float value) {
  if (__SkipPublish()) {
    return;
  }
  // Convert the value to a char array
  char valueString[8];
  dtostrf(value, 1, 2, valueString);
  mqttClient.publish(topic, valueString);
}
//==================================================================================================
void mqtt_PublishString(const char* topic, const char* value) {
  if (__SkipPublish()) {
    return;
  }
  mqttClient.publish(topic, value);
}
//==================================================================================================
bool __SkipConnect() {
  if (millis() - lastMqttConnection <= CONNECTION_COLLDOWN_TIME) {
    return true;
  } else {
    lastMqttConnection = millis();
    return false;
  }
}
//==================================================================================================
bool __SkipPublish() {
  if (millis() - lastMqttPublish <= PUBLISH_COLLDOWN_TIME) {
    return true;
  } else {
    lastMqttPublish = millis();
    return false;
  }
}
//==================================================================================================