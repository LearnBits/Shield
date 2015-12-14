/*
 REMINDER
  Serial  = USB port to host
  Serial1 = UART to RPI
*/

#include <ArduinoJson.h>
#include "UARTReader.h"
#include "WaveBurstGenerator.h"


Serial_&        Serial2USB = Serial;
HardwareSerial& Serial2RPI = Serial1;

char           msgStopChar = '\n';
char         msgStartStr[] = "json:";
UARTReader uartReader(Serial2RPI, msgStartStr, msgStopChar);

boolean sampleMPU6050 = false;
WaveBurstGenerator* MPU6050gen[6];

const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

// MPU6050 Sensor variables
#define MPU6050_ADDR 0x68
int16_t MPU6050_VAL[6]={0,0,0,0,0,0};
unsigned long MPU6050NextTick = 0;
unsigned long MPU6050Period = 0;
// LED BAR
#define NUMPIXELS      8



void setup() {
  // initialize both serial ports:
  Serial2USB.begin(115200);
  Serial2RPI.begin(57600);

  MPU6050gen[0] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
  MPU6050gen[1] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
  MPU6050gen[2] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
  MPU6050gen[3] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
  MPU6050gen[4] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
  MPU6050gen[5] = new WaveBurstGenerator(15000 /*A*/, 17000 /*B*/, 
                                         1 /*w1min*/, 100 /*w1max*/, 1 /*w2min*/, 100 /*w2max*/,
                                         10 /*lowmin*/, 40 /*lowmax*/, 3 /*burstmin*/, 15 /*burstmax*/);
}

void loop() {

  // Get message:
  int ret_code = uartReader.getMessage();
  
  switch(ret_code) {
    case UARTReader::UART_GOT_MESSAGE:
      parseMessage();
      break;
    case UARTReader::UART_BUFF_OVERFLOW:
      Serial2USB.println("UART_BUFF_OVERFLOW");
      break;
  }
  
  now = millis();

  if(sampleMPU6050)
    getValuesMPU6050(now);
}

void getValuesMPU6050(unsigned long now) {
 
  StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
    
  if(MPU6050NextTick > now)
    // not the time
    return;

  // update next tick 
  MPU6050NextTick = now + MPU6050Period;

  int16_t MPU6050_VAL[6]={0,0,0,0,0,0};
  generateValues6(MPU6050_VAL);
  // generate a message
  JsonObject& jsonResp1 = outputJsonBuffer.createObject();
  jsonResp1["SENSOR"] = "MPU6050_ACCELEROMETER";
  JsonArray& arr1 = jsonResp1.createNestedArray("VAL");
  arr1.add(MPU6050_VAL[0]);
  arr1.add(MPU6050_VAL[1]);
  arr1.add(MPU6050_VAL[2]);
  sendMessage(jsonResp1);
  
  JsonObject& jsonResp2 = outputJsonBuffer.createObject();
  jsonResp2["SENSOR"] = "MPU6050_GYROSCOPE";
  JsonArray& arr2 = jsonResp2.createNestedArray("VAL");
  arr2.add(MPU6050_VAL[3]);
  arr2.add(MPU6050_VAL[4]);
  arr2.add(MPU6050_VAL[5]);
  sendMessage(jsonResp2);
}

void generateValues6(int16_t val[]) {
  for(int i = 0 ; i < 6 ; i++)
    val[i] = nextSample(i+1);
}

void sendMessage(JsonObject& jsonResp) {
  jsonResp.printTo(Serial2RPI);
  Serial2RPI.println(); // must end with a '\n'
}

void parseMessage(){
    // Json buffers must be declared locally so it's allocated on the stack automatically    
    StaticJsonBuffer<JSON_BUFFER_SIZE>  inputJsonBuffer; 
    StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
  
    // Parse cmd
    JsonObject& json = inputJsonBuffer.parseObject(uartReader.buffer());
    // Create Response object
    JsonObject& jsonResp = outputJsonBuffer.createObject();

    if(json.success()) {
      String CMD_Type=json["CMD"];
      
      if (CMD_Type=="MOTOR"){
        int Motor_Left_Val=json["MOVE"][0];
        int Motor_Right_Val=json["MOVE"][1];
        // send command to motors
        //move(0, Motor_Left_Val);
        //move(1, Motor_Left_Val);

        // generate a response
        jsonResp["RESP"] = "MOTOR";
        JsonArray& Move_Data = jsonResp.createNestedArray("MOVE");
        Move_Data.add(Motor_Left_Val);
        Move_Data.add(Motor_Right_Val);
        sendMessage(jsonResp);
      }
      
      if (CMD_Type=="LED"){
        //for (int ii=0; ii<NUMPIXELS; ii++){
        //  int RGB_Table_Index=json["SET"][ii];
        //  if ((RGB_Table_Index>=0)&&(RGB_Table_Index<RGB_TABLE_SIZE))
        //    pixels.setPixelColor(ii,RGB_TABLE[RGB_Table_Index]); // Set pixel color. 
        //}
        //pixels.show(); // Updated pixel color Hardware.

        // generate a response
        jsonResp["RESP"] = "LED";
        JsonArray& Color_Data = jsonResp.createNestedArray("COLOR");
        for (int ii=0; ii<NUMPIXELS; ii++){
          //uint32_t Color_Hex=pixels.getPixelColor(ii);
          Color_Data.add(json["SET"][ii]);
        }
        sendMessage(jsonResp);
      }

      if (CMD_Type=="SCAN"){
        //Scan_I2C();
        // generate a response
        jsonResp["RESP"] = "I2C";
        JsonArray& ADDR_I2C_Data = jsonResp.createNestedArray("ADDR");
        //for (int ii=0; ii<Num_I2C_Availble; ii++){
        //  ADDR_I2C_Data.add(Availble_I2C[ii]);
        //}
        ADDR_I2C_Data.add(MPU6050_ADDR); // Add the MPU to the 
        sendMessage(jsonResp);
      }

     if (CMD_Type=="MPU6050"){
        MPU6050Period = (unsigned long)json["MSEC"];
        sampleMPU6050 = (MPU6050Period > 0);
        if(sampleMPU6050 && MPU6050Period < 100) 
          MPU6050Period = 100;
        // generate a response

        jsonResp["RESP"] = "MPU6050";
        jsonResp["MSEC"] = MPU6050Period;
        sendMessage(jsonResp);
      }

    }// end json parse
}

