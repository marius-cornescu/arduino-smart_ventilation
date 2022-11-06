//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Common.h"
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
const char* prepareMessageToSend();

//= VARIABLES ======================================================================================
RtznCommProtocol commProto("OFFLINE-WORKER", &processReceivedMessage, &prepareMessageToSend);

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
  if (commProto.hasMessageInInboxThenReadMessage()) {
    commProto.actOnMessage();
    if (commProto.isHaveToPublish()) {
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
  if (previousAction == &Action3 || previousAction == &Action4) {
    currentVentSpeed = 3;
  }

  commProto.actOnPollMessage();
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
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
  //------------------------------------
  byte value1 = message[0] - '0';
  if (currentVentSpeed != value1) {
    haveToPublish = true;

    if (value1 == 0) {
      currentActionCode = ActionVentOff.actionCode;
    }
    if (value1 == 1) {
      currentActionCode = Action1.actionCode;
    }
    if (value1 == 2) {
      currentActionCode = Action2.actionCode;
    }
    if (value1 == 3) {
      currentActionCode = Action3.actionCode;
    }
  }
  //------------------------------------
  byte actionCode = message[1] - '0';
  if (currentActionCode != actionCode) {
    if (actionCode > 0) {
      currentActionCode = actionCode;
      haveToPublish = true;
    }
  }
  //------------------------------------
  return haveToPublish;
}

const char* prepareMessageToSend() {
  char* message = new char[2];
  sprintf(message, "%1u%1u", currentVentSpeed, currentActionCode);
  return message;
}
//==================================================================================================
#endif
//==================================================================================================