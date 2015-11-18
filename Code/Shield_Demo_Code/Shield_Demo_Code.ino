/*
  Mega multple serial test

 Receives from the main serial port, sends to the others.
 Receives from serial port 1, sends to the main serial (Serial 0).

 This example works only on the Arduino Mega

 The circuit:
 * Any serial device attached to Serial port 1
 * Serial monitor open on Serial port 0:

 created 30 Dec. 2008
 modified 20 May 2012
 by Tom Igoe & Jed Roach

 This example code is in the public domain.

 */

#include <Adafruit_NeoPixel.h>
#define PIN            23
#define NUMPIXELS      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int STBY = 20; //standby

//Motor A
int PWMA = 22; //Speed control 
int AIN1 = 17; //Direction
int AIN2 = 16; //Direction

//Motor B
int PWMB = 21; //Speed control
int BIN1 = 15; //Direction
int BIN2 = 14; //Direction



int led = 13;
void setup() {
  // initialize both serial ports:
  Serial.begin(57600);
  Serial2.begin(57600);
    pinMode(led, OUTPUT);   
      digitalWrite(led, HIGH); 
      pixels.begin(); // This initializes the NeoPixel library.
 for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(50,50,0)); // Moderately bright green color. 
 }
 pixels.show(); // This sends the updated pixel color to the hardware.

   pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);


}

void loop() {
  // read from port 1, send to port 0:
  if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }

  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  if (0){
  move(1, 255, 1); //motor 1, full speed, left
  move(2, 255, 1); //motor 2, full speed, left

  delay(1000); //go for 1 second
  stop(); //stop
  delay(250); //hold for 250ms until move again

  move(1, 128, 0); //motor 1, half speed, right
  move(2, 128, 0); //motor 2, half speed, right

    delay(1000); //go for 1 second
  stop(); //stop
  delay(250); //hold for 250ms until move again

  move(1, 128, 1); //motor 1, half speed, right
  move(2, 128, 0); //motor 2, half speed, right

  delay(1000);
  stop();
  delay(250);
}
}





void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }

}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}
