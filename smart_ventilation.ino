/*
  PIN CONNECTIONS
  -------------------------------
  RF-433 RX
    GND  --> GND
    VCC  --> 5V
    DATA --> D2
  -------------------------------
  DISPLAY - I2C
    SDA is the serial data
    SCL is the serial clock

    GND --> GND
    VCC --> 5V
    SDA --> A4
    SCL --> A5
  -------------------------------
  DISPLAY - I2C
    SDA is the serial data
    SCL is the serial clock

  -------------------------------
  RTC-DS3231 - I2C
    SDA is the serial data
    SCL is the serial clock

  -------------------------------
  RELAY
    D5  --> Relay1
    D6  --> Relay2
    D7  --> Relay3
    D8  --> Relay4
  -------------------------------

*/
//= DEFINES ========================================================================================
//#define DEBUG
#define UseDisplay      // Log information and actions to the Display // uses 18% of memory
//#define RfLogsToSerial  // Print RF messages to Serial Terminal       // uses 9% of memory
//#define I2CLogsToSerial  // Print I2C messages to Serial Terminal       // uses ??% of memory
#define UseRealTimeClock  // Use the RTC
#define UseBlynkWifi  // Use the Blynk Wifi with ESP8266

//= INCLUDES =======================================================================================
#if defined(DEBUG) || defined(RfLogsToSerial) || defined(I2CLogsToSerial)
#include <stdio.h> // for function sprintf
#endif

#include "Remotes.h"
#include "Actions.h"

#include <Wire.h>   // using I2C
#include "PCF8575.h"
#include <RCSwitch.h>

#ifdef UseDisplay
#include <LiquidCrystal_I2C.h>
#endif

#ifdef UseRealTimeClock
#include <DS3231.h>
#endif

#ifdef UseBlynkWifi
#include "Secrets.h"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#endif

//= CONSTANTS ======================================================================================
const int LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED
const int LED_RED_PIN = 13;
//------------------------------------------------
#ifdef DEBUG
const byte TIME_TICK = 500;
#else
const byte TIME_TICK = 10;
#endif
//------------------------------------------------
// MANUAL MODE BUTTON
const bool IS_PRESSED = false;
//------------------------------------------------
const char RF_INTERRUPT_D2_PIN = 0; // RF Receiver on INT0 => pin D2
//------------------------------------------------
// Fast Check
const unsigned int RF_TARGET_PROTOCOL = 1;
const unsigned int RF_TARGET_BIT_COUNT = 24;
//------------------------------------------------

//--------------------------------------------------------------------------------------------------
// RELEE
const unsigned int RELAY_1_PIN = 5; // DIGITAL PORT 5
const unsigned int RELAY_2_PIN = 6; // DIGITAL PORT 6
const unsigned int RELAY_3_PIN = 7; // DIGITAL PORT 7
const unsigned int RELAY_4_PIN = 8; // DIGITAL PORT 8
//
//= VARIABLES ======================================================================================
RCSwitch rfRx = RCSwitch();

struct Action *previousAction = &NoAction;

//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("START-UP");
#endif
  // initialize digital pins
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  // i2C
  Wire.begin();
  //
  delay(TIME_TICK * 50);
  //
#ifdef UseDisplay
  display_Setup();
  display_Print2ndLine("<BOOTING>");
#endif
  //
  actions_Setup();
  //
  menu_Setup();
  //
  rfRx.enableReceive(RF_INTERRUPT_D2_PIN);
  //
  wifi_Setup();
  //
  delay(TIME_TICK * 50);
  digitalWrite(LED_INDICATOR_PIN, LOW);
  display_Print2ndLine("         ");
}
//**************************************************************************************************
//==================================================================================================
void loop() {
  wifi_Run();
  //
  if (rfRx.available()) {
    digitalWrite(LED_RED_PIN, HIGH);
    unsigned long buttonId = rfRx.getReceivedValue();
    if (isButtonValid_FastCheck(buttonId, rfRx.getReceivedProtocol(), rfRx.getReceivedBitlength(), rfRx.getReceivedDelay(), rfRx.getReceivedRawdata())) {

      Action *currentAction = actions_ComputeActionForButton(buttonId);

      if (currentAction->actionCode < ACTION_MAX_VALID) {
        if (currentAction != previousAction) {
          actions_ProcessAction(currentAction);

          previousAction = currentAction;
        }

        display_ShowProgress();
      }
    }

    rfRx.resetAvailable();
    digitalWrite(LED_RED_PIN, LOW);
    //
  } else {
    // GETS EXECUTED CONTINUOUSLY WHEN NO MESSAGE
    clock_TriggerIfAlarm();
    //
    menu_ActIfActivity();
    delay(TIME_TICK * 10);
    //
    //display_ShowProgress();
  }
}
//==================================================================================================
bool isButtonValid_FastCheck(unsigned long decimal, unsigned int protocol, unsigned int length, unsigned int delay, unsigned int* raw) {
  printRxToSerial(decimal, length, delay, raw, protocol);

  if ((protocol == RF_TARGET_PROTOCOL) && (length == RF_TARGET_BIT_COUNT)) {
    return true;
  } else {
#ifdef DEBUG
    Serial.println("????");
#endif
    return false;
  }
}
//==================================================================================================
//==================================================================================================
