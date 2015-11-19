/*
 LearnBits Raspberry Pi Shield - Demo Code - Arkadi Rafalovich 11/18/2015

 Description:
 LearnBits Raspberry pi Shield V0

 */


#include "LearnBitsShield.h"

//////////////////////
//  Define Objects  //
//////////////////////
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DIN_NEO, NEO_GRB + NEO_KHZ800);


///////////////////////
// Define Functions //
///////////////////////

// Shield initialize function:
void Init_LearnBitsShield(){
  // initialize serial ports:
  Serial.begin(57600); // Pc Communication
  Serial1.begin(57600); // Pi communication
  Serial2.begin(57600); // Bluetooth Communication

  // initialize Neo pixels
  pixels.begin(); // initializes NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(50,50,0)); // Set pixel color. 
  }
  pixels.show(); // Updated pixel color Hardware.

  // initialize motor driver
  pinMode(MOT_STBY, OUTPUT);
  pinMode(MOT_PWMA, OUTPUT);
  pinMode(MOT_AIN1, OUTPUT);
  pinMode(MOT_AIN2, OUTPUT);
  pinMode(MOT_PWMB, OUTPUT);
  pinMode(MOT_BIN1, OUTPUT);
  pinMode(MOT_BIN2, OUTPUT);
  digitalWrite(MOT_STBY, LOW);  //Set standby

}// end Init_LearnBitsShield



// Motors Functions
//Move specific motor at speed and direction
void move(int motor, int speed, int direction){
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(MOT_STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(MOT_AIN1, inPin1);
    digitalWrite(MOT_AIN2, inPin2);
    analogWrite(MOT_PWMA, speed);
  }else{
    digitalWrite(MOT_BIN1, inPin1);
    digitalWrite(MOT_BIN2, inPin2);
    analogWrite(MOT_PWMB, speed);
  }

}

// Stop Motors
void stop(){
//enable standby  
  digitalWrite(MOT_STBY, LOW); 
}


