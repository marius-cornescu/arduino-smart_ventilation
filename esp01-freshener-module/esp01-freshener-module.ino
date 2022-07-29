/*
  PIN CONNECTIONS
  -------------------------------
  GPIO2 -> MOSFET

  GND  -> GND
  VCC  -> 3.3V
  CHPD -> 3.3V
  -------------------------------
  -------------------------------
*/

//= DEFINES ========================================================================================
//#define DEBUG

#define ESP8266_BUILTIN_LED 1 // LOW is on and HIGH is off for blue led
#define ESP8266_GPIO0 0        // HIGH is 3.3V
#define ESP8266_GPIO2 2        // HIGH is 3.3V 

//= INCLUDES =======================================================================================



//= CONSTANTS ======================================================================================
const int D_PIN = ESP8266_GPIO2;
//
const unsigned long ON_CYCLE_DURATION = 17000; // 17 seconds (spray starts ar 15 seconds)
const unsigned long OFF_CYCLE_DURATION = 3000; // 3 seconds (spray is OFF)
//
const byte DELAYS_CNT = 5;
/*
 * 0:15''' -> first spray
 * 0:35''' -> II  (20''' + 0''' extra delay)
 * 3:00''' -> III (20''' + 2min25sec delay)
 * 6:00''' -> IV  (20''' + 2min40sec delay)
 * 9:00''' -> V   (20''' + 2min40sec delay)
 * every 10min (20''' + 9min40sec delay)
*/
const unsigned long DELAYS[] = {0, 185000, 160000, 160000, 580000};


//= VARIABLES ======================================================================================
volatile int iteration = 0;


//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("START-UP");
#endif
  //
  pinMode(D_PIN, OUTPUT);
}
//**************************************************************************************************
//==================================================================================================
void loop() {
  // Enable for 17sec (15"' to power on + 2"' to run)
  digitalWrite(D_PIN, HIGH);
  delay(17000); //-> DONE
  // Power down for 3sec to cool down
  digitalWrite(D_PIN, LOW);
  delay(3000);
  // 20 seconds is minimum
  unsigned long dynamic_delay = computeDynamicDelay();
  delay(dynamic_delay);
}
//==================================================================================================
unsigned long computeDynamicDelay() {
  unsigned long response = DELAYS[DELAYS_CNT - 1];
  if (iteration < DELAYS_CNT) {
    response = DELAYS[iteration];
  }
  iteration = iteration + 1;
  return response;
}
//==================================================================================================
