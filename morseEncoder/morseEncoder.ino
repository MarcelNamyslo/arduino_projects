#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN D5 // Pin for light senor
#define UNITTIME 300
#define LCD_MAX_COLS 16
#define LCD_MAX_ROWS  2




LiquidCrystal_I2C lcd(0x27, LCD_MAX_COLS, LCD_MAX_ROWS);


int DOT = 1 * UNITTIME;  // length of Morse code dot
int DASH = 3 * UNITTIME; // length of Morse code dash

int INTERSYMBOL = 1 * UNITTIME;
int INTERLETTER = 3 * UNITTIME;
int INTERWORD = 7 * UNITTIME;

int sensorValue;            // variable to store the sensor value
bool lightDetected = false;   // flag to indicate if a dot has been detected
String message = "";        // string to create the received message
String codeDetected = "";   //morse codes detected
char letter; // variable to store the translated letter
unsigned long lightsOnMoments = 0; // variable to store the previous time a signal was detected

unsigned long timeToNextLight = 0;
bool receiving = false;

bool lightsout = false;
bool switchMode = false;
bool dashprinted = false;
bool lightmode = false;
unsigned lightsoutTime = 0;
unsigned lightsOnTime = 0;
unsigned long lightsOutMoment = 0;

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
  /*X*/ "-..-", /*Y*/ "-.--", /*Z*/ "--.."
};

void setup()
{

  lcd.init();
  lcd.backlight();

  // initialize serial communication
  Serial.begin(115200);
  Serial.println("Morse Code Receiver is ready...");
  lcd.setCursor(0, 0);
  lcd.println("Morse Code Receiver is ready...");
  delay(3000);
  Serial.println("Starting receive");
  lcd.setCursor(0, 1);
  lcd.println("Starting receive");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void loop()
{
  sensorValue = analogRead(D5); // read the sensor value
  //Serial.println(sensorValue);
  if (sensorValue < 2.5) {
    receiving = true;
    if (!lightmode) {
      if (!lightDetected) {
        lightDetected = true;
        lightsOnMoments = millis();
        if (lightsout) {
          lightsoutTime = millis() - lightsOutMoment;
          //Serial.println(lightsoutTime);


          if (lightsoutTime >= INTERLETTER - 100)
          {
            if (codeDetected.length() > 0) {
              letter = translateCode(codeDetected);
              Serial.println(codeDetected);
              Serial.println(letter);
              message += letter;
              lcd.clear();
              lcd.print(message);
              codeDetected = "";

            }
          }
          if (lightsoutTime  >= INTERWORD - 100)
          {
            message += " ";
            Serial.println("leerzeichen");

          }
        }
      }

    } lightsout = false;
    lightmode = true;
  } else {
    if (lightmode) {
      if (!lightsout)
      {
        lightsout = true;
        lightDetected = false;
        lightsOutMoment = millis();
        lightsOnTime = lightsOutMoment - lightsOnMoments;
        //Serial.println(lightsOnTime);
        if (lightsOnTime >= DASH - 50) {
          codeDetected += '-';
        }
        else if (lightsOnTime >= DOT - 50 ) {
          codeDetected += '.';
        }


      }

      lightmode = false;

    }
    if (receiving) {
      if (millis() - lightsOutMoment >= 8 * UNITTIME) {
        receiving = false;
        letter = translateCode(codeDetected);
        Serial.println(codeDetected);
        Serial.println(letter);
        message += letter;
        Serial.println(message);
        lcd.print(message);
        
      }

    } else {
      delay(1);
    }
  }
}


char translateCode(String  morseCode)
{
  if (morseCode == "-.-.--") {
    return '!';
  }
  else if (morseCode == ".-..-.") {
    return '"';
  }
  else if (morseCode == "...-..-") {
    return '$';
  }
  else if (morseCode == ".-...") {
    return '&';
  }
  else if (morseCode == "-.--.") {
    return '(';
  }
  else if (morseCode == "-.--.-") {
    return ')';
  }
  else if (morseCode == ".-.-.") {
    return '+';
  }
  else if (morseCode == "--..--") {
    return ',';
  }
  else if (morseCode == "-....-") {
    return '-';
  }
  else if (morseCode == ".-.-.-") {
    return '.';
  }
  else if (morseCode == "-..-.") {
    return '/';
  }
  else if (morseCode == "-----") {
    return '0';
  }
  else if (morseCode == ".----") {
    return '1';
  }
  else if (morseCode == "..---") {
    return '2';
  }
  else if (morseCode == "...--") {
    return '3';
  }
  else if (morseCode == "....-") {
    return '4';
  }
  else if (morseCode == ".....") {
    return '5';
  }
  else if (morseCode == "-....") {
    return '6';
  }
  else if (morseCode == "--...") {
    return '7';
  }
  else if (morseCode == "---..") {
    return '8';
  }
  else if (morseCode == "----.") {
    return '9';
  }
  else if (morseCode == "---...") {
    return ':';
  }
  else if (morseCode == "-.-.-.") {
    return ';';
  }
  else if (morseCode == "-...-") {
    return '=';
  }
  else if (morseCode == "..--..") {
    return '?';
  }
  else if (morseCode == "..--..") {
    return '@';
  }
  else if (morseCode == ".--.-.") {
    return 'A';
  }
  else if (morseCode == "-...") {
    return 'B';
  }
  else if (morseCode == "-.-.") {
    return 'C';
  }
  else if (morseCode == "-..") {
    return 'D';
  }
  else if (morseCode == ".") {
    return 'E';
  }
  else if (morseCode == "..-.") {
    return 'F';
  }
  else if (morseCode == "--.") {
    return 'G';
  }
  else if (morseCode == "....") {
    return 'H';
  }
  else if (morseCode == "..") {
    return 'I';
  }
  else if (morseCode == ".---") {
    return 'J';
  }
  else if (morseCode == "-.-") {
    return 'K';
  }
  else if (morseCode == ".-..") {
    return 'L';
  }
  else if (morseCode == "--") {
    return 'M';
  }
  else if (morseCode == "-.") {
    return 'N';
  }
  else if (morseCode == "---") {
    return 'O';
  }
  else if (morseCode == ".--.") {
    return 'P';
  }
  else if (morseCode == "--.-") {
    return 'Q';
  }
  else if (morseCode == ".-.") {
    return 'R';
  }
  else if (morseCode == "...") {
    return 'S';
  }
  else if (morseCode == "-") {
    return 'T';
  }
  else if (morseCode == "..-") {
    return 'U';
  }
  else if (morseCode == "...-") {
    return 'V';
  }
  else if (morseCode == ".--") {
    return 'W';
  }
  else if (morseCode == "-..-") {
    return 'X';
  }
  else if (morseCode == "-.--") {
    return 'Y';
  }
  else if (morseCode == "--..") {
    return 'Z';
  }
  return 0;
}
