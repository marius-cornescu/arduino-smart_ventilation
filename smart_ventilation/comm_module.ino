//= DEFINES ========================================================================================
#define COMM_SKIP_GET_SPEED
#define COMM_SKIP_GET_LABEL

//= INCLUDES =======================================================================================
#include "CommCommon.h"
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================


//= VARIABLES ======================================================================================
SerialTransfer commProto;

//==================================================================================================
//**************************************************************************************************
void comm_Setup() {
#ifdef UseCOMM
#ifdef DEBUG
  Serial.println("COMM:Setup >>>");
#endif
  //..............................
  // Open serial communications and wait for port to open
  Serial.begin(115200);
  while (!Serial) { ; }
  commProto.begin(Serial);
  //..............................
#ifdef DEBUG
  Serial.println("COMM:Setup <<<");
#endif
  delay(50 * TIME_TICK);
#endif
}
//**************************************************************************************************
//==================================================================================================
void comm_ActOnNewDataToSend() {
#ifdef UseCOMM
  memset(payload, '|', PAYLOAD_SIZE);  // all 'zero' character
  payload[PAYLOAD_SIZE - 1] = '\0';    // end with array terminator
  //
  _Data_To_Payload();
  //
  __printPayloadIfDebug();
  //
  commProto.sendDatum(payload);
#endif
}
//==================================================================================================
void _Data_To_Payload() {
#ifndef COMM_SKIP_SEND_SPEED
  // add speed
  char currentSpeedString[SPEED_PAYLOAD_SIZE + 1];
  sprintf(currentSpeedString, "%01d", currentVentSpeed);
  memcpy(&payload[PAYLOAD_SPEED_START], currentSpeedString, SPEED_PAYLOAD_SIZE);
#endif

#ifndef COMM_SKIP_SEND_ACTION
  // add actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  sprintf(currentActionCodeString, "%02d", previousAction->actionCode);
  memcpy(&payload[PAYLOAD_ACTION_START], currentActionCodeString, ACTION_PAYLOAD_SIZE);
#endif

#ifndef COMM_SKIP_SEND_LABEL
  // add actionCodeLabel
  memcpy(&payload[PAYLOAD_LABEL_START], previousAction->description, LABEL_PAYLOAD_SIZE);
#endif
}
//==================================================================================================
//==================================================================================================
bool comm_ActIfReceivedMessage() {
  bool hasUpdate = false;
#ifdef UseCOMM
  if (commProto.available()) {
    memset(payload, '0', PAYLOAD_SIZE);  // all 'zero' character
    payload[PAYLOAD_SIZE - 1] = '\0';    // end with array terminator
    //
    commProto.rxObj(payload);
    //
    __printPayloadIfDebug();
    //
    _Payload_To_Data();
    //
    __printDataIfDebug();
    //
    hasUpdate = true;
  }
#endif
  return hasUpdate;
}
//==================================================================================================
void _Payload_To_Data() {
#ifndef COMM_SKIP_GET_SPEED
  // add speed
  char currentSpeedString[SPEED_PAYLOAD_SIZE + 1];
  memcpy(currentSpeedString, &payload[PAYLOAD_SPEED_START], SPEED_PAYLOAD_SIZE);
  currentVentSpeed = atoi(currentSpeedString);
#endif

#ifndef COMM_SKIP_GET_ACTION
  // add actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  memcpy(currentActionCodeString, &payload[PAYLOAD_ACTION_START], ACTION_PAYLOAD_SIZE);
  currentActionCode = atoi(currentActionCodeString);
#endif

#ifndef COMM_SKIP_GET_LABEL
  // add actionCodeLabel
  memcpy(currentActionLabel, &payload[PAYLOAD_LABEL_START], LABEL_PAYLOAD_SIZE);
#endif
}
//==================================================================================================
void __printPayloadIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print("payload = [");
  Serial.print(payload);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================
void __printDataIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print("currentActionCode = [");
  Serial.print(currentActionCode);
  Serial.print("] currentActionLabel = [");
  Serial.print(currentActionLabel);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================