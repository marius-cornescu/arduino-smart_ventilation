//= DEFINES ========================================================================================

//= INCLUDES =======================================================================================
#include "CommCommon.h"
#include "SerialTransfer.h"

//= CONSTANTS ======================================================================================


//= VARIABLES ======================================================================================
SerialTransfer commProto;

byte currentActionCode = 1;
char currentActionLabel[LABEL_PAYLOAD_SIZE + 1] = "VentS3(*5m) -> VentS1";

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
  _printPayloadIfDebug();
  //
  commProto.sendDatum(payload);
#endif
}
//==================================================================================================
void _Data_To_Payload() {
  // add actionCode
  char currentActionCodeString[ACTION_PAYLOAD_SIZE + 1];
  sprintf(currentActionCodeString, "%02d", currentActionCode);
  memcpy(&payload[PAYLOAD_ACTION_START], currentActionCodeString, ACTION_PAYLOAD_SIZE);
  // add actionCodeLabel
  memcpy(&payload[PAYLOAD_LABEL_START], currentActionLabel, LABEL_PAYLOAD_SIZE);
}
//==================================================================================================
void _printPayloadIfDebug() {
#ifdef DEBUG
  Serial.println();
  Serial.print("payload = [");
  Serial.print(payload);
  Serial.print("]");
  Serial.println();
#endif
}
//==================================================================================================
void _printDataIfDebug() {
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