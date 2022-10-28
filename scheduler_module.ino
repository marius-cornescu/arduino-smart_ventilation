//= DEFINES ========================================================================================
//= INCLUDE ========================================================================================
#include "Common.h"

//= CONSTANTS ======================================================================================
const byte VENT_STATUS_PIN = 4;  // DIGITAL PORT 4
//----------------------------------

//= VARIABLES ======================================================================================
static unsigned int lastVentStatusState = HIGH;

//**************************************************************************************************
void scheduler_Setup() {
#ifdef DEBUG
  Serial.println("SCHEDULER:Setup >>>");
#endif
  pinMode(VENT_STATUS_PIN, INPUT);
#ifdef DEBUG
  Serial.println(">>> SCHEDULER:Setup");
#endif
}
//**************************************************************************************************
//==================================================================================================
void scheduler_CheckStatus() {
  unsigned int current = digitalRead(VENT_STATUS_PIN);
  if (current != lastVentStatusState) {
    lastVentStatusState = current;
    actions_onVentilationOnInV1Change(lastVentStatusState == HIGH);
  }
}
//==================================================================================================
//==================================================================================================