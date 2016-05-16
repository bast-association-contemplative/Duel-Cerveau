/* ----------------------------------------------------------------------------------------------------
 * DUEL CERVEAU, 2016
 * Update: 16/05/16
 *
 * Installation connecting two brains to a LED bar, a mental tug of war
 * with OpenBCI / Processing
 * 
 * V1.1
 * Written by Bastien DIDIER
 * more info : http://one-billion-cat.com/
 *
 * ----------------------------------------------------------------------------------------------------
 */ 

#include <Adafruit_NeoPixel.h>

#define NEO_PIXEL_PIN 6
#define 	  RESET_PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int setupPos = strip.numPixels()/2;

//PLAYERS SETTINGS
const String J1 = "Red";
const char J1Char = 'r';
uint32_t J1color = strip.Color(255, 0, 0);

const String J2 = "Bleu";
const char J2Char = 'b';
uint32_t J2color = strip.Color(0, 0, 255);

volatile char inChar; //Stores the values sent by OpenBCI GUI

void setup() {
  // initialize serial:
  Serial.begin(115200);

  digitalWrite(RESET_PIN, HIGH);
  pinMode(RESET_PIN, OUTPUT);

  pinMode(NEO_PIXEL_PIN, OUTPUT);
  strip.begin();
  strip.show();

  //SETUP LED POSITION
  for(uint16_t i=0; i<setupPos; i++) {
      strip.setPixelColor(i, J1color);
      strip.show();
  }
  for(uint16_t i=setupPos; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, J2color);
      strip.show();
  }
  
  // Print Help
  Serial.println("BRAIN BATTLE Controller: starting..."); 
  Serial.println("Help Commands: ");
  Serial.println("    '"+String(J1Char)+"' = Add "+J1+" Pixel");
  Serial.println("    '"+String(J2Char)+"' = Add "+J2+" Pixel");

  Serial.println("Waiting for Alpha Brainwaves...");
}

void loop() {}

void serialEvent() {
  while (Serial.available()) {
    char inChar = Serial.read();
    Serial.print("Received :");
    Serial.println(inChar);
    
    if( inChar == J2Char ){
      if(setupPos > 0){
        strip.setPixelColor(setupPos, J2color);
        setupPos--;
        strip.setPixelColor(setupPos, J2color);
        strip.show();
      } else {
        win(J2, J2color);
      }
    } else if( inChar == J1Char ) {
      if(setupPos < strip.numPixels()){
        strip.setPixelColor(setupPos, J1color);
        setupPos++;
        strip.setPixelColor(setupPos, J1color);
        strip.show();
      } else {
        win(J1, J1color);
      }
    }
  }
}

void win(String J, uint32_t c){
  
  Serial.println(J+" wins");
  int nbBlink = 3;
  
  for(int i = 0; i<=3; i++){
    for(uint16_t i = 0; i <= nbBlink; i++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
      }
      delay(5);
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        strip.show();
      } 
    }
    delay(500);
  }

  //Wait before Resetting
  delay(5000);
  digitalWrite(RESET_PIN, LOW);
}
