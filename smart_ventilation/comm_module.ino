//= DEFINES ========================================================================================
#ifdef UseCOMMPro

//= INCLUDES =======================================================================================
#include "Common.h"
#include "CommCommon.h"
#include "RtznCommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------
bool processReceivedMessage(const char* message);
void prepareMessageToSend(char* message);

//= VARIABLES ======================================================================================
RtznCommProtocol commProto(COMM_ROLE, PAYLOAD_SIZE, &processReceivedMessage, &prepareMessageToSend);

#endif
//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMMPro
#ifdef DEBUG
  Serial.println(F("COMM:Setup >>>"));
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  //..............................
  delay(1 * TIME_TICK);
#ifdef DEBUG
  Serial.println(F("COMM:Setup <<<"));
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMMPro
  currentActionCode = previousAction->actionCode;

  commProto.actOnPollMessage();
#endif
}
//==================================================================================================
//==================================================================================================
bool comm_ActIfReceivedMessage() {
#ifdef UseCOMMPro
  if (commProto.hasMessageInInboxThenAct() && commProto.isHaveToPublish()) {
    const Action* currentAction = actions_ComputeActionForCode(currentActionCode);
    if (actions_ShouldProcessAction(currentAction)) {
      actions_ProcessAction(currentAction);
      display_ShowProgress();
      //
      return true;
    }
  }
#endif
  return false;
}
//==================================================================================================
#ifdef UseCOMMPro
//==================================================================================================
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
  //------------------------------------
  // get actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  memcpy(currentActionCodeString, &message[PAYLOAD_ACTION_START], ACTION_PAYLOAD_SIZE);
  byte newActionCode = atoi(currentActionCodeString);
  
  if (newActionCode != ACTION_NOP && newActionCode != previousAction->actionCode) {
    currentActionCode = newActionCode;

    haveToPublish = true;
  }
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
  // add speed
  char currentSpeedString[SPEED_PAYLOAD_SIZE + 1];
  sprintf(currentSpeedString, "%1d", currentVentSpeed);
  memcpy(&message[PAYLOAD_SPEED_START], currentSpeedString, SPEED_PAYLOAD_SIZE);

  // add actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  sprintf(currentActionCodeString, "%02d", previousAction->actionCode);
  memcpy(&message[PAYLOAD_ACTION_START], currentActionCodeString, ACTION_PAYLOAD_SIZE);

  // add actionCodeLabel
  char currentActionLabelString[LABEL_PAYLOAD_SIZE + 1];
  sprintf(currentActionLabelString, "%s", previousAction->description);
  memcpy(&message[PAYLOAD_LABEL_START], currentActionLabelString, LABEL_PAYLOAD_SIZE);
}
//==================================================================================================
#endif
//==================================================================================================