#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN D0 // Pin for light senor
#define UNITTIME 50
#define LCD_MAX_COLS 16
#define LCD_MAX_ROWS  2


LiquidCrystal_I2C lcd(0x27,LCD_MAX_COLS,LCD_MAX_ROWS);  


int DOT = 1 * UNITTIME;  // length of Morse code dot
int DASH = 3 * UNITTIME; // length of Morse code dash

int sensorValue;            // variable to store the sensor value
bool dotDetected = false;   // flag to indicate if a dot has been detected
String message = "";        // string to create the received message
String codeDetected = "";   //morse codes detected
char letter; // variable to store the translated letter
unsigned long prevTime = 0; // variable to store the previous time a signal was detected

const char *MorseTable[] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* ASCI 0-31 has no MORSE code*/
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /*SPACE*/ NULL, /*!*/ "-.-.--", /*"*/ ".-..-.", /*#*/ NULL, /*$*/ "...-..-", /*%*/ NULL, /*&*/ ".-...", /*'*/ ".----.",
    /*(*/ "-.--.", /*)*/ "-.--.-", /***/ NULL, /*+*/ ".-.-.", /*,*/ "--..--", /*-*/ "-....-", /*.*/ ".-.-.-", /*/*/ "-..-.",
    /*0*/ "-----", /*1*/ ".----", /*2*/ "..---", /*3*/ "...--", /*4*/ "....-", /*5*/ ".....", /*6*/ "-....", /*7*/ "--...",
    /*8*/ "---..", /*9*/ "----.", /*:*/ "---...", /*;*/ "-.-.-.", /*<*/ NULL, /*=*/"-...-", /*>*/ NULL, /*?*/ "..--..",
    /*@*/ ".--.-.", /*A*/ ".-", /*B*/ "-...", /*C*/ "-.-.", /*D*/ "-..", /*E*/ ".", /*F*/ "..-.", /*G*/ "--.",
    /*H*/ "....", /*I*/ "..", /*J*/ ".---", /*K*/ "-.-", /*L*/ ".-..", /*M*/ "--", /*N*/ "-.", /*O*/ "---",
    /*P*/ ".--.", /*Q*/ "--.-", /*R*/ ".-.", /*S*/ "...", /*T*/ "-", /*U*/ "..-", /*V*/ "...-", /*W*/ ".--",
    /*X*/ "-..-", /*Y*/ "-.--", /*Z*/ "--.."};

void setup()
{
  
  lcd.init();                      
  lcd.backlight();

  // initialize serial communication
  Serial.begin(115200);
  Serial.println("Morse Code Receiver is ready...");
  lcd.setCursor(0,0);
  lcd.println("Morse Code Receiver is ready...");
  delay(1500);
  Serial.println("Starting receive");
  lcd.setCursor(0,1);
  lcd.println("Starting receive");
  delay(500);
  lcd.clear();  
  lcd.setCursor(0,0);
}

void loop()
{
  delay(1);
  sensorValue = analogRead(SENSOR_PIN); // read the sensor value
  if (sensorValue < 2.5)
  Serial.print("licht");
  {
    if (!dotDetected)
    {                      // if this is the start of a new signal
      prevTime = millis(); // store the current time
      dotDetected = true;  // set the dotDetected flag
    }
    else
    { // if this is a continuation of the current signal
      if (millis() - prevTime >= DASH)
      {                      // if a dash is detected
        codeDetected += '-';      // add a dash to the message
        dotDetected = false; // reset the dotDetected flag
      }
    }
  }
  else
  { // if no light is detected
    if (dotDetected)
    { // if the end of the signal is reached
      if (millis() - prevTime >= DOT)
      {                      // if a dot is detected
        codeDetected += '.';      // add a dot to the message
        dotDetected = false; // reset the dotDetected flag
      }
    }
  }
  if (codeDetected.length() > 0) { // if a message has been received
    letter = translateCode(codeDetected); // translate the message into a letter
    Serial.print(letter); // print the letter to the serial monitor
    codeDetected = ""; // clear the message string
    message += letter;
    lcd.print(letter);
  }

  
}


char translateCode(String  morseCode) 
{
  if (morseCode == "-.-.--") { return '!'; }
  else if (morseCode == ".-..-.") { return '"'; }
  else if (morseCode == "...-..-") { return '$'; }
  else if (morseCode == ".-...") { return '&'; }
  else if (morseCode == "-.--.") { return '('; }
  else if (morseCode == "-.--.-") { return ')'; }
  else if (morseCode == ".-.-.") { return '+'; }
  else if (morseCode == "--..--") { return ','; }
  else if (morseCode == "-....-") { return '-'; }
  else if (morseCode == ".-.-.-") { return '.'; }
  else if (morseCode == "-..-.") { return '/'; }
  else if (morseCode == "-----") { return '0'; }
  else if (morseCode == ".----") { return '1'; }
  else if (morseCode == "..---") { return '2'; }
  else if (morseCode == "...--") { return '3'; }
  else if (morseCode == "....-") { return '4'; }
  else if (morseCode == ".....") { return '5'; }
  else if (morseCode == "-....") { return '6'; }
  else if (morseCode == "--...") { return '7'; }
  else if (morseCode == "---..") { return '8'; }
  else if (morseCode == "----.") { return '9'; }
  else if (morseCode == "---...") { return ':'; }
  else if (morseCode == "-.-.-.") { return ';'; }
  else if (morseCode == "-...-") { return '='; }
  else if (morseCode == "..--..") { return '?'; }
  else if (morseCode == "..--..") { return '@'; }
  else if (morseCode == ".--.-.") { return 'A'; }
  else if (morseCode == "-...") { return 'B'; }
  else if (morseCode == "-.-.") { return 'C'; }
  else if (morseCode == "-..") { return 'D'; }
  else if (morseCode == ".") { return 'E'; }
  else if (morseCode == "..-.") { return 'F'; }
  else if (morseCode == "--.") { return 'G'; }
  else if (morseCode == "....") { return 'H'; }
  else if (morseCode == "..") { return 'I'; }
  else if (morseCode == ".---") { return 'J'; }
  else if (morseCode == "-.-") { return 'K'; }
  else if (morseCode == ".-..") { return 'L'; }
  else if (morseCode == "--") { return 'M'; }
  else if (morseCode == "-.") { return 'N'; }
  else if (morseCode == "---") { return 'O'; }
  else if (morseCode == ".--.") { return 'P'; }
  else if (morseCode == "--.-") { return 'Q'; }
  else if (morseCode == ".-.") { return 'R'; }
  else if (morseCode == "...") { return 'S'; }
  else if (morseCode == "-") { return 'T'; }
  else if (morseCode == "..-") { return 'U'; }
  else if (morseCode == "...-") { return 'V'; }
  else if (morseCode == ".--") { return 'W'; }
  else if (morseCode == "-..-") { return 'X'; }
  else if (morseCode == "-.--") { return 'Y'; }
  else if (morseCode == "--..") { return 'Z'; }
  return 0;
}