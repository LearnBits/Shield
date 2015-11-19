/*
 LearnBits Raspberry Pi Shield - Demo Code - Arkadi Rafalovich 11/18/2015

 Description:
 Demonstration code of the basic functionalities.
 
 */


///////////////
// libraries //
///////////////
#include "LearnBitsShield.h"



///////////////
// Variables //
///////////////

////////////////
// Neo Pixels //
////////////////

////////////////////////////
// communication variable //
////////////////////////////

///////////////
// SETUP     //
///////////////

void setup() {
  // Initialize LearnBitsShield
  Init_LearnBitsShield();
  
}// End Setup

///////////////
// MAIN LOOP //
///////////////

void loop() {
  static int inByte=' ';
  // read from port 2, send to port 0,1:
  if (Serial2.available()) {
    inByte = Serial2.read();
    Serial.write(inByte);
    Serial1.write(inByte);
  }

  // read from port 0, send to port 1,2:
  if (Serial.available()) {
    inByte = Serial.read();
    Serial1.write(inByte);
    Serial2.write(inByte);
  }

  // read from port 1, send to port 0,2:
  if (Serial1.available()) {
    inByte = Serial1.read();
    Serial.write(inByte);
    Serial2.write(inByte);
  }

  // actuate motors based on the incoming byte
  switch (inByte){
    // move forward
    case 'f':
      move(1, 25, 1); //motor 1, full speed, left
      move(2, 25, 1); //motor 2, full speed, left
      inByte=' ';
    break;

    // move reverse
    case 'b':
      move(1, 25, 0); //motor 1, full speed, left
      move(2, 25, 0); //motor 2, full speed, left
      inByte=' ';
    break;

    // move left
    case 'l':
      move(1, 25, 0); //motor 1, full speed, left
      move(2, 25, 1); //motor 2, full speed, left
      inByte=' ';
    break;

    // move right
    case 'r':
      move(1, 25, 1); //motor 1, full speed, left
      move(2, 25, 0); //motor 2, full speed, left
    inByte=' ';
    break;

    // stop
    case 's': 
      stop();
      inByte=' ';
    break;
    
    default:
    break; 
  }
} // end main


///////////////
// Functions //
///////////////
