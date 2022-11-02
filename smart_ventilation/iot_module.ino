//= DEFINES ========================================================================================
#define COMM_ROLE "OFFLINE-WORKER"

//= INCLUDE ========================================================================================
#include "Common.h"
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------

//= VARIABLES ======================================================================================
byte currentVentSpeed = 0;
byte currentActionCode = 0;

//==================================================================================================
//**************************************************************************************************
void iot_Setup() {
#ifdef UseIoT
#ifdef DEBUG
  Serial.println("IOT:Setup >>>");
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  //..............................
#ifdef DEBUG
  Serial.println(">>> IOT:Setup");
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void iot_ActIfActivity() {
#ifdef UseIoT
  if (hasMessageInInboxThenReadMessage()) {
    commActOnMessage();
    if (haveToPublish) {
      __actOnPartnerDataChanged();
    }
  }
#endif
}
//==================================================================================================
void iot_actOnNewAction() {
#ifdef UseIoT
  currentActionCode = previousAction->actionCode;

  if (previousAction == &ActionVentOff) {
    currentVentSpeed = 0;
  }
  if (previousAction == &Action1) {
    currentVentSpeed = 1;
  }
  if (previousAction == &Action2) {
    currentVentSpeed = 2;
  }
  if (previousAction == &Action3) {
    currentVentSpeed = 3;
  }

  commActOnPollMessage();
#endif
}
//==================================================================================================
#ifdef UseIoT
//==================================================================================================
void __actOnPartnerDataChanged() {
  const Action* currentAction = actions_ComputeActionForCode(currentActionCode);
  if (actions_ShouldProcessAction(currentAction)) {
    actions_ProcessAction(currentAction);
    display_ShowProgress();
  }
}
//==================================================================================================
//==================================================================================================
byte getValue1() {
  return currentVentSpeed;
}
void setValue1(byte ventSpeed) {
  if (ventSpeed == 0) {
    currentActionCode = ActionVentOff.actionCode;
  }
  if (ventSpeed == 1) {
    currentActionCode = Action1.actionCode;
  }
  if (ventSpeed == 2) {
    currentActionCode = Action2.actionCode;
  }
  if (ventSpeed == 3) {
    currentActionCode = Action3.actionCode;
  }
}
//----------------------------------
byte getValue2() {
  return currentActionCode;
}
void setValue2(byte actionCode) {
  if (actionCode > 0) {
    currentActionCode = actionCode;
  }
}
//==================================================================================================
#endif
//==================================================================================================