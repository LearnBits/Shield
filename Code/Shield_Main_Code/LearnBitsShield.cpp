/*
 LearnBits Raspberry Pi Shield - Arkadi Rafalovich 11/24/2015

 Description:
 LearnBits Raspberry pi Shield V0

 */


#include "LearnBitsShield.h"

///////////////////////////
//  Neo Pixels Variables //
///////////////////////////
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DIN_NEO, NEO_GRB + NEO_KHZ800);

const uint32_t RGB_TABLE[RGB_TABLE_SIZE]={
0x800000,0x8B0000,0xA52A2A,0xB22222,0xDC143C,0xFF0000,0xFF6347,0xFF7F50,0xCD5C5C,0xF08080,0xE9967A,0xFA8072,
0xFFA07A,0xFF4500,0xFF8C00,0xFFA500,0xFFD700,0xB8860B,0xDAA520,0xEEE8AA,0xBDB76B,0xF0E68C,0x808000,0xFFFF00,
0x9ACD32,0x556B2F,0x6B8E23,0x7CFC00,0x7FFF00,0xADFF2F,0x006400,0x008000,0x228B22,0x00FF00,0x32CD32,0x90EE90,
0x98FB98,0x8FBC8F,0x00FA9A,0x00FF7F,0x2E8B57,0x66CDAA,0x3CB371,0x20B2AA,0x2F4F4F,0x008080,0x008B8B,0x00FFFF,
0x00FFFF,0xE0FFFF,0x00CED1,0x40E0D0,0x48D1CC,0xAFEEEE,0x7FFFD4,0xB0E0E6,0x5F9EA0,0x4682B4,0x6495ED,0x00BFFF,
0x1E90FF,0xADD8E6,0x87CEEB,0x87CEFA,0x191970,0x000080,0x00008B,0x0000CD,0x0000FF,0x4169E1,0x8A2BE2,0x4B0082,
0x483D8B,0x6A5ACD,0x7B68EE,0x9370DB,0x8B008B,0x9400D3,0x9932CC,0xBA55D3,0x800080,0xD8BFD8,0xDDA0DD,0xEE82EE,
0xFF00FF,0xDA70D6,0xC71585,0xDB7093,0xFF1493,0xFF69B4,0xFFB6C1,0xFFC0CB,0xFAEBD7,0xF5F5DC,0xFFE4C4,0xFFEBCD,
0xF5DEB3,0xFFF8DC,0xFFFACD,0xFAFAD2,0xFFFFE0,0x8B4513,0xA0522D,0xD2691E,0xCD853F,0xF4A460,0xDEB887,0xD2B48C,
0xBC8F8F,0xFFE4B5,0xFFDEAD,0xFFDAB9,0xFFE4E1,0xFFF0F5,0xFAF0E6,0xFDF5E6,0xFFEFD5,0xFFF5EE,0xF5FFFA,0x708090,
0x778899,0xB0C4DE,0xE6E6FA,0xFFFAF0,0xF0F8FF,0xF8F8FF,0xF0FFF0,0xFFFFF0,0xF0FFFF,0xFFFAFA,0x000000,0x696969,
0x808080,0xA9A9A9,0xC0C0C0,0xD3D3D3,0xDCDCDC,0xF5F5F5,0xFFFFFF};

///////////////////////
// Define Functions //
///////////////////////

// Shield initialize function:
void Init_LearnBitsShield(){
  // initialize serial ports:
  Serial.begin(57600); // Pc Communication
  Serial1.begin(57600); // Pi communication
  Serial2.begin(57600); // Bluetooth Communication

  // initialize motor driver
  pinMode(MOT_STBY, OUTPUT);
  pinMode(MOT_PWMA, OUTPUT);
  pinMode(MOT_AIN1, OUTPUT);
  pinMode(MOT_AIN2, OUTPUT);
  pinMode(MOT_PWMB, OUTPUT);
  pinMode(MOT_BIN1, OUTPUT);
  pinMode(MOT_BIN2, OUTPUT);
  digitalWrite(MOT_STBY, LOW);  //Set standby

  // initialize Neo pixels
  pixels.begin(); // initializes NeoPixel library.
  for(int ii=0;ii<NUMPIXELS;ii++){
    pixels.setPixelColor(ii, pixels.Color(0,0,0)); // Set pixel color. 
  }
  pixels.show(); // Updated pixel color Hardware.
  

}// end Init_LearnBitsShield



// Motors Functions
//Move specific motor at speed and direction
void move(int motor, int speed){
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(MOT_STBY, HIGH); //disable standby

  boolean direction=0;
  if (speed<0){
    direction=1;
    speed=abs(speed);
  }
  
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


