//= DEFINES ========================================================================================
//------------------------------------------------
// Various debug options
//#define DEBUG
//#define RfLogsToSerial   // Print RF messages to Serial Terminal       // uses 9% of memory
//#define I2CLogsToSerial  // Print I2C messages to Serial Terminal       // uses ??% of memory
//------------------------------------------------
// Various Features
#define UseDisplay        // Log information and actions to the Display // uses 18% of memory
#define UseRealTimeClock  // Use the RTC                                // uses 1% of memory

//= INCLUDES =======================================================================================
#if defined(DEBUG) || defined(RfLogsToSerial) || defined(I2CLogsToSerial)
#include <stdio.h>  // for function sprintf
#endif

#include "Common.h"
#include "Remotes.h"
#include "Actions.h"

#include <Wire.h>  // using I2C
#include "PCF8575.h"

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//------------------------------------------------
#ifdef DEBUG
const byte TIME_TICK = 500;
#else
const byte TIME_TICK = 10;
#endif
//= VARIABLES ======================================================================================

//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("START-UP >>>");
#endif
  //..............................
  // initialize digital pin LED_INDICATOR_PIN as an output.
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  // i2C
  Wire.begin();
  //
  delay(TIME_TICK * 50);
  //
  display_Setup();
  __printSwVersion();
  //
  actions_Setup();
  //
  scheduler_Setup();
  //
  menu_Setup();
  //
  rf433_Setup();
  //
  delay(TIME_TICK * 50);
  //
  digitalWrite(LED_INDICATOR_PIN, LOW);
  display_Clear2ndLine();
  //..............................
#ifdef DEBUG
  Serial.println(">>> START-UP");
#endif
}
//**************************************************************************************************
//==================================================================================================
void loop() {
  if (rf433_ActIfActivity()) {
    // act
  } else {
    // GETS EXECUTED CONTINUOUSLY WHEN NO MESSAGE
    //
    scheduler_CheckStatus();
    //
    clock_TriggerIfAlarm();
    //
    menu_ActIfActivity();
    delay(TIME_TICK * 10);
    //
    //display_ShowProgress();
  }
}
//==================================================================================================
void __printSwVersion() {
  char boot_message[16];
  sprintf(boot_message, "<BOOT v%8s>", SW_VERSION);
  display_Print2ndLine(boot_message);
}
//==================================================================================================
//==================================================================================================