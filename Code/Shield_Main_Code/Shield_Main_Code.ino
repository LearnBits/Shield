/*
 LearnBits Raspberry Pi Shield - Demo Code - Arkadi Rafalovich 11/18/2015

 Description:
 Demonstration code of the basic functionalities.
 
 */
#define DEBUGPRINT

///////////////
// libraries //
///////////////
#include "LearnBitsShield.h"
#include <ArduinoJson.h>
#include "UARTReader.h"
#include "Wire.h"

// Sensors Libraries:
#include "I2Cdev.h"
#include "MPU6050.h"

HardwareSerial& SerialPi=Serial1;
///usb_serial_class& SerialPi=Serial;

//////////////////////////
// fnction decleration: //
//////////////////////////

// Parse message:
void Parser_MSG();

// Scan I2C bus
void Scan_I2C();

// Initialize I2C device
void Init_I2C(uint8_t I2C_ADDR);

// Update Sensor Values
void Sample_Sensors(unsigned long TimeStamp);

///////////////
// Variables //
///////////////


// I2C Max addresses
#define MAX_I2C_ADDR 0x7F

// I2C Devices ADDR:
#define MPU6050_ADDR 0x68

// variable to store if the device was allready initialized.
bool Initialized_I2C[MAX_I2C_ADDR]={0};

// store Available addresses
uint8_t Num_I2C_Available=0;
uint8_t Available_I2C[MAX_I2C_ADDR]={0};


/////////////////
// Sensors I2C //
/////////////////

// variable to store millis counter each loop.
unsigned long Time_Millis=0;

// MPU6050 Sensor variables
MPU6050 MPU6050_SENS;
int16_t MPU6050_VAL[6]={0,0,0,0,0,0};
unsigned long MPU6050_Millis=0;
unsigned long MPU6050_Millis_Delay=0;

////////////////////////////
// communication variable //
////////////////////////////
// Json 
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

UARTReader uartReader(SerialPi, (char*)"json:", '\n'); // \n


///////////////
// SETUP     //
///////////////

void setup() {
  // Initialize LearnBitsShield
  Init_LearnBitsShield();
  // Setup for Master mode, pins 18/19, external pullups, 400kHz //I2C_PULLUP_EXT
  Wire.begin();
}// End Setup

///////////////
// MAIN LOOP //
///////////////

void loop() {
  // Get message:
  int ret_code = uartReader.getMessage();
  
  // if message complete Parse message:
  if(ret_code == UARTReader::GOT_MESSAGE) {
    Parser_MSG();
  }
  if(ret_code == UARTReader::BUFF_OVERFLOW) Serial.println("BUFF_OVERFLOW");

  // Read Sensors and generate message:
  Time_Millis=millis();
  Sample_Sensors(Time_Millis);

} // end main


///////////////
// Functions //
///////////////

// Parse message:
void Parser_MSG(){
    // Json buffers must be declared locally so it's allocated on the stack automatically    
    StaticJsonBuffer<JSON_BUFFER_SIZE>  inputJsonBuffer; 
    StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
  
    // Parse cmd
    JsonObject& json = inputJsonBuffer.parseObject(uartReader.buffer());
    if(json.success()) {
      String CMD_Type=json["CMD"];
      
      if (CMD_Type=="MOTOR"){
        int Motor_Left_Val=json["MOVE"][0];
        int Motor_Right_Val=json["MOVE"][1];
        // send command to motors
        move(0, Motor_Left_Val);
        move(1, Motor_Left_Val);

        // generate a response
        JsonObject& jsonResp = outputJsonBuffer.createObject();
        jsonResp["RESP"] = "MOTOR";
        JsonArray& Move_Data = jsonResp.createNestedArray("MOVE");
        Move_Data.add(Motor_Left_Val);
        Move_Data.add(Motor_Right_Val);
        jsonResp.printTo(SerialPi);
        SerialPi.println(); // must end with a '\n'
      }
      
      if (CMD_Type=="LED"){
        for (int ii=0; ii<NUMPIXELS; ii++){
          int RGB_Table_Index=json["SET"][ii];
          if ((RGB_Table_Index>=0)&&(RGB_Table_Index<RGB_TABLE_SIZE))
          pixels.setPixelColor(ii,RGB_TABLE[RGB_Table_Index]); // Set pixel color. 
        }
        pixels.show(); // Updated pixel color Hardware.

        // generate a response
        JsonObject& jsonResp = outputJsonBuffer.createObject();
        jsonResp["RESP"] = "LED";
        JsonArray& Color_Data = jsonResp.createNestedArray("COLOR");
        for (int ii=0; ii<NUMPIXELS; ii++){
          uint32_t Color_Hex=pixels.getPixelColor(ii);
          Color_Data.add(Color_Hex);
        }
        jsonResp.printTo(SerialPi);
        SerialPi.println(); // must end with a '\n'
      }

      
      if (CMD_Type=="SCAN"){
        Scan_I2C();
        // generate a response
        JsonObject& jsonResp = outputJsonBuffer.createObject();
        jsonResp["RESP"] = "I2C";
        JsonArray& ADDR_I2C_Data = jsonResp.createNestedArray("ADDR");
        for (int ii=0; ii<Num_I2C_Available; ii++){
          ADDR_I2C_Data.add(Available_I2C[ii]);
        }
        jsonResp.printTo(SerialPi);
        SerialPi.println(); // must end with a '\n'
      }

     if (CMD_Type=="MPU6050"){
        MPU6050_Millis_Delay=(unsigned long)json["MSEC"];
        // generate a response
        JsonObject& jsonResp = outputJsonBuffer.createObject();
        jsonResp["RESP"] = "MPU6050";
        jsonResp["MSEC"] = MPU6050_Millis_Delay;
        jsonResp.printTo(SerialPi);
        SerialPi.println(); // must end with a '\n'
      }

    }// end json parse
}

// Scan I2C bus
void Scan_I2C(){
  uint8_t target; // slave addr
  Num_I2C_Available=0;
  for(target = 1; target <= MAX_I2C_ADDR; target++){ // sweep addr, skip general call
      Wire.beginTransmission(target);       // slave addr
      int error_status=Wire.endTransmission();               // no data, just addr
      if (error_status==0){      // address Available
        Available_I2C[Num_I2C_Available]=target; // update Available I2C ADDR
        Num_I2C_Available++;
        if (Initialized_I2C[target]==0){ // if device wasn`t initialized, initialize device.
          Init_I2C(target);
        }
      }else{
        Initialized_I2C[target]=0;
      }
  }
}

// Initialize I2C device
void Init_I2C(uint8_t I2C_ADDR){
  #ifdef DEBUGPRINT
    Serial.print("Addr 0x");
    Serial.print(I2C_ADDR,HEX);
    Serial.print("\r\n");
  #endif   
  // check which device it is:
  switch (I2C_ADDR){
    case MPU6050_ADDR:
      // initialize device
      MPU6050_SENS.initialize();
      #ifdef DEBUGPRINT
        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(MPU6050_SENS.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
      #endif
      Initialized_I2C[I2C_ADDR]=1; // initialization success,
      break;
    
    case 1:
      break;
    
    default:  // No Such Device:,
      Initialized_I2C[I2C_ADDR]=0;
      #ifdef DEBUGPRINT
        Serial.print("No Such Device:  ");
        Serial.print("Addr 0x");
        Serial.print(I2C_ADDR,HEX);
        Serial.print("\r\n");
      #endif 
  }
}

// Update Sensor Values
void Sample_Sensors(unsigned long TimeStamp){
 // MPU6050:
 StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
    
  if (MPU6050_Millis_Delay>0){
    if (Initialized_I2C[MPU6050_ADDR]){
      if (((TimeStamp-MPU6050_Millis)>MPU6050_Millis_Delay)||(MPU6050_Millis>Time_Millis)){
        MPU6050_Millis=TimeStamp;
        // Test connection:
        if (MPU6050_SENS.testConnection()){
          // Sample MPU6050 and send Message with data:
          MPU6050_SENS.getMotion6(&MPU6050_VAL[0], &MPU6050_VAL[1], &MPU6050_VAL[2], &MPU6050_VAL[3], &MPU6050_VAL[4], &MPU6050_VAL[5]); 
          #ifdef DEBUGPRINT
            Serial.print("a/g:\t");
            Serial.print(MPU6050_VAL[0]); Serial.print("\t");
            Serial.print(MPU6050_VAL[1]); Serial.print("\t");
            Serial.print(MPU6050_VAL[2]); Serial.print("\t");
            Serial.print(MPU6050_VAL[3]); Serial.print("\t");
            Serial.print(MPU6050_VAL[4]); Serial.print("\t");
            Serial.print(MPU6050_VAL[5]); Serial.print("\t");
            Serial.println(TimeStamp);
          #endif
          // generate a message
          JsonObject& jsonResp = outputJsonBuffer.createObject();
          jsonResp["SENSOR"] = "MPU6050";
          JsonArray& MPU6050_Data = jsonResp.createNestedArray("VAL");
          MPU6050_Data.add(MPU6050_VAL[0]);
          MPU6050_Data.add(MPU6050_VAL[1]);
          MPU6050_Data.add(MPU6050_VAL[2]);
          MPU6050_Data.add(MPU6050_VAL[3]);
          MPU6050_Data.add(MPU6050_VAL[4]);
          MPU6050_Data.add(MPU6050_VAL[5]);
          jsonResp.printTo(SerialPi);
          SerialPi.println(); // must end with a '\n'

         
        }else{ // sensor was disconnected:
          #ifdef DEBUGPRINT
            Serial.println("Sensor was disconnected");
          #endif 
            Initialized_I2C[MPU6050_ADDR]=0;
            MPU6050_Millis_Delay=0;
        }
      }
    }else{
      MPU6050_Millis_Delay=0;
      #ifdef DEBUGPRINT
        Serial.print("Device Not Initialized:  ");
        Serial.print("Addr 0x");
        Serial.print(MPU6050_ADDR,HEX);
        Serial.print("\r\n");
      #endif 
    }
  } 
}



