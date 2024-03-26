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
#define UseCOMMPro        // Use the IoT module                         // uses ??% of memory
#define COMM_ROLE "OFFLINE-WORKER"

//= INCLUDES =======================================================================================
#include "Common.h"
#include "Remotes.h"
#include "Actions.h"

#include <Wire.h>  // using I2C
#include "PCF8575.h"

//= CONSTANTS ======================================================================================
const byte LED_INDICATOR_PIN = LED_BUILTIN;  // choose the pin for the LED // D13
//------------------------------------------------
const unsigned long AUTO_RESET_GRACE_PERIOD = 20 * HOUR;

//= VARIABLES ======================================================================================
byte currentVentSpeed = 0;
byte currentActionCode = ACTION_NOP;

//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println(F("START-UP >>>>>>>>>>>>>>>"));
#endif
  //..............................
  // initialize digital pin LED_INDICATOR_PIN as an output.
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(LED_INDICATOR_PIN, HIGH);
  // i2C
  Wire.begin();
  //
  delay(50 * TIME_TICK);
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
  comm_Setup();
  //
  delay(50 * TIME_TICK);
  //
  digitalWrite(LED_INDICATOR_PIN, LOW);
  display_Clear2ndLine();
  //..............................
#ifdef DEBUG
  Serial.println(F("START-UP <<<<<<<<<<<<<<<"));
#endif
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
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
    comm_ActIfReceivedMessage();
    //
    menu_ActIfActivity();
    delay(10 * TIME_TICK);
  }
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void __rebootAfterHours_atMidnight() {
  if (millis() >= AUTO_RESET_GRACE_PERIOD && clock_HourIsEqualTo(0)) {
    resetFunc();
  }
}
//==================================================================================================
void __printSwVersion() {
  char boot_message[16];
  sprintf(boot_message, "v%10s H:%02d", SW_VERSION, clock_GetHour());
  display_Print2ndLine(boot_message);
  delay(10 * SEC);
}
//==================================================================================================
//==================================================================================================