/*
  PIN CONNECTIONS
  -------------------------------
  -------------------------------
  -------------------------------

*/

//= DEFINES ========================================================================================
#define ESP8266_BUILTIN_LED 1 // LOW is on and HIGH is off for blue led
#define ESP8266_DPIN 3

//= INCLUDES =======================================================================================



//= CONSTANTS ======================================================================================
const int LED_INDICATOR_PIN = 2;  // choose the pin for the LED
const int D_PIN = 0;


//= VARIABLES ======================================================================================
volatile int ledState = HIGH;


//==================================================================================================
//**************************************************************************************************
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // initialize digital pin LED_INDICATOR_PIN as an output.
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  //
  pinMode(D_PIN, OUTPUT);
}
//**************************************************************************************************
//==================================================================================================
void loop() {
  ledState = HIGH;
  //digitalWrite(LED_INDICATOR_PIN, ledState);
  digitalWrite(D_PIN, ledState);
  delay(1000);

  ledState = LOW;
  //digitalWrite(LED_INDICATOR_PIN, ledState);
  digitalWrite(D_PIN, ledState);
  delay(4000);
}
//==================================================================================================
