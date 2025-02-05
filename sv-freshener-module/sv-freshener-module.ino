/*
  PIN CONNECTIONS
  -------------------------------
  GPIO0 -> DIGITAL SIGNAL
  GPIO1 -> DIGITAL SIGNAL
  GPIO2 -> DIGITAL SIGNAL

  GND  -> GND
  VCC  -> 3.3V
  CHPD -> 3.3V
  -------------------------------
  -------------------------------
*/

//= DEFINES ========================================================================================
//
//#define DEBUG

#define ESP8266_GPIO0 0        // HIGH is 3.3V
#define ESP8266_GPIO1 1        // HIGH is 3.3V
#define ESP8266_GPIO2 2        // HIGH is 3.3V
#define ESP8266_GPIO3 3  // RX
// Built-In LED: LOW is on and HIGH is off for blue led
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PUBLISH_TOPIC "home/ventilation/unit-A/status"
#define SUBSCRIBE_TOPIC "home/ventilation/unit-A/command/+"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define DEBUG

//= INCLUDES =======================================================================================
#include "Common.h"
#include "Secrets.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

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

//------------------------------------------------
const char host_name[] = HOST_NAME;

// WiFi credentials (password to "" for open networks)
const char ssid[] = WIFI_SSID;
const char pass[] = WIFI_PASSWORD;

//= VARIABLES ======================================================================================
WiFiClient espClient;

volatile int iteration = 0;


//==================================================================================================
//**************************************************************************************************
void setup() {
#ifdef DEBUG
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) { ; }
#endif
  debugPrintln(F("START-UP >>>>>>>>>>>>>>>"));
  //..............................
  //
  wifi_Setup();
  //
  pins_Setup();
  //
  mqtt_Setup();
  //
  delay(1 * TIME_TICK);
  //..............................
  debugPrintln(F("START-UP <<<<<<<<<<<<<<<"));
}
//**************************************************************************************************
void wifi_Setup() {
  delay(TIME_TICK);
  WiFi.hostname(host_name);
  WiFi.setOutputPower(0);  // Sets WiFi RF power output to lowest level, lowest RF power usage
  // We start by connecting to a WiFi network
  debugPrintln(F(""));
  debugPrint(F("Connecting to "));
  debugPrint(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50 * TIME_TICK);
    debugPrint(F("."));
  }

  debugPrintln(F(""));
  debugPrint(F("WiFi connected | "));
  debugPrint(F("IP address: "));
  debugPrintln(WiFi.localIP());
}
//**************************************************************************************************
void pins_Setup() {
  pinMode(ESP8266_GPIO0, OUTPUT);
  pinMode(ESP8266_GPIO1, OUTPUT);
  pinMode(ESP8266_GPIO2, OUTPUT);
  //
  digitalWrite(ESP8266_GPIO0, LOW); // LED_OFF: FIXME
  digitalWrite(ESP8266_GPIO1, LOW); // LED_OFF: FIXME
  digitalWrite(ESP8266_GPIO2, LOW); // LED_OFF: FIXME
}
//**************************************************************************************************
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
void loop() {
  mqtt_MaintainConnection();
  //
  if (mqtt_ShouldPublish()) {
    // publishVoltageDataToMqtt(); // FIXME
    // publishStatusDataToMqtt(); // FIXME
  }
  //
  test_LEDs();
  //
  delay(1 * SEC);
}
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//==================================================================================================
void test_LEDs() {
  //
  digitalWrite(ESP8266_GPIO0, HIGH); // LED_ON
  delay(10 * SEC);
  //
  digitalWrite(ESP8266_GPIO1, HIGH); // LED_ON
  delay(10 * SEC);
  //
  digitalWrite(ESP8266_GPIO2, HIGH); // LED_ON
  delay(10 * SEC);
  //
  digitalWrite(ESP8266_GPIO0, LOW); // LED_OFF
  digitalWrite(ESP8266_GPIO1, LOW); // LED_OFF
  digitalWrite(ESP8266_GPIO2, LOW); // LED_OFF
  delay(5 * SEC);
}

//==================================================================================================
void spray_old_config() {
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
