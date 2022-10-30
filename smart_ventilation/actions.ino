//= DEFINES ========================================================================================
//= INCLUDE ========================================================================================
#include "Common.h"
#include "Actions.h"

//= CONSTANTS ======================================================================================

// RELAYs
const byte RELAY_1_PIN = 5;  // DIGITAL PORT 5
const byte RELAY_2_PIN = 6;  // DIGITAL PORT 6
const byte RELAY_3_PIN = 7;  // DIGITAL PORT 7
const byte RELAY_4_PIN = 8;  // DIGITAL PORT 8
//--------------------------------------------------------------------------------------------------

//= VARIABLES ======================================================================================
const Action *previousAction = &NoAction;
// is the ventilation status pin on HIGH => should ventilation be running ?
static bool isVentilationOnInV1 = true;

//==================================================================================================
//**************************************************************************************************
void actions_Setup() {
#ifdef DEBUG
  Serial.println("ACTIONS:Setup >>>");
#endif
  __SetupRelays();
  //
  __SetupFunctionsForActions();
  //
  actions_SetStateToDefault();
  //
#ifdef DEBUG
  debug_PrintActions();
  Serial.println(">>> ACTIONS:Setup");
#endif
}
//**************************************************************************************************
void __SetupFunctionsForActions() {
  NoAction.function = __NoAction;
  //
  Action1.function = __VentilationSpeed1;
  Action2.function = __VentilationSpeed2;
  Action3.function = __VentilationSpeed3;
  Action4.function = __ACTION4;
  //
  ActionVentOff.function = __VentilationOff;
  ActionVentOn.function = __VentilationOn;
}
//**************************************************************************************************
void __SetupRelays() {
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);
  //
  digitalWrite(RELAY_1_PIN, HIGH);  // V1 is ON in NC mode, so ON = HIGH (so it's active even when arduino is removed)
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  digitalWrite(RELAY_4_PIN, HIGH);
}
//**************************************************************************************************
void actions_SetStateToDefault() {
  __VentilationOn();
}
//**************************************************************************************************
//==================================================================================================
const Action *actions_ComputeActionForButton(unsigned long buttonId) {
  for (byte act = 0; act < ARRAY_LEN(Actions); act++) {
    const Action *action = Actions[act];
    for (int i = 0; i < ARRAY_LEN(action->buttons); i++) {
      if (action->buttons[i] == buttonId) {
        return action;
      }
    }
  }
  return &NoAction;
}
//==================================================================================================
void actions_ProcessAction(const Action *action) {
  if (action == &NoAction) {
    return;
  }
  //
  previousAction = action;
  //
  display_Print1stLine(action);
  action->function();
}
//==================================================================================================
//==================================================================================================
void __NoAction() {
  // Do Nothing - Unmapped Action - BUG
}
//==================================================================================================
void __VentilationSpeed1() {
  digitalWrite(RELAY_1_PIN, isVentilationOnInV1 ? HIGH : LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
}
//==================================================================================================
void __VentilationSpeed2() {
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
}
//==================================================================================================
void __VentilationSpeed3() {
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, LOW);
}
//==================================================================================================
void __ACTION4() {
  __VentilationSpeed3();
  clock_Alarm1_SetInMinutesWithAction(ACTION4_DELAY, &Action1);
}
//==================================================================================================
void __VentilationOff() {
  digitalWrite(RELAY_1_PIN, LOW);  // V1 is reversed, so LOW is OFF
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
}
//==================================================================================================
void __VentilationOn() {  // FLIP from 2 to 1, to go into low speed mode
  __VentilationSpeed2();
  delay(TIME_TICK * 50);
  actions_ProcessAction(&Action1);
}
//==================================================================================================


//##################################################################################################
void actions_onSchedulerStatusChange(bool newValue) {
  if (isVentilationOnInV1 == newValue) {
    return;
  }
  isVentilationOnInV1 = newValue;

  if (isVentilationOnInV1) {
    if ((previousAction == &ActionVentOff) || (previousAction == &Action1)) {
      actions_ProcessAction(&ActionVentOn);
    }
  } else {
    if (previousAction == &Action1) {
      actions_ProcessAction(&ActionVentOff);
    }
  }
}
//##################################################################################################
//==================================================================================================
#ifdef DEBUG
void debug_PrintActions() {
  char buffer[200];
  sprintf(buffer, "|%d|%d|%d|%d|",
          Remote1.button1, Remote1.button2, Remote1.button3, Remote1.button4);
  Serial.println(buffer);
  Serial.println("--------------------------");
  for (byte i = 0; i < ARRAY_LEN(Actions); i = i + 1) {
    __PrintAction(Actions[i]);
  }
  Serial.println("--------------------------");
}

void __PrintAction(const Action *action) {
  char buffer[200];
  sprintf(buffer, "Action {name=\"%s\", actionCode=%d, buttons=[ ",
          action->name, action->actionCode);
  Serial.print(buffer);

  for (int i = 0; i < ARRAY_LEN(action->buttons); i++) {
    if (i) {
      Serial.print(", ");
    }
    Serial.print(action->buttons[i]);
  }
  Serial.println(" ]}");
}
#endif
//==================================================================================================