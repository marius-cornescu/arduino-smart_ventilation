#ifdef UseRealTimeClock

//= CONSTANTS ======================================================================================
#define RTC_I2C_ADDRESS 0x68

//= VARIABLES ======================================================================================
DS3231 rtcClock;
RTClib myRTC;

volatile byte a1_ScheduledAction = ACTION_NOP;
volatile byte a2_ScheduledAction = ACTION_NOP;

#endif
//**************************************************************************************************
void clock_Setup() {
#ifdef UseRealTimeClock

#endif
}
//**************************************************************************************************
//==================================================================================================
void clock_GetTimeStamp(char *timestamp) {
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
void clock_Alarm1_SetInMinutesWithAction(byte extraMinutes, byte scheduledAction) {
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
void clock_Alarm2_SetInMinutesWithAction(byte extraMinutes, byte scheduledAction) {
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
  clock_Alarm1_TriggerIfAlarm();
  clock_Alarm2_TriggerIfAlarm();
}
//==================================================================================================
void clock_Alarm1_TriggerIfAlarm() {
#ifdef UseRealTimeClock
  if (rtcClock.checkIfAlarm(1) && a1_ScheduledAction != ACTION_NOP) {
    actions_ProcessAction(a1_ScheduledAction);
    //
    a1_ScheduledAction = ACTION_NOP;
    //
#ifdef DEBUG
    Serial.println("Alarm 1 triggered");
#endif
    //
    display_Clear2ndLine();
  }
#endif
}
//==================================================================================================
void clock_Alarm2_TriggerIfAlarm() {
#ifdef UseRealTimeClock
  if (rtcClock.checkIfAlarm(2) && a2_ScheduledAction != ACTION_NOP) {
    actions_ProcessAction(a2_ScheduledAction);
    //
    a2_ScheduledAction = ACTION_NOP;
    //
#ifdef DEBUG
    Serial.println("Alarm 2 triggered");
#endif
    //
    display_Clear2ndLine();
  }
#endif
}
//==================================================================================================
void clock_ResetAlarms() {
#ifdef UseRealTimeClock
  rtcClock.turnOffAlarm(1);
  rtcClock.turnOffAlarm(2);
  //
  display_Clear2ndLine();
#endif
}
//==================================================================================================

//==================================================================================================
//==================================================================================================
