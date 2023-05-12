#include <LiquidCrystal_I2C.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#define LCD_MAX_COLS 16
#define LCD_MAX_ROWS  2
LiquidCrystal_I2C lcd(0x27, LCD_MAX_COLS, LCD_MAX_ROWS);

void setup() {
  // set up the LCD's number of columns and rows:

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.println("Morse Code Receiver is ready...");
  

  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
