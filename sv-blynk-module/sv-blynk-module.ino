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
#define PUBLISH_ACTION_CODE_TOPIC "home/ventilation/unit-A/actionCode"
#define PUBLISH_ACTION_LABEL_TOPIC "home/ventilation/unit-A/actionLabel"

#define SUBSCRIBE_TOPIC "home/ventilation/unit-A/command"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define BLYNK_PRINT Serial
//#define DEBUG
//------------------------------------------------
// Various Features
#define UseCOMMPro  // Use the IoT module                         // uses ??% of memory

//= INCLUDES =======================================================================================
#include "Common.h"
#include "CommCommon.h"

#include "Secrets.h"
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h> // non-SSL
#include <BlynkSimpleEsp8266_SSL.h>

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
    currentVentSpeed = newVentSpeed;
    currentActionCode = actionCodeFromVentSpeed(newVentSpeed);

    comm_ActOnNewDataToSend();

    // Clear the ActionCode
    currentActionCode = 0;
  }
}
//##################################################################################################
// will run every time Action Widget writes values to Virtual Pin V1
BLYNK_WRITE(V1) {                           // values [1..90]
  int newActionCode = (byte)param.asInt();  // assigning incoming value from pin V1 to a variable

  if (newActionCode != currentActionCode) {
    currentActionCode = newActionCode;

    comm_ActOnNewDataToSend();

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

    // Clear the ActionCode
    currentActionCode = 0;
    Blynk.virtualWrite(V1, currentActionCode);
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
  comm_Setup();
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
  mqtt_PublishInt(PUBLISH_SPEED_TOPIC, currentVentSpeed);
  mqtt_PublishInt(PUBLISH_ACTION_CODE_TOPIC, currentActionCode);
}
//==================================================================================================