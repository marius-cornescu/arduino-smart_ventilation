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
//#define BLYNK_PRINT Serial
//#define DEBUG

//= INCLUDES =======================================================================================
#include "Common.h"
#include "CommCommon.h"

#include "Secrets.h"
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h> // non-SSL
#include <BlynkSimpleEsp8266_SSL.h>

#include "Artizan-CommProtocol.h"
#include "Actions.h"

//= CONSTANTS ======================================================================================
const char host_name[] = HOST_NAME;
const char auth[] = BLYNK_AUTH_TOKEN;
// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;
//
const int LED_INDICATOR_PIN = LED_BUILTIN;

//= VARIABLES ======================================================================================
BlynkTimer timer;
//..............................
WiFiClient espClient;
//..............................

byte currentVentSpeed = 0;
byte currentActionCode = ACTION_NOP;
char currentActionLabel[LABEL_PAYLOAD_SIZE + 1];

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
    currentActionCode = currentAction->actionCode;

    commProto.actOnPollMessage();

    // Clear the ActionCode
    currentActionCode = 0;
    Blynk.virtualWrite(V1, currentActionCode);
  }
}
//##################################################################################################
void timerEvent() {
  if (comm_ActIfReceivedMessage()) {

    Blynk.virtualWrite(V0, currentVentSpeed);
    Blynk.virtualWrite(V1, currentActionCode);
    Blynk.virtualWrite(V2, currentActionLabel);

    mqtt_PublishUpdate();
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
  Serial.println("VentMaster:Setup >>>");
#endif
  //..............................
  WiFi.hostname(host_name);
  //
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called X seconds
  timer.setInterval(1 * SEC, timerEvent);
  //
  comm_Setup();
  //
  mqtt_Setup();
  //..............................
#ifdef DEBUG
  Serial.println("VentMaster:Setup <<<");
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
//==================================================================================================
const char* prepareMessageToSend() {
  char* message = new char[4];
  memset(message, 0, 4);
  message[0] = currentVentSpeed + (byte)'0';
  message[1] = currentActionCode + (byte)'0';

  return message;
}
//==================================================================================================
byte actionCodeFromVentSpeed(byte newVentSpeed) {
  byte newActionCode = ACTION_NOP;

  if (newVentSpeed == 0) {
    newActionCode = ACTION_4;
  }
  if (newVentSpeed == 1) {
    if (currentVentSpeed == 0) {
      newActionCode = ACTION_5;
    } else {
      newActionCode = ACTION_1;
    }
  }
  if (newVentSpeed == 2) {
    newActionCode = ACTION_2;
  }
  if (newVentSpeed == 3) {
    newActionCode = ACTION_3;
  }

  return newActionCode;
}
//==================================================================================================
void mqtt_PublishUpdate() {
  mqtt_PublishInt("home/ventilation/unit-A/speed", currentVentSpeed);
  mqtt_PublishInt("home/ventilation/unit-A/actionCode", currentActionCode);
  mqtt_PublishInt("home/ventilation/unit-A/actionLabel", 0); // currentActionLabel
}
//==================================================================================================