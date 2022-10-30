/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------

*/
//= DEFINES ========================================================================================
/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial
//#define DEBUG

#define ESP8266_GPIO0 0  // HIGH is 3.3V
#define ESP8266_GPIO1 1  // TX
#define ESP8266_GPIO2 2  // HIGH is 3.3V
#define ESP8266_GPIO3 3  // RX
// Built-In LED: LOW is on and HIGH is off for blue led
#define ESP8266_BUILTIN_LED ESP8266_GPIO2

#define SEC 1000                // 1 second

//= INCLUDES =======================================================================================
#include "Secrets.h"
#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h> // non-SSL
#include <BlynkSimpleEsp8266_SSL.h>

//= CONSTANTS ======================================================================================
const int LED_INDICATOR_PIN = ESP8266_GPIO1;

const char auth[] = BLYNK_AUTH_TOKEN;
// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;

//= VARIABLES ======================================================================================
BlynkTimer timer;

//==================================================================================================
// function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  Blynk.sendInternal("meta", "set", "Device Model", "ESP8266");
  Blynk.sendInternal("meta", "set", "Serial Number", "ESP-Blynk-001");

  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
}
//==================================================================================================
// function will run every time Slider Widget writes values to Virtual Pin V1
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();  // assigning incoming value from pin V0 to a variable

  blinkPinXTimes(LED_INDICATOR_PIN, pinValue);
}
//==================================================================================================
void timerEvent() {
  // You can send any value at any time. Please don't send more that 10 values per second.
  //Blynk.virtualWrite(V5, millis() / 1000);
}
//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("START-UP");
#endif
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called every second
  timer.setInterval(1000L, timerEvent);
  //
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  blinkPinXTimes(LED_INDICATOR_PIN, 1);
}
//**************************************************************************************************
//==================================================================================================
void loop() {
  Blynk.run();
  timer.run();  // Initiates BlynkTimer
}
//==================================================================================================
void blinkPinXTimes(uint8_t pin, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    blinkPin(pin);
    delay(2 * SEC);
  }
}
//==================================================================================================
void blinkPin(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(1 * SEC);
  digitalWrite(pin, LOW);
}
//==================================================================================================