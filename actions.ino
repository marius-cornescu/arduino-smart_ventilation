//= INCLUDES =======================================================================================
#include "Actions.h"

//= CONSTANTS ======================================================================================
#define ACTION4_DELAY 5

//= VARIABLES ======================================================================================

//**************************************************************************************************
void actions_Setup() {
  actions_SetupInitialState();
  //
#ifdef DEBUG
  debug_PrintActions();
#endif
}
//**************************************************************************************************
//==================================================================================================
void actions_SetupInitialState() {
  //
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);
  //
  actions_SetStateToDefault();
}
//==================================================================================================
void actions_SetStateToDefault() {
  //
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  digitalWrite(RELAY_4_PIN, HIGH);
}
//==================================================================================================
byte actions_ComputeActionForButton(unsigned long buttonId) {
  for (byte act = 0; act < ARRAY_LEN(Actions); act = act + 1) {
    Action *action = Actions[act];
    for (int i = 0; i < action->buttonCnt; i++) {
      if (action->buttons[i] == buttonId) {
        return action->actionCode;
      }
    }
  }

  return ACTION_NOP;
}
//==================================================================================================
#ifdef DEBUG
void debug_PrintActions() {
  //Serial.print("|"); Serial.print(Remote1.button1); Serial.print("|"); Serial.print(Remote1.button2); Serial.print("|"); Serial.print(Remote1.button3); Serial.print("|");Serial.print(Remote1.button4);Serial.print("|"); Serial.println();

  Serial.println("--------------------------");
  for (byte i = 0; i < ARRAY_LEN(Actions); i = i + 1) {
    debug_PrintAction(Actions[i]);
  }
  Serial.println("--------------------------");

  //Serial.print("|"); Serial.print(Action1.buttons[0]); Serial.print("|"); Serial.print(Action1.buttons[1]); Serial.print("|"); Serial.print(Action1.buttons[2]); Serial.print("|"); Serial.println();
}

void debug_PrintAction(const struct Action *action) {
  char buffer[200];
  sprintf(buffer, "Action {name=\"%s\", actionCode=%d, buttonCnt=%d, buttons=[ ",
          action->name, action->actionCode, action->buttonCnt);
  Serial.print(buffer);

  for (int i = 0; i < action->buttonCnt; i++) {
    if (i) {
      Serial.print(", ");
    }
    Serial.print(action->buttons[i]);

  }
  Serial.println(" ]}");
}
#endif
//==================================================================================================
void actions_ProcessAction(const struct Action *action) {
  actions_ProcessAction(action->actionCode);
  display_Print1stLine(action->name);
}
//==================================================================================================
void actions_ProcessAction(byte currentAction) {
  switch (currentAction) {
    //-------------------------------------------
    case ACTION_1:
      actions_ACTION1();
      break;
    //-------------------------------------------
    case ACTION_2:
      actions_ACTION2();
      break;
    //-------------------------------------------
    case ACTION_3:
      actions_ACTION3();
      break;
    //-------------------------------------------
    case ACTION_4:
      actions_ACTION4();
      break;
    //-------------------------------------------
    default:
      actions_DEFAULT();
      break;
  }
}
//==================================================================================================
void actions_DEFAULT() {
  // Do Nothing - Unmapped Action - BUG
}
//==================================================================================================
void actions_ACTION1() {
  //
  digitalWrite(RELAY_1_PIN, LOW);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, HIGH);
  //
  display_Print1stLine("Vent", 1);
}
//==================================================================================================
void actions_ACTION2() {
  //
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, LOW);
  digitalWrite(RELAY_3_PIN, HIGH);
  //
  display_Print1stLine("Vent", 2);
}
//==================================================================================================
void actions_ACTION3() {
  //
  digitalWrite(RELAY_1_PIN, HIGH);
  digitalWrite(RELAY_2_PIN, HIGH);
  digitalWrite(RELAY_3_PIN, LOW);
  //
  display_Print1stLine("Vent", 3);
}
//==================================================================================================
void actions_ACTION4() {
  //
  actions_ACTION3();
  clock_Alarm1_SetInMinutesWithAction(ACTION4_DELAY, ACTION_1);
  //
}
//==================================================================================================
