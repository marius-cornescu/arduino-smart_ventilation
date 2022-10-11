//= INCLUDES =======================================================================================
#include "Actions.h"

//= CONSTANTS ======================================================================================
#define ACTION4_DELAY 5

const Action *Actions[] = { &Action1,  &Action2, &Action3, &Action4 };

//= VARIABLES ======================================================================================


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
}
//**************************************************************************************************
void __SetupRelays() {
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);
  //
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  digitalWrite(RELAY_4_PIN, HIGH);
}
//**************************************************************************************************
void actions_SetStateToDefault() {
  actions_ProcessAction(&Action2);
  delay(TIME_TICK * 50);
  actions_ProcessAction(&Action1);
  //
}
//**************************************************************************************************
//==================================================================================================
Action* actions_ComputeActionForButton(unsigned long buttonId) {
  for (byte act = 0; act < ARRAY_LEN(Actions); act++) {
    Action *action = Actions[act];
    for (int i = 0; i < ARRAY_LEN(action->buttons); i++) {
      if (action->buttons[i] == buttonId) {
        return action;
      }
    }
  }
  return &NoAction;
}
//==================================================================================================
void actions_ProcessAction(struct Action *action) {
  if (action == &NoAction) {
    return;
  }
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
  digitalWrite(RELAY_1_PIN, LOW);
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
    debug_PrintAction(Actions[i]);
  }
  Serial.println("--------------------------");
}

void debug_PrintAction(struct Action *action) {
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
