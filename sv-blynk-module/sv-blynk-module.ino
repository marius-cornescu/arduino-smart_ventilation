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

#define SEC 1000  // 1 second

//= INCLUDES =======================================================================================
#include "Secrets.h"
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h> // non-SSL
#include <BlynkSimpleEsp8266_SSL.h>

#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
const char auth[] = BLYNK_AUTH_TOKEN;
// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;
//
const int LED_INDICATOR_PIN = LED_BUILTIN;

bool processReceivedMessage(const char* message);
const char* prepareMessageToSend();

//= VARIABLES ======================================================================================
BlynkTimer timer;
//..............................

RtznCommProtocol commProto("ONLINE-WORKER", &processReceivedMessage, &prepareMessageToSend);

byte currentVentSpeed = 0;
byte currentActionLabel = 0;
byte currentActionCode = 0;

//##################################################################################################
// function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  Blynk.sendInternal("meta", "set", "Device Model", "SV-Bridge");
  Blynk.sendInternal("meta", "set", "Serial Number", "ESP01s-SmartV");

  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
}
//##################################################################################################
// will run every time Slider Widget writes values to Virtual Pin V0
BLYNK_WRITE(V0) {                           // values [0..3]
  byte newVentSpeed = (byte)param.asInt();  // assigning incoming value from pin V0 to a variable

  if (newVentSpeed != currentVentSpeed) {
    currentVentSpeed = newVentSpeed;
    commProto.actOnPollMessage();
  }
}
//##################################################################################################
// will run every time Action Widget writes values to Virtual Pin V1
BLYNK_WRITE(V1) {                           // values [1..90]
  int newActionCode = (byte)param.asInt();  // assigning incoming value from pin V1 to a variable

  if (newActionCode != currentActionCode) {
    currentActionCode = newActionCode;
    currentActionLabel = newActionCode;
    commProto.actOnPollMessage();
    
    // Clear the ActionCode
    currentActionCode = 0;
    Blynk.virtualWrite(V1, currentActionCode);
  }
}
//##################################################################################################
void timerEvent() {
  // You can send any value at any time. Don't send more that 10 values per second.
  if (commProto.hasMessageInInboxThenReadMessage()) {
    commProto.actOnMessage();
  }
  //
  if (commProto.isHaveToPublish()) {
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
  Serial.println("VentMaster:Setup >>>");
#endif
  //..............................
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called X seconds
  timer.setInterval(1 * SEC, timerEvent);
  //..............................
#ifdef DEBUG
  Serial.println(">>> VentMaster:Setup");
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  Blynk.run();
  timer.run();  // Initiates BlynkTimer
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
  //------------------------------------
  byte value1 = message[0] - '0';
  if (currentVentSpeed != value1) {
    currentVentSpeed = value1;
    haveToPublish = true;
  }
  //------------------------------------
  byte value2 = message[1] - '0';
  if (currentActionLabel != value2) {
    currentActionLabel = value2;
    haveToPublish = true;
  }
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
const char* prepareMessageToSend() {
  char* message = new char[2];
  sprintf(message, "%1u%1u", currentVentSpeed, currentActionCode);
  return message;
}
//==================================================================================================
//==================================================================================================