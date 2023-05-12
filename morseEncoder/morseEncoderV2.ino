#include <Wire.h> 
#include <Arduino.h>
#define EMITTER_PIN D0
#define BUZZER_PIN D7
#define UNITTIME 50


// Morse code timings 
int DOT = 1 * UNITTIME;
int DASH = 3 * UNITTIME;
int INTERSYMBOL = 1 * UNITTIME; 
int INTERLETTER = 3 * UNITTIME;
int INTERWORD = 7 * UNITTIME;




String message = "MARCEL";

const char* MorseTable[] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* ASCI 0-31 has no MORSE code*/
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,        
        /*SPACE*/ NULL, /*!*/ "-.-.--", /*"*/ ".-..-.", /*#*/ NULL, /*$*/ "...-..-", /*%*/ NULL, /*&*/ ".-...", /*'*/ ".----.",
        /*(*/ "-.--.", /*)*/ "-.--.-", /***/ NULL, /*+*/ ".-.-.", /*,*/ "--..--", /*-*/ "-....-", /*.*/ ".-.-.-", /*/*/ "-..-.",
        /*0*/ "-----", /*1*/ ".----", /*2*/ "..---", /*3*/ "...--", /*4*/ "....-", /*5*/ ".....", /*6*/ "-....", /*7*/ "--...",
        /*8*/ "---..", /*9*/ "----.", /*:*/ "---...", /*;*/ "-.-.-.", /*<*/ NULL, /*=*/ "-...-", /*>*/ NULL, /*?*/ "..--..",
        /*@*/ ".--.-.", /*A*/ ".-", /*B*/ "-...", /*C*/ "-.-.", /*D*/ "-..", /*E*/ ".", /*F*/ "..-.", /*G*/ "--.",
        /*H*/ "....", /*I*/ "..", /*J*/ ".---", /*K*/ "-.-", /*L*/ ".-..", /*M*/ "--", /*N*/ "-.", /*O*/ "---", 
        /*P*/ ".--.", /*Q*/ "--.-", /*R*/ ".-.", /*S*/ "...", /*T*/ "-", /*U*/ "..-", /*V*/ "...-",/*W*/ ".--",
        /*X*/ "-..-", /*Y*/ "-.--", /*Z*/ "--.."};


void setup() {  
  pinMode(EMITTER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);

  // send the message
  tone(BUZZER_PIN, 440, DOT);  
  Serial.println("Morse Code Transmitter is ready...");

  delay(1500);
  Serial.println("Starting to send");  
  delay(500);
   

  sendMessage(message);   
  message = "";    
  Serial.println("Message sent");
      
  
}


// loop code is not used for this example. 
void loop() { 

  delay(1);
}

void sendMessage(String message)
{
  
  Serial.print("Sending message: [");Serial.print(message); Serial.println("]");    
  for (int i=0;i< message.length(); i++){
    char ch = message[i];  
   
    if (ch==' '){
      Serial.print("["); Serial.print(ch); Serial.println("]: [ ]"); 
      sendSpace();
    }
    else{
      Serial.print("["); Serial.print(ch); Serial.print("]: [");  
      sendChar(MorseTable[ch]);    
    }
    
    delay(INTERLETTER-INTERSYMBOL);        
  }    
  
  delay(1000);
}

void sendChar(const char * morseCode) 
{
  int i = 0;
  while(morseCode[i] != 0)
  {
    Serial.print(morseCode[i]);
    if(morseCode[i] == '.'){
      sendDot();
    } else if (morseCode[i] == '-'){
      sendDash();
    }
    i++;
  }
  Serial.println("]");
}


void sendDot()
{
  digitalWrite(EMITTER_PIN, HIGH);
  tone(BUZZER_PIN, 440, DOT);
  delay(DOT);
  noTone(BUZZER_PIN);
  digitalWrite(EMITTER_PIN, LOW);   
  delay(INTERSYMBOL);
}

void sendDash()
{
  digitalWrite(EMITTER_PIN, HIGH);
  tone(BUZZER_PIN, 440, DASH);
  delay(DASH);
  noTone(BUZZER_PIN);
  digitalWrite(EMITTER_PIN, LOW);    
  delay(INTERSYMBOL);
} 

void sendSpace()
{  
  delay(INTERWORD - INTERLETTER);
} 