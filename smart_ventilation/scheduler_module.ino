//= DEFINES ========================================================================================
//= INCLUDE ========================================================================================
#include "Common.h"

//= CONSTANTS ======================================================================================
const byte VENT_STATUS_PIN = 4;  // DIGITAL PORT 4
const unsigned long SCHEDULER_GRACE_PERIOD = 60 * SEC;  // how long to wait until registering change in scheduler
//----------------------------------

//= VARIABLES ======================================================================================
static unsigned int lastVentStatusState = HIGH;
unsigned long lastVentStatusUpdateMillis = 0;

//==================================================================================================
//**************************************************************************************************
void scheduler_Setup() {
#ifdef DEBUG
  Serial.println(F("SCHEDULER:Setup >>>"));
#endif
  //..............................
  pinMode(VENT_STATUS_PIN, INPUT);
  lastVentStatusUpdateMillis = 5 * SCHEDULER_GRACE_PERIOD; // ignore updates in the begining / let the system stabilize
  //..............................
#ifdef DEBUG
  Serial.println(F("SCHEDULER:Setup <<<"));
#endif
}
//**************************************************************************************************
//==================================================================================================
void scheduler_CheckStatus() {
  unsigned int current = digitalRead(VENT_STATUS_PIN);
  if (__IsAlowedToChange() && current != lastVentStatusState) {
    lastVentStatusState = current;
    lastVentStatusUpdateMillis = millis();
    actions_onSchedulerStatusChange(lastVentStatusState == HIGH);
  }
}
//==================================================================================================
bool __IsAlowedToChange() {
  return millis() - lastVentStatusUpdateMillis > SCHEDULER_GRACE_PERIOD;
}
//==================================================================================================
//==================================================================================================