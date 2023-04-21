/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------
*/
//= DEFINES ========================================================================================
//
#define ESP8266_GPIO0 0  // HIGH is 3.3V
#define ESP8266_GPIO1 1  // TX
#define ESP8266_GPIO2 2  // HIGH is 3.3V
#define ESP8266_GPIO3 3  // RX
// Built-In LED: LOW is on and HIGH is off for blue led
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PUBLISH_SPEED_TOPIC "home/ventilation/unit-A/speed"
#define PUBLISH_ACTION_TOPIC "home/ventilation/unit-A/actionCode"
#define SUBSCRIBE_TOPIC "home/ventilation/unit-A/command"

#define COMM_ROLE "ONLINE-WORKER"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define BLYNK_PRINT Serial
//#define DEBUG

//= INCLUDES =======================================================================================
#include "Common.h"
#include "Secrets.h"
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h> // non-SSL
#include <BlynkSimpleEsp8266_SSL.h>

#include "CommCommon.h"
#include "RtznCommProtocol.h"
#include "Actions.h"

//= CONSTANTS ======================================================================================
const char host_name[] = HOST_NAME;
const char auth[] = BLYNK_AUTH_TOKEN;
// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;
//
const int LED_INDICATOR_PIN = LED_BUILTIN;

bool processReceivedMessage(const char* message);
void prepareMessageToSend(char* message);

//= VARIABLES ======================================================================================
BlynkTimer timer;
//..............................
WiFiClient espClient;
//..............................
RtznCommProtocol commProto(COMM_ROLE, PAYLOAD_SIZE, &processReceivedMessage, &prepareMessageToSend);

byte currentVentSpeed = 0;
byte currentActionCode = ACTION_NOP;

const char* currentActionLabel = (&NoAction)->description;

//##################################################################################################
// function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  Blynk.sendInternal("meta", "set", "Device Model", "SV-Bridge");
  Blynk.sendInternal("meta", "set", "Serial Number", "ESP01s-SmartV");

  // Request Blynk server to re-send latest values for all pins
  // Blynk.syncAll();
}
//##################################################################################################
// will run every time Slider Widget writes values to Virtual Pin V0
BLYNK_WRITE(V0) {                           // values [0..3]
  byte newVentSpeed = (byte)param.asInt();  // assigning incoming value from pin V0 to a variable

  if (newVentSpeed != currentVentSpeed) {
    currentActionCode = actionCodeFromVentSpeed(newVentSpeed);
    currentVentSpeed = newVentSpeed;

    commProto.actOnPollMessage();

    // Clear the ActionCode
    currentActionCode = 0;
  }
}
//##################################################################################################
// will run every time Action Widget writes values to Virtual Pin V1
BLYNK_WRITE(V1) {                           // values [1..90]
  int newActionCode = (byte)param.asInt();  // assigning incoming value from pin V1 to a variable

  if (newActionCode != currentActionCode) {
    const Action* currentAction = getActionByActionCode(newActionCode);

    currentActionCode = currentAction->actionCode;
    currentActionLabel = currentAction->description;

    commProto.actOnPollMessage();

    // Clear the ActionCode
    currentActionCode = 0;
    Blynk.virtualWrite(V1, currentActionCode);
  }
}
//##################################################################################################
void timerEvent() {
  // You can send any value at any time. Don't send more that 10 values per second.
  if (commProto.hasMessageInInboxThenAct() && commProto.isHaveToPublish()) {
    Blynk.virtualWrite(V0, currentVentSpeed);
    Blynk.virtualWrite(V1, currentActionCode);
    Blynk.virtualWrite(V2, currentActionLabel);
    commProto.setHaveToPublish(false);
  }
}
//##################################################################################################
//==================================================================================================
//**************************************************************************************************
void setup() {
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  //..............................
#ifdef DEBUG
  Serial.println(F("START-UP >>>>>>>>>>>>>>>"));
#endif
  //..............................
  WiFi.hostname(host_name);
  //
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called X seconds
  timer.setInterval(1 * SEC, timerEvent);
  //
  mqtt_Setup();
  //..............................
#ifdef DEBUG
  Serial.println(F("START-UP <<<<<<<<<<<<<<<"));
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  Blynk.run();
  timer.run();  // Initiates BlynkTimer
  mqtt_MaintainConnection();
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
  //------------------------------------
  byte newVentSpeed = message[0] - (byte)'0';
  // ignore - the meat is in the action
  //------------------------------------
  byte newActionCode = message[1] - (byte)'0';
  if (ACTION_NOP != newActionCode) {
    const Action* newAction = getActionByActionCode(newActionCode);

    currentVentSpeed = ventSpeedFromActionCode(newAction);
    currentActionLabel = newAction->description;

    haveToPublish = true;

    mqtt_PublishUpdate();
  }
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
  message[0] = currentVentSpeed + (byte)'0';
  message[1] = currentActionCode + (byte)'0';

  mqtt_PublishUpdate();
}
//==================================================================================================
byte actionCodeFromVentSpeed(byte newVentSpeed) {
  byte newActionCode = ACTION_NOP;

  if (newVentSpeed == 0) {
    newActionCode = ActionVentOff.actionCode;
  }
  if (newVentSpeed == 1) {
    if (currentVentSpeed == 0) {
      newActionCode = ActionVentOn.actionCode;
    } else {
      newActionCode = ActionVent1.actionCode;
    }
  }
  if (newVentSpeed == 2) {
    newActionCode = ActionVent2.actionCode;
  }
  if (newVentSpeed == 3) {
    newActionCode = ActionVent3.actionCode;
  }

  return newActionCode;
}
//==================================================================================================
byte ventSpeedFromActionCode(const Action* newAction) {
  byte newVentSpeed = currentVentSpeed;

  if (newAction == &ActionVentOff) {
    newVentSpeed = 0;
  }
  if (newAction == &ActionVent1) {
    newVentSpeed = 1;
  }
  if (newAction == &ActionVent2) {
    newVentSpeed = 2;
  }
  if (newAction == &ActionVent3 || newAction == &ActionVent3Vent1Short || newAction == &ActionVent3Vent1Long) {
    newVentSpeed = 3;
  }

  return newVentSpeed;
}
//==================================================================================================
void mqtt_PublishUpdate() {
  mqtt_PublishInt(PUBLISH_SPEED_TOPIC, currentVentSpeed);
  mqtt_PublishInt(PUBLISH_ACTION_TOPIC, currentActionCode);
}
//==================================================================================================