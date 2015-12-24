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
#include <SFE_BMP180.h>
#include "I2C_ADC.h"

HardwareSerial& SerialPi=Serial1;
//usb_serial_class& SerialPi=Serial;

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

// Sum up message and send
void SendResponse(JsonObject& req, JsonObject& resp);

//Reset Sensors
void Resset_Sensors();
///////////////
// Variables //
///////////////


// I2C Max addresses
#define MAX_I2C_ADDR 0x7F
// I2C Devices ADDR:
#define SLIDEPOT_ADDR  0x50 // 80
#define MPU6050_ADDR   0x68 // 104
#define BMP180_ADDR    0x77 // 119

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
unsigned long MPU6050_Millis=0;
unsigned long MPU6050_Millis_Delay=0;
uint16_t MPU6050_Count=0;

// Barometer sensor BMP180
SFE_BMP180 BMP180_SENS;
unsigned long BMP180_Millis=0;
unsigned long BMP180_Sample_Delay=0; // sample delay of BMP180 afte get sample request
unsigned long BMP180_Millis_Delay=0;
uint16_t BMP180_Count=0;

// Slide Potentiometer ( SLIDEPOT )
unsigned long SLIDEPOT_Millis=0;
unsigned long SLIDEPOT_Millis_Delay=0;
uint16_t SLIDEPOT_Count=0;

////////////////////////////
// communication variable //
////////////////////////////
// Json 
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

UARTReader uartReader(SerialPi, (char*)"json:", '\n'); // \n //


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
  if(ret_code == UARTReader::UART_GOT_MESSAGE) {
    Parser_MSG();
  }
  if(ret_code == UARTReader::UART_BUFF_OVERFLOW) Serial.println("BUFF_OVERFLOW");

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

      // generate response
      JsonObject& jsonResp = outputJsonBuffer.createObject();
      jsonResp["RESP"] = CMD_Type;
      
      if (CMD_Type=="MOTOR"){
        int Motor_Left_Val=json["MOVE"][0];
        int Motor_Right_Val=json["MOVE"][1];
        // send command to motors
        Motor_move(0, Motor_Left_Val);
        Motor_move(1, Motor_Right_Val);
      }
      
      if (CMD_Type=="LED"){
        for (int ii=0; ii<NUMPIXELS; ii++){
          int RGB_Table_Index=json["SET"][ii];
          if ((RGB_Table_Index>=0)&&(RGB_Table_Index<RGB_TABLE_SIZE))
          pixels.setPixelColor(ii,RGB_TABLE[RGB_Table_Index]); // Set pixel color. 
        }
        pixels.show(); // Updated pixel color Hardware.
      }
       
      if (CMD_Type=="SCAN"){
        Scan_I2C();
        // update response with available I2C addreses.
        JsonArray& ADDR_I2C_Data = jsonResp.createNestedArray("I2C_ADDR");
        for (int ii=0; ii<Num_I2C_Available; ii++){
          ADDR_I2C_Data.add(Available_I2C[ii]);
        }
      }

      if (CMD_Type=="RESET"){
        Resset_Sensors();
      }

      if (CMD_Type=="MPU6050"){
        MPU6050_Millis_Delay=(unsigned long)json["MSEC"];
      }

      if (CMD_Type=="BMP180"){
        BMP180_Millis_Delay=(unsigned long)json["MSEC"];
        if ((BMP180_Millis_Delay<10)&&(BMP180_Millis_Delay!=0)) BMP180_Millis_Delay=10; // Max sample frequency 100 Hz
      }
      if (CMD_Type=="SLIDEPOT"){
        SLIDEPOT_Millis_Delay=(unsigned long)json["MSEC"];
      }
      // send response:
      SendResponse(json,jsonResp); 
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
  int Connection_Error_Status=0;
  // check which device it is:
  switch (I2C_ADDR){
    case MPU6050_ADDR:
      // initialize device
      MPU6050_SENS.initialize();
      Connection_Error_Status=MPU6050_SENS.testConnection();
      #ifdef DEBUGPRINT
        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(Connection_Error_Status ? "MPU6050 connection successful" : "MPU6050 connection failed");
      #endif
      if (Connection_Error_Status) Initialized_I2C[I2C_ADDR]=1; // initialization success,
      break;
    
    case BMP180_ADDR:
      // initialize device
      Connection_Error_Status=BMP180_SENS.begin(); // returns if initialization was succesful
      #ifdef DEBUGPRINT
        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(Connection_Error_Status ? "BMP180 connection successful" : "BMP180 connection failed");
      #endif
      if (Connection_Error_Status) Initialized_I2C[I2C_ADDR]=1; // initialization success,
      break;

    case SLIDEPOT_ADDR:
      // initialize device
      Connection_Error_Status=Init_I2C_ADC(SLIDEPOT_ADDR); // returns if initialization was succesful
      #ifdef DEBUGPRINT
        // verify connection
        Serial.println("Testing device connections...");
        Serial.println(Connection_Error_Status ? "SLIDEPOT connection successful" : "SLIDEPOT connection failed");
      #endif
      if (Connection_Error_Status) Initialized_I2C[I2C_ADDR]=1; // initialization success,
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
    
  //sample MPU6050 
  if (MPU6050_Millis_Delay>0){   
    if (Initialized_I2C[MPU6050_ADDR]){
      if (((TimeStamp-MPU6050_Millis)>MPU6050_Millis_Delay)||(MPU6050_Millis>Time_Millis)){
        MPU6050_Millis=TimeStamp;
        // Test connection:
        if (MPU6050_SENS.testConnection()){
          // Sample MPU6050 and send Message with data:
          int16_t MPU6050_VAL[6]={0,0,0,0,0,0};
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
          StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;  // generate json  buffer
          // generate a message
          JsonObject& jsonResp = outputJsonBuffer.createObject();
          jsonResp["SAMPLE_ID"] = "MPU6050";
          jsonResp["COUNT"] = MPU6050_Count++;
          
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

  //sample BMP180
  if (BMP180_Millis_Delay>0){
    static double BMP180_Temperature=0;
    static double BMP180_Pressure=0;
    static boolean BMP180_GetSample_Pressure=0; // flag to get sample after sample request
    static boolean BMP180_GetSample_Temperature=0; // flag to get sample after sample request
    if (Initialized_I2C[BMP180_ADDR]){
      /// get sample // overflow isn`t handled properly yet
        if (((TimeStamp-BMP180_Millis)>(BMP180_Millis_Delay+BMP180_Sample_Delay))||(BMP180_Millis>Time_Millis)){ // get sample
          // Test connection:
          Wire.beginTransmission(BMP180_ADDR);
          if (Wire.endTransmission()==0){ // connection succes
            if (BMP180_GetSample_Temperature){
              BMP180_SENS.getTemperature(BMP180_Temperature);
              BMP180_GetSample_Temperature=0;
              BMP180_Sample_Delay+=BMP180_SENS.startPressure(0);
              BMP180_GetSample_Pressure=1;
            }else if (BMP180_GetSample_Pressure){
              BMP180_SENS.getPressure(BMP180_Pressure,BMP180_Temperature);
              BMP180_GetSample_Pressure=0;
              BMP180_Millis=TimeStamp-BMP180_Sample_Delay; // update time stamp only after temperature and pressure were sampled
              BMP180_Sample_Delay=0; // reset delay;
              // send json with new data
              #ifdef DEBUGPRINT
                Serial.print("T,P:\t");
                Serial.print(BMP180_Temperature); Serial.print("\t");
                Serial.print(BMP180_Pressure); Serial.print("\t");
                Serial.println(TimeStamp);
              #endif
              StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;  // generate json  buffer 
              // generate a message
              JsonObject& jsonResp = outputJsonBuffer.createObject();
              jsonResp["SAMPLE_ID"] = "BMP180";
              jsonResp["COUNT"] = BMP180_Count++;
              JsonArray& BMP180_Data = jsonResp.createNestedArray("VAL");
              BMP180_Data.add(BMP180_Temperature);
              BMP180_Data.add(BMP180_Pressure);
              jsonResp.printTo(SerialPi);
              SerialPi.println(); // must end with a '\n'
              
            }else{ // get new sample, start with temperature sample
              BMP180_Sample_Delay=BMP180_SENS.startTemperature();
              BMP180_GetSample_Temperature=1;
            }
          }else{ // sensor was disconnected:
            #ifdef DEBUGPRINT
              Serial.println("Sensor was disconnected");
            #endif 
              Initialized_I2C[BMP180_ADDR]=0;
              BMP180_Millis_Delay=0;
              BMP180_GetSample_Temperature=0;
              BMP180_GetSample_Pressure=0;
              BMP180_Sample_Delay=0; 
          }
        }
    }else{
      BMP180_Millis_Delay=0;
      BMP180_GetSample_Temperature=0;
      BMP180_GetSample_Pressure=0;
      BMP180_Sample_Delay=0; 
      #ifdef DEBUGPRINT
        Serial.print("Device Not Initialized:  ");
        Serial.print("Addr 0x");
        Serial.print(MPU6050_ADDR,HEX);
        Serial.print("\r\n");
      #endif 
    }
  }


  //sample Slide Potentiometer 
  if (SLIDEPOT_Millis_Delay>0){   
    if (Initialized_I2C[SLIDEPOT_ADDR]){
      if (((TimeStamp-SLIDEPOT_Millis)>SLIDEPOT_Millis_Delay)||(SLIDEPOT_Millis>Time_Millis)){
        SLIDEPOT_Millis=TimeStamp;
        // Test connection:
        Wire.beginTransmission(SLIDEPOT_ADDR);
        if (Wire.endTransmission()==0){ // connection succes
          // Sample Slide potentiometer
          uint16_t Slide_Potentiometer_ADC=Read_I2C_ADC(SLIDEPOT_ADDR);
          #ifdef DEBUGPRINT
            Serial.print("Slide Potentiometer ADC \t");
            Serial.print(Slide_Potentiometer_ADC);
            Serial.println(TimeStamp);
          #endif
          StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;  // generate json  buffer
          // generate a message
          JsonObject& jsonResp = outputJsonBuffer.createObject();
          jsonResp["SAMPLE_ID"] = "MPU6050";
          jsonResp["COUNT"] = MPU6050_Count++;
          jsonResp["VAL"] = Slide_Potentiometer_ADC;
          
          jsonResp.printTo(SerialPi);
          SerialPi.println(); // must end with a '\n'
          
        }else{ // sensor was disconnected:
          #ifdef DEBUGPRINT
            Serial.println("Sensor was disconnected");
          #endif 
            Initialized_I2C[SLIDEPOT_ADDR]=0;
            SLIDEPOT_Millis_Delay=0;
        }
      }
    }else{
      SLIDEPOT_Millis_Delay=0;
      #ifdef DEBUGPRINT
        Serial.print("Device Not Initialized:  ");
        Serial.print("Addr 0x");
        Serial.print(SLIDEPOT_ADDR,HEX);
        Serial.print("\r\n");
      #endif 
    }
  }    
}// end Sample_Sensors

//Reset Sensors
void Resset_Sensors(){
  MPU6050_Millis_Delay=0;
  MPU6050_Count=0;
  
  BMP180_Millis_Delay=0;
  BMP180_Count=0;

  SLIDEPOT_Millis_Delay=0;
  SLIDEPOT_Count=0;

  Motor_stop();

  for(int ii=0;ii<NUMPIXELS;ii++){
    pixels.setPixelColor(ii, pixels.Color(0,0,0)); // Set pixel color. 
  }
  pixels.show(); // Updated pixel color Hardware.
} //end Reset Sensors

// Sum up message and send to Raspberry pi
void SendResponse(JsonObject& req, JsonObject& resp) {
  resp["REQ_ID"] = req.containsKey("REQ_ID") ? req["REQ_ID"] : "-1";
  resp.printTo(SerialPi);
  SerialPi.println(); // must end with a '\n'
}//end SendResponse


// Init I2C_ADC Device:
boolean Init_I2C_ADC(uint8_t I2C_ADC_Address){
  Wire.beginTransmission(I2C_ADC_Address);        // transmit to device
  Wire.write(ADC_REG_ADDR_CONFIG);                // Configuration Register
  Wire.write(ADC_REG_ADDR_CONV_INT);              // set ADC convertation interval to 3.4 ksps
  Wire.endTransmission();  

  // get data from device.
  Wire.beginTransmission(I2C_ADC_Address);        // transmit to device
  Wire.write(ADC_REG_ADDR_RESULT);                // get result
  if (Wire.endTransmission()==0){                 // connection success
    return 1;
  }else {
    return 0;
  }
}//end Init_I2C_ADC

//Read ADC Value
uint16_t Read_I2C_ADC(uint8_t I2C_ADC_Address){     //unsigned int *data
  
    uint16_t getData=0;
    Wire.requestFrom(I2C_ADC_Address, 2);           // request 2byte from device
    if(Wire.available()<=2)
    {
      getData = (Wire.read()&0x0f)<<8;
      getData |= Wire.read();
    }
    return getData; // range : 0-4095/2
}// end Read_I2C_ADC


