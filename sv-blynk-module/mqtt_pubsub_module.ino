//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include <PubSubClient.h>

//= CONSTANTS ======================================================================================
// MQTT Broker IP address, example:
const char* mqtt_server = MQTT_BROKER_ADDRESS;

unsigned long PUBLISH_COLLDOWN_TIME = 5 * SEC;  // value in milliseconds

//= VARIABLES ======================================================================================
PubSubClient mqttClient(espClient);
unsigned long lastMqttPublish = 0;

//==================================================================================================
//**************************************************************************************************
void mqtt_Setup() {
#ifdef DEBUG
  Serial.println(F("Mqtt:Setup >>>"));
#endif
  //..............................
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_Callback);
  //..............................
  delay(10 * TIME_TICK);
#ifdef DEBUG
  Serial.println(F("Mqtt:Setup <<<"));
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
    Serial.print(F("Attempting MQTT connection..."));
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
      Serial.println(F(" try again in 5 seconds"));
#endif
      // Wait 5 seconds before retrying
      delay(500 * TIME_TICK);
    }
  }
}
//==================================================================================================
void mqtt_Callback(char* topic, byte* message, unsigned int length) {
#ifdef DEBUG
  Serial.print(F("Message arrived on topic: "));
  Serial.print(topic);
  Serial.print(F(". Message: "));
#endif
  String messageTemp;

  for (int i = 0; i < length; i++) {
#ifdef DEBUG
    Serial.print((char)message[i]);
#endif
    messageTemp += (char)message[i];
  }
#ifdef DEBUG
  Serial.println();
#endif

  if (String(topic).endsWith("/speed")) {
    byte newVentSpeed = messageTemp.toInt();
    onVentilationSpeedChanged(newVentSpeed, true);
  } else if (String(topic).endsWith("/actionCode")) {
    byte newActionCode = messageTemp.toInt();
    onActionCodeChanged(newActionCode, true);
  }
}
//==================================================================================================
void mqtt_PublishInt(const char* topic, byte value) {
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