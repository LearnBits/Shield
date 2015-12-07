/*
 LearnBits Raspberry Pi Shield - Demo Code - Arkadi Rafalovich 11/18/2015

 Description:
 Demonstration code of the basic functionalities.
 
 */


///////////////
// libraries //
///////////////
#include "LearnBitsShield.h"
#include <ArduinoJson.h>
#include "UARTReader.h"

///////////////
// Variables //
///////////////

////////////////////////////
// communication variable //
////////////////////////////
// Json 
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

UARTReader uartReader(Serial, (char*)"json:", '\n');


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
  // Get message:
  int ret_code = uartReader.getPacket();
  
  // if message complete Parse message:
  if(ret_code == UARTReader::GOT_PACKET) {
    Parser_MSG();
  }
  if(ret_code == UARTReader::BUFF_OVERFLOW) Serial.println("BUFF_OVERFLOW");

  
//  static int inByte=' ';
//  // read from port 2, send to port 0,1:
//  if (Serial2.available()) {
//    inByte = Serial2.read();
//    Serial.write(inByte);
//    Serial1.write(inByte);
//  }
//
//  // read from port 0, send to port 1,2:
//  if (Serial.available()) {
//    inByte = Serial.read();
//    Serial1.write(inByte);
//    Serial2.write(inByte);
//  }
//
//  // read from port 1, send to port 0,2:
//  if (Serial1.available()) {
//    inByte = Serial1.read();
//    Serial.write(inByte);
//    Serial2.write(inByte);
//  }
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
        jsonResp.printTo(Serial);
        Serial.println(); // must end with a '\n'
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
        jsonResp.printTo(Serial);
        Serial.println(); // must end with a '\n'
      }


      
    }
}

