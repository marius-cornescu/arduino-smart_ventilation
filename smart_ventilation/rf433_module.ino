//= DEFINES ========================================================================================
//= INCLUDE ========================================================================================
#include "Common.h"
#include <RCSwitch.h>

//= CONSTANTS ======================================================================================
//------------------------------------------------
const byte RF_INTERRUPT_D2_PIN = 0;  // RF Receiver on INT0 => pin D2
//------------------------------------------------
// Fast Check
const byte RF_TARGET_PROTOCOL = 1;
const byte RF_TARGET_BIT_COUNT = 24;
//------------------------------------------------
//= VARIABLES ======================================================================================
RCSwitch rfRx = RCSwitch();

//==================================================================================================
//**************************************************************************************************
void rf433_Setup() {
#ifdef DEBUG
  Serial.println(F("RF433:Setup >>>"));
#endif
  //..............................
  rfRx.enableReceive(RF_INTERRUPT_D2_PIN);
  //..............................
  delay(TIME_TICK * 1);
#ifdef DEBUG
  Serial.println(F(">>> RF433:Setup"));
#endif
}
//**************************************************************************************************
//==================================================================================================
bool rf433_ActIfActivity() {
  if (rfRx.available()) {
    digitalWrite(LED_INDICATOR_PIN, HIGH);
    unsigned long buttonId = rfRx.getReceivedValue();
    if (__isRemoteCodeValid_FastCheck(buttonId, rfRx.getReceivedProtocol(), rfRx.getReceivedBitlength(), rfRx.getReceivedDelay(), rfRx.getReceivedRawdata())) {

      const Action* currentAction = actions_ComputeActionForButton(buttonId);
      actions_OnAction(currentAction);
    }

    rfRx.resetAvailable();
    digitalWrite(LED_INDICATOR_PIN, LOW);
    //
    return true;
  }
  return false;
}
//==================================================================================================
bool __isRemoteCodeValid_FastCheck(unsigned long decimal, unsigned int protocol, unsigned int length, unsigned int delay, unsigned int* raw) {
#ifdef RfLogsToSerial
  __printRxToSerial(decimal, length, delay, raw, protocol);
#endif

  if ((protocol == RF_TARGET_PROTOCOL) && (length == RF_TARGET_BIT_COUNT)) {
    return true;
  } else {
#ifdef DEBUG
    Serial.println(F("???? Unknown RF code / wrong protocol or bit count"));
#endif
    return false;
  }
}
//==================================================================================================
#ifdef RfLogsToSerial
//==================================================================================================
void __printRxToSerial(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {
  const char* b = __dec2binWzerofill(decimal, length);

  char buffer[200];
  sprintf(buffer, "Decimal: %d (%dBit) Binary: %d Tri-State: %d PulseLength: %d microseconds Protocol: %d",
          decimal, length, b, __bin2tristate(b), delay, protocol);
  Serial.println(buffer);

  Serial.print(F("Raw data: "));
  for (unsigned int i = 0; i <= length * 2; i++) {
    Serial.print(raw[i]);
    Serial.print(",");
  }
  Serial.println();
  Serial.println();
}
//==================================================================================================
static const char* __bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos] != '\0' && bin[pos + 1] != '\0') {
    if (bin[pos] == '0' && bin[pos + 1] == '0') {
      returnValue[pos2] = '0';
    } else if (bin[pos] == '1' && bin[pos + 1] == '1') {
      returnValue[pos2] = '1';
    } else if (bin[pos] == '0' && bin[pos + 1] == '1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos + 2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}
//==================================================================================================
static char* __dec2binWzerofill(unsigned long decimal, unsigned int bitLength) {
  static char bin[64];
  unsigned int i = 0;

  while (decimal > 0) {
    bin[32 + i++] = ((decimal & 1) > 0) ? '1' : '0';
    decimal = decimal >> 1;
  }

  for (unsigned int j = 0; j < bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[31 + i - (j - (bitLength - i))];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';

  return bin;
}
//==================================================================================================
#endif
//==================================================================================================