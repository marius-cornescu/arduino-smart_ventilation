#ifdef UseRealTimeClock

//= DEFINES ========================================================================================

//= INCLUDE ========================================================================================
#include "Common.h"
#include <DS3231.h>

//= CONSTANTS ======================================================================================
#define RTC_I2C_ADDRESS 0x68

//= VARIABLES ======================================================================================
DS3231 rtcClock;
RTClib myRTC;

struct Action *a1_ScheduledAction = &NoAction;
struct Action *a2_ScheduledAction = &NoAction;

#endif
//==================================================================================================
//**************************************************************************************************
void clock_Setup() {
#ifdef UseRealTimeClock
#ifdef DEBUG
  Serial.println(F("CLOCK:Setup >>>"));
#endif
  //..............................
  //..............................
#ifdef DEBUG
  Serial.println(F("CLOCK:Setup <<<"));
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void clock_Alarm1_SetInMinutesWithAction(byte extraMinutes, struct Action *scheduledAction) {
#ifdef UseRealTimeClock
  a1_ScheduledAction = scheduledAction;
  //
  DateTime now = myRTC.now();
  rtcClock.setA1Time(now.day(), now.hour(), now.minute() + extraMinutes,
                     now.second(),
                     0x0, false, false, false);
  // Turn on alarm, with external interrupt
  rtcClock.turnOnAlarm(1);
  //
  display_Print2ndLine("<A1>");
#endif
}
//==================================================================================================
void clock_Alarm2_SetInMinutesWithAction(byte extraMinutes, struct Action *scheduledAction) {
#ifdef UseRealTimeClock
  a2_ScheduledAction = scheduledAction;
  //
  DateTime now = myRTC.now();
  rtcClock.setA2Time(now.day(), now.hour(), now.minute() + extraMinutes,
                     0x0, false, false, false);
  // Turn on alarm, with external interrupt
  rtcClock.turnOnAlarm(2);
  //
  display_Print2ndLine("<A2>");
#endif
}
//==================================================================================================
void clock_TriggerIfAlarm() {
#ifdef UseRealTimeClock
  __Alarm1_TriggerIfAlarm();
  __Alarm2_TriggerIfAlarm();
#endif
}
//==================================================================================================
#ifdef UseRealTimeClock
//==================================================================================================
void __Alarm1_TriggerIfAlarm() {
  if (rtcClock.checkIfAlarm(1) && a1_ScheduledAction != &NoAction) {
    actions_ProcessAction(a1_ScheduledAction);
    //
    a1_ScheduledAction = &NoAction;
    //
#ifdef DEBUG
    Serial.println(F("Alarm 1 triggered"));
#endif
    //
    display_Clear2ndLine();
  }
}
//==================================================================================================
void __Alarm2_TriggerIfAlarm() {
  if (rtcClock.checkIfAlarm(2) && a2_ScheduledAction != &NoAction) {
    actions_ProcessAction(a2_ScheduledAction);
    //
    a2_ScheduledAction = &NoAction;
    //
#ifdef DEBUG
    Serial.println(F("Alarm 2 triggered"));
#endif
    //
    display_Clear2ndLine();
  }
}
//==================================================================================================
void __ResetAlarms() {
  rtcClock.turnOffAlarm(1);
  rtcClock.turnOffAlarm(2);
  //
  display_Clear2ndLine();
}
//==================================================================================================
void __GetTimeStamp(char *timestamp) {
#ifdef UseRealTimeClock
  DateTime now = myRTC.now();
  sprintf(timestamp, "%02d/%02d/%02d %02d:%02d:%02d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());

#ifdef DEBUG
  Serial.print(F("Date/Time: "));
  Serial.println(timestamp);
#endif
#endif
}
//==================================================================================================
#endif
//==================================================================================================