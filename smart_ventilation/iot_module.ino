//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "Common.h"
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================

//= VARIABLES ======================================================================================
SerialTransfer commProto;

byte currentVentSpeed = 0;
byte currentActionCode = ACTION_NOP;

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
  commProto.begin(Serial);
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
void iot_actOnNewAction(const Action* action) {
#ifdef UseIoT
  currentActionCode = action->actionCode;

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
  byte newVentSpeed = message[0] - (byte)'0';
  // ignore - the meat is in the action
  //------------------------------------
  byte newActionCode = message[1] - (byte)'0';
  if (currentActionCode != newActionCode) {
    if (newActionCode > ACTION_NOP) {
      currentActionCode = newActionCode;
      haveToPublish = true;
    }
  }
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* payload) {
  payload[0] = currentVentSpeed + (byte)'0';
  payload[1] = currentActionCode + (byte)'0';
}
//==================================================================================================
#endif
//==================================================================================================