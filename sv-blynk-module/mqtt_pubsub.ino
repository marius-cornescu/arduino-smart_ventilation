//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

unsigned long PUBLISH_COLLDOWN_TIME = 5 * SEC;      // value in milliseconds

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);
unsigned long lastMqttPublish = 0;

//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
#ifdef DEBUG
  Serial.println("Mqtt:Setup >>>");
#endif
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_Callback);
  //..............................
  delay(10 * TIME_TICK);
#ifdef DEBUG
  Serial.println("Mqtt:Setup <<<");
#endif
}
//**************************************************************************************************
//==================================================================================================
void mqtt_MaintainConnection() {
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
      mqttClient.subscribe(SUBSCRIBE_TOPIC);
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
void mqtt_Callback(char* topic, byte* message, unsigned int length) {
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