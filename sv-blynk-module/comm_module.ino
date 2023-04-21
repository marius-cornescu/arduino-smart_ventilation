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
  commProto.actOnPollMessage();
#endif
}
//==================================================================================================
//==================================================================================================
bool comm_ActIfReceivedMessage() {
#ifdef UseCOMMPro
  if (commProto.hasMessageInInboxThenAct() && commProto.isHaveToPublish()) {
    return true;
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
  byte newVentSpeed = message[0] - (byte)'0';
  // ignore - the meat is in the action
  //------------------------------------
  byte newActionCode = message[1] - (byte)'0';
  if (newActionCode != ACTION_NOP) {
    const Action* newAction = getActionByActionCode(newActionCode);

    currentVentSpeed = ventSpeedFromActionCode(newAction);
    currentActionCode = newActionCode;
    currentActionLabel = newAction->description;

    haveToPublish = true;
  }
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
  message[0] = currentVentSpeed + (byte)'0';
  message[1] = currentActionCode + (byte)'0';
}
//==================================================================================================
#endif
//==================================================================================================