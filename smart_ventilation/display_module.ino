//= DEFINES ========================================================================================
#define DISPLAY_STATUS_POS 15
#define DISPLAY_I2C_ADDRESS 0x27

#define BLANK_LINE "                                        "
#define TEST_LINE  "***************#123456789012345678901234"

#define LINE1 0
#define LINE2 1

//= INCLUDE ========================================================================================
#include "Common.h"
#ifdef UseDisplay
#include <LiquidCrystal_I2C.h>

//= CONSTANTS ======================================================================================

//= VARIABLES ======================================================================================
LiquidCrystal_I2C lcd(DISPLAY_I2C_ADDRESS, 16, 2);
byte progress = 0;                  // used to indicate progress on display

#endif
//==================================================================================================
//**************************************************************************************************
void display_Setup() {
#ifdef UseDisplay
#ifdef DEBUG
  Serial.println(F("DISPLAY:Setup >>>"));
#endif
  //..............................
  lcd.init();      // initialise the LCD
  lcd.backlight(); // turn backlight on
  lcd.blink();     // cursor blinks
  //------------- TEST SCREEN -----------------------
  #ifdef DEBUG
  lcd.clear();         // clear the screen
  lcd.home();          // set the cursor to position 0, line 1
  lcd.print(TEST_LINE);
  lcd.setCursor(0, LINE2);
  lcd.print(TEST_LINE);
  lcd.home();
  delay(TIME_TICK * 50);
  #endif
  //-------------------
  lcd.clear();
  //-------------------------------------------------
  //lcd.noBacklight(); // turn backlight off
  lcd.noBlink();
  //..............................
#ifdef DEBUG
  Serial.println(F("DISPLAY:Setup <<<"));
#endif
#endif
}
//**************************************************************************************************
//==================================================================================================
void display_ShowProgress() {
#ifdef UseDisplay

  lcd.setCursor(DISPLAY_STATUS_POS, LINE1);        // set the cursor to position 15, line 1
  progress = progress + 1;
  if (progress == 1) {
    lcd.print("+");
  } else if (progress == 2) {
    lcd.print("-");
  } else if (progress == 3) {
    lcd.print("#");
  } else if (progress == 4) {
    lcd.print("|");
  } else {
    lcd.print("*");
    progress = 0;
  }

#endif
}
//==================================================================================================
void display_Print1stLine(const Action *action) {
  display_Print1stLine(action, action->name);
}
//==================================================================================================
void display_Print1stLine(const Action *action, const char* label) {
  char buffer[30];
  sprintf(buffer, "A%02d:%11s", action->actionCode, label);
  display_Print1stLine(buffer);
}
//==================================================================================================
void display_Print1stLine(const char* label) {
#ifdef UseDisplay
  lcd.home();
  lcd.print(BLANK_LINE);
  lcd.home();
  lcd.print(label);
#endif
}
//==================================================================================================
void display_Print2ndLine(const Action *action) {
  display_Print2ndLine(action, action->name);
}
//==================================================================================================
void display_Print2ndLine(const Action *action, const char* label) {
#ifdef UseDisplay
  char buffer[30];
  sprintf(buffer, "A%02d:%11s", action->actionCode, label);
  display_Print2ndLine(buffer);
#endif
}
//==================================================================================================
void display_Print2ndLine(const char* label) {
#ifdef UseDisplay
  lcd.setCursor(0, LINE2);        // set the cursor to position 0, line 2
  lcd.print(BLANK_LINE);
  lcd.setCursor(0, LINE2);
  lcd.print(label);
#endif
}
//==================================================================================================
void display_Clear1stLine() {
  __ClearLine(LINE1);
}
//==================================================================================================
void display_Clear2ndLine() {
  __ClearLine(LINE2);
}
//==================================================================================================
void __ClearLine(byte line) {
#ifdef UseDisplay
  lcd.setCursor(0, line);
  lcd.print(BLANK_LINE);
  lcd.setCursor(0, line);
#endif
}
//==================================================================================================
