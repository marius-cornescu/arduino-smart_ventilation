#ifdef UseDisplay

//= CONSTANTS ======================================================================================
#define DISPLAY_STATUS_POS 15
#define DISPLAY_I2C_ADDRESS 0x27

#define BLANK_LINE "                                                                                "

//= VARIABLES ======================================================================================
LiquidCrystal_I2C lcd(DISPLAY_I2C_ADDRESS, 16, 2);
byte progress = 0;                  // used to indicate progress on display

//**************************************************************************************************
void display_Setup() {
  lcd.init();                 // initialise the LCD

  lcd.backlight(); // turn backlight on
  lcd.blink();     // cursor blinks
  //------------- TEST SCREEN -----------------------
  lcd.clear();         // clear the screen
  lcd.home();          // set the cursor to position 0, line 1
  // Numbers are stored but not displayed
  lcd.print("***************#123456789012345678901234***************#123456789012345678901234");  // write on the screen
  lcd.home();
  delay(TIME_TICK * 50);
  //-------------------
  lcd.clear();
  //-------------------------------------------------
  //lcd.noBacklight(); // turn backlight off
  lcd.noBlink();
}
//**************************************************************************************************

#endif

//==================================================================================================
void display_Print1stLine(const char* label, byte currentAction) {
#ifdef UseDisplay
  char buffer[80];
  sprintf(buffer, "%s: %d", label, currentAction);
  display_Print1stLine(buffer);
#endif
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
void display_ShowProgress() {
#ifdef UseDisplay

  lcd.setCursor(DISPLAY_STATUS_POS, 0);        // set the cursor to position 15, line 2
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
void display_Print2ndLine(const char* label) {
#ifdef UseDisplay
  lcd.setCursor(0, 1);        // set the cursor to position 0, line 2
  lcd.print("                                ");
  lcd.setCursor(0, 1);
  lcd.print(label);
#endif
}
//==================================================================================================
