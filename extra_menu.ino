
//= DEFINES ========================================================================================
//= INCLUDE ========================================================================================
//= CONSTANTS ======================================================================================
const byte LED_RED_1 = P0;     // 

#define BUTTONS_I2C_ADDRESS 0x20

const byte ON = LOW;
const byte OFF = HIGH;

//----------------------------------
const byte BTN_NONE = 0;
//----------------------------------
const byte BTN_VENTILATION_1 = 1;
const byte BTN_VENTILATION_2 = 2;
const byte BTN_VENTILATION_3 = 3;
//----------------------------------
const byte BTN_UP = 4;
const byte BTN_DOWN = 5;
const byte BTN_BACK = 6;
const byte BTN_FWARD = 7;
const byte BTN_ENTER = 8;
//----------------------------------

//= VARIABLES ======================================================================================
// adjust addresses if needed
PCF8575 buttons(BUTTONS_I2C_ADDRESS);  // add switches to lines  (used as input)


//**************************************************************************************************
void menu_Setup() {
#ifdef DEBUG
  Serial.println("MENU:Setup >>>");
#endif
  // Set pinMode
  // Side A
  buttons.pinMode(P0, OUTPUT);
  buttons.pinMode(P1, OUTPUT);
  buttons.pinMode(P2, OUTPUT);
  buttons.pinMode(P3, OUTPUT);
  buttons.pinMode(P4, OUTPUT);
  buttons.pinMode(P5, OUTPUT);
  buttons.pinMode(P6, OUTPUT);
  buttons.pinMode(P7, OUTPUT);
  // Side B - BUTTONS
  buttons.pinMode(P8, INPUT);
  buttons.pinMode(P9, INPUT);
  buttons.pinMode(P10, INPUT);
  buttons.pinMode(P11, INPUT);
  buttons.pinMode(P12, INPUT);
  buttons.pinMode(P13, INPUT);
  buttons.pinMode(P14, INPUT);
  buttons.pinMode(P15, INPUT);
  //..............................
  buttons.begin();
  //..............................
  buttons.digitalWrite(LED_RED_1, OFF);
  //..............................
  delay(TIME_TICK * 10);
#ifdef DEBUG
  Serial.println(">>> MENU:Setup");
#endif
}
//**************************************************************************************************
//==================================================================================================
void menu_ActIfActivity() {
  byte button = get_button(buttons);

  if(button == BTN_ENTER) {
    digitalWrite(LED_INDICATOR_PIN, HIGH);
    //
    display_Print2ndLine("<MANUAL>");
    display_ShowProgress();
    //
    menu_print();
    menu_process_loop(buttons);
    //
    display_Print2ndLine("");
    digitalWrite(LED_INDICATOR_PIN, LOW);
  }
}
//==================================================================================================
byte get_button(PCF8575 &io) {
  byte response = BTN_NONE;
  PCF8575::DigitalInput io_vals = io.digitalReadAll();

  if(io_vals.p8 == 1) {
    response = BTN_VENTILATION_1;
  } else if(io_vals.p9 == 1) {
    response = BTN_VENTILATION_2;
  } else if(io_vals.p10 == 1) {
    response = BTN_VENTILATION_3;
  } else if(io_vals.p11 == 1) {
    response = BTN_UP;
  } else if(io_vals.p12 == 1) {
    response = BTN_DOWN;
  } else if(io_vals.p13 == 1) {
    response = BTN_BACK;
  } else if(io_vals.p14 == 1) {
    response = BTN_FWARD;
  } else if(io_vals.p15 == 1) {
    response = BTN_ENTER;
  } else {
    response = BTN_NONE;
  }

#ifdef I2CLogsToSerial
  char buffer[200];

  sprintf(buffer, "|0=%d|1=%d|2=%d|3=%d|4=%d|5=%d|6=%d|7=%d||||8=%d|9=%d|10=%d|11=%d|12=%d|13=%d|14=%d|15=%d|", io_vals.p0, io_vals.p1, io_vals.p2, io_vals.p3, io_vals.p4, io_vals.p5, io_vals.p6, io_vals.p7, io_vals.p8, io_vals.p9, io_vals.p10, io_vals.p11, io_vals.p12, io_vals.p13, io_vals.p14, io_vals.p15);
  //sprintf(buffer, "|8=%d|9=%d|10=%d|11=%d|12=%d|13=%d|14=%d|15=%d| => %d", io_vals.p8, io_vals.p9, io_vals.p10, io_vals.p11, io_vals.p12, io_vals.p13, io_vals.p14, io_vals.p15, response);
  Serial.println(buffer);
#endif

  return response;
}
//==================================================================================================
void menu_print() {

}
//==================================================================================================
void menu_process_loop(PCF8575 &io) {
  byte prevs_button = BTN_NONE;
  byte button = get_button(io);

  while(button != BTN_BACK) {
    //
    if (button != BTN_NONE) {
      io.digitalWrite(LED_RED_1, ON);// indication of activity
      menu_process_button_action(button, prevs_button);
    }
    //
    delay(TIME_TICK);
    io.digitalWrite(LED_RED_1, OFF);// indication of activity
    prevs_button = button;
    button = get_button(io);
  }

  io.digitalWrite(LED_RED_1, OFF);
}
//==================================================================================================
void menu_process_button_action(byte &button, byte &prevs_button) {
  if(button == prevs_button) {
    return; // ignore phake multi-press
  }

  #ifdef I2CLogsToSerial
    char buffer[200];
    sprintf(buffer, "|button=%d|prevs_button=%d|", button, prevs_button);
    Serial.println(buffer);
  #endif

  switch (button) {
    case BTN_VENTILATION_1:
      actions_ProcessAction(&Action1);
      break;
    case BTN_VENTILATION_2:
      actions_ProcessAction(&Action2);
      break;
    case BTN_VENTILATION_3:
      actions_ProcessAction(&Action3);
      break;
    case BTN_UP:
      // statements
      break;
    case BTN_DOWN:
      // statements
      break;
    case BTN_BACK:
      // statements
      break;
    case BTN_FWARD:
      // statements
      break;
    case BTN_ENTER:
      // statements
      break;
    default:
      // statements
      break;
  }
}
//==================================================================================================
