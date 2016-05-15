/* ----------------------------------------------------------------------------------------------------
 * DUEL CERVEAU, 2016
 * Update: 15/05/16
 *
 * Installation connecting two brains to a LED bar, a mental tug of war
 * with OpenBCI / Processing
 * 
 * V1.0
 * Written by Bastien DIDIER
 * more info : http://one-billion-cat.com
 *
 * ----------------------------------------------------------------------------------------------------
 */ 

#include <Adafruit_NeoPixel.h>

// définition des broches utilisées
#define NEO_PIXEL_PIN 6
#define 	RESET_PIN 4

volatile char inChar;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int setupPos = strip.numPixels()/2;

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
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show();
  }

  for(uint16_t i=setupPos; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 255));
      strip.show();
  }
  
  // print help
  Serial.println("LIGHT Controller: starting..."); 
  Serial.println("Help Commands: ");
  Serial.println("    'r' = Add Red Pixel");
  Serial.println("    'p' = Add Bleu Pixel");
  
  Serial.println("Waiting for Alpha Brainwaves...");
}

void loop() {}

void serialEvent() {
  while (Serial.available()) {
    char inChar = Serial.read();
    Serial.print("Received :");
    Serial.println(inChar);
    
    if( inChar == 'p' ){
      if(setupPos > 0){
      strip.setPixelColor(setupPos, strip.Color(0, 0, 255));
      setupPos--;
      strip.setPixelColor(setupPos, strip.Color(0, 0, 255));
      strip.show();
      } else {
        Serial.println("Les Bleu Gagne");
        victoire(strip.Color(0, 0, 255), 3);
        delay(5000);
        digitalWrite(RESET_PIN, LOW);
      }
    } else if( inChar == 'r' ) {
      if(setupPos < strip.numPixels()){
      strip.setPixelColor(setupPos, strip.Color(255, 0, 0));
      setupPos++;
      strip.setPixelColor(setupPos, strip.Color(255, 0, 0));
      strip.show();
      } else {
        Serial.println("Les Rouge Gagne");
        victoire(strip.Color(255, 0,0), 3);
        delay(5000);
        digitalWrite(RESET_PIN, LOW);
      }
    }
    Serial.println(setupPos);
  }
}

void victoire(uint32_t c, uint8_t nbBlink){
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
}