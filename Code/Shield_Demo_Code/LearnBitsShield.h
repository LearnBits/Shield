/*
  LearnBits Raspberry Pi Shield - Demo Code - Arkadi Rafalovich 11/18/2015
  
  Description:
  LearnBits Raspberry pi Shield V0
 
  Pinout:
  Teensy 3.1 3.3v
  Uart 0 --> PC USB
  
  RaspberryPi 3.3V
  Uart 1 --> Pi Uart
   
  Bluetooth 3.3v
  Uart 2 --> BTM HC-05
  
  Motor Driver Vin
  D22 --> MOT_PWMA
  D21 --> MOT_PWMB
  D20 --> MOT_STBY
  D17 --> MOT_AIN2
  D16 --> MOT_AIN1
  D15 --> MOT_BIN1
  D14 --> MOT_BIN2
  
  NeoPixel 5V
  D23 --> DIN_NEO
  
  I2CBus 3.3V
  D18 --> I2C_SDA
  D19 --> I2C_SCL
  
  NRF24 3.3V
  D6  --> CE_NRF
  D2  --> CSN_NRF
  D13 (SCK)  --> SCK_NRF
  D11 (MOSI) --> MOSI_NRF
  D12 (MISO) --> MISO_NRF
  D5  --> IRQ_NRF

*/
#ifndef LEARNBITSSHIELD_H_
#define LEARNBITSSHIELD_H_

/////////////////////////
//  Included Libraries //
/////////////////////////
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

/////////////////
//  Variables: //
/////////////////

////////////////////
//  Shield PinOut //
////////////////////

//  Motor Driver Vin
#define MOT_PWMA 22
#define MOT_PWMB 21
#define MOT_STBY 20
#define MOT_AIN2 17
#define MOT_AIN1 16
#define MOT_BIN2 15
#define MOT_BIN1 14
  
//  NeoPixel 5V
#define DIN_NEO 23

//  NRF24 3.3V

#define CE_NRF 6
#define CSN_NRF 2
#define SCK_NRF 13
#define MOSI_NRF 11
#define MISO_NRF 12
#define IRQ_NRF 5


///////////////////////////
//  Neo Pixels Variables //
///////////////////////////
#define NUMPIXELS      8

///////////////////////
// Declare Functions //
///////////////////////

// Shield initialize function:
void Init_LearnBitsShield();

// Motors Functions
//Move specific motor at speed
void move(int motor, int speed);
  
// Stop Motors
void stop();


#endif /* LEARNBITSSHIELD_H_ */
