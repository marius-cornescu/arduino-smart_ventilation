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
    commProto.setHaveToPublish(false);
    return true;
  }
#endif
  return false;
}
//==================================================================================================
void comm_SendPollMessage() {
#ifdef UseCOMMPro
  commProto.sendPollMessage();
#endif
}
//==================================================================================================
#ifdef UseCOMMPro
//==================================================================================================
//==================================================================================================
bool processReceivedMessage(const char* message) {
  bool haveToPublish = false;
  //------------------------------------
  // get speed
  char currentSpeedString[SPEED_PAYLOAD_SIZE + 1];
  memcpy(currentSpeedString, &message[PAYLOAD_SPEED_START], SPEED_PAYLOAD_SIZE);
  currentSpeedString[SPEED_PAYLOAD_SIZE] = '\0';
  byte newVentSpeed = (byte)atoi(currentSpeedString);
  //------------------------------------
  // get actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  memcpy(currentActionCodeString, &message[PAYLOAD_ACTION_START], ACTION_PAYLOAD_SIZE);
  byte newActionCode = (byte)atoi(currentActionCodeString);

  if (newActionCode != ACTION_NOP && newActionCode != currentActionCode) {
    currentVentSpeed = newVentSpeed;
    currentActionCode = newActionCode;

    haveToPublish = true;
  }
  //------------------------------------
  // get actionCodeLabel
  memset(currentActionLabel, 0, LABEL_PAYLOAD_SIZE);
  memcpy(currentActionLabel, &message[PAYLOAD_LABEL_START], LABEL_PAYLOAD_SIZE);
  //------------------------------------
  return haveToPublish;
}
//==================================================================================================
void prepareMessageToSend(char* message) {
  // add actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  sprintf(currentActionCodeString, "%02d", currentActionCode);
  memcpy(&message[PAYLOAD_ACTION_START], currentActionCodeString, ACTION_PAYLOAD_SIZE);
}
//==================================================================================================
#endif
//==================================================================================================