//= DEFINES ========================================================================================
#define COMM_ROLE "OFFLINE-WORKER"

//= INCLUDE ========================================================================================
#include "Common.h"
#include "Artizan-CommProtocol.h"

//= CONSTANTS ======================================================================================
//----------------------------------

//= VARIABLES ======================================================================================

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
  if (actOnAction()) {
    actOnDataChanged();
    // TODO: act on action or trigger ventilation
    commActOnPollMessage();
  }
  //
  if (hasMessageInInboxThenReadMessage()) {
    commActOnMessage();
    if (haveToPublish) {
      actOnDataChanged();
    }
  }
#endif
}
//==================================================================================================
#ifdef UseIoT
//==================================================================================================
bool actOnAction() {
  byte newVentSpeed = currentVentSpeed;
  if (isV1ButtonPressed()) {
    newVentSpeed = 1;
  }
  if (isV2ButtonPressed()) {
    newVentSpeed = 2;
  }
  if (isV3ButtonPressed()) {
    newVentSpeed = 3;
  }

  if (newVentSpeed != currentVentSpeed) {
    currentVentSpeed = newVentSpeed;
    return true;
  }
  return false;
}
//==================================================================================================
void actOnDataChanged() {
  turnAllLedsOff();
  turnVLedsOn();
}
//==================================================================================================
void turnAllLedsOff() {
  digitalWrite(PIN_LED_V1, LOW);
  digitalWrite(PIN_LED_V2, LOW);
  digitalWrite(PIN_LED_V3, LOW);
}
//==================================================================================================
void turnVLedsOn() {
  digitalWrite(13 - currentVentSpeed, HIGH);
}
//==================================================================================================
bool isV1ButtonPressed() {
  byte currentBtnState = digitalRead(PIN_BTN_V1);
  if (currentBtnState != lastV1BtnState) {
    lastV1BtnState = currentBtnState;
    if (currentBtnState == HIGH) {
#ifdef DEBUG
      Serial.println("V1");
#endif
      return true;
    }
  }
  return false;
}
//==================================================================================================
bool isV2ButtonPressed() {
  byte currentBtnState = digitalRead(PIN_BTN_V2);
  if (currentBtnState != lastV2BtnState) {
    lastV2BtnState = currentBtnState;
    if (currentBtnState == HIGH) {
#ifdef DEBUG
      Serial.println("V2");
#endif
      return true;
    }
  }
  return false;
}
//==================================================================================================
bool isV3ButtonPressed() {
  byte currentBtnState = digitalRead(PIN_BTN_V3);
  if (currentBtnState != lastV3BtnState) {
    lastV3BtnState = currentBtnState;
    if (currentBtnState == HIGH) {
#ifdef DEBUG
      Serial.println("V3");
#endif
      return true;
    }
  }
  return false;
}
//==================================================================================================


//==================================================================================================
//==================================================================================================
byte getValue1() {
  return currentVentSpeed;
}
void setValue1(byte value1) {
  currentVentSpeed = value1;
}
byte getValue2() {
  return currentActionCode;
}
void setValue2(byte value2) {
  currentActionCode = value2;
}
//==================================================================================================
#endif
//==================================================================================================