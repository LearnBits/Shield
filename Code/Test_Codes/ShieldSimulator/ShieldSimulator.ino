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
//Stream& inSerial  = Serial2RPI;
//Stream& outSerial = Serial2RPI;
//Stream& errSerial = Serial2RPI;
Stream& inSerial  = Serial2USB;
Stream& outSerial = Serial2USB;
Stream& errSerial = Serial2USB;


char           msgStopChar = '\n';
char         msgStartStr[] = "json:";
UARTReader uartReader(inSerial, msgStartStr, msgStopChar);

boolean sampleMPU6050 = false;
WaveBurstGenerator* MPU6050gen[6];

const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

// MPU6050 Sensor variables
#define MPU6050_ADDR 0x68
int16_t MPU6050_VAL[6]={0,0,0,0,0,0};
unsigned long MPU6050NextTick = 0;
unsigned long MPU6050Period = 0;
unsigned long MPU6050Count = 0;
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
      errSerial.println("UART_BUFF_OVERFLOW");
      break;
  }
  
  unsigned long now = millis();

  if(sampleMPU6050)
    getValuesMPU6050(now);
}

void getValuesMPU6050(unsigned long now) {
 
  StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
    
  if(MPU6050NextTick > now)
    // It's not the time
        // NOTE: in case of overflow in MPU6050NextTic,
        // this condition will be true during MPU6050Period ms,
        // until now will overflow as well
    return;

  // update next tick 
  MPU6050NextTick = now + MPU6050Period;

  // generate a message
  JsonObject& resp = outputJsonBuffer.createObject();
  resp["SAMPLE_ID"] = "MPU6050";
  resp["COUNT"] = MPU6050Count++;
  JsonArray& jsonArr = resp.createNestedArray("VALUES");
  for(int i = 0 ; i < 6 ; i++)
    jsonArr.add(MPU6050gen[i]->next());
  sendMessage(resp);
}


void sendMessage(JsonObject& resp) {
  resp.printTo(outSerial);
  outSerial.println(); // must end with a '\n'
}

void ack(JsonObject& req, JsonObject& resp) {
  resp["RESP"] = req["CMD"];
  resp["REQ_ID"] = req.containsKey("REQ_ID") ? req["REQ_ID"] : "-1";
  sendMessage(resp);
}

void parseMessage(){
    // Json buffers must be declared locally so it's allocated on the stack automatically    
    StaticJsonBuffer<JSON_BUFFER_SIZE>  inputJsonBuffer; 
    StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
  
    // Parse cmd
    JsonObject& req = inputJsonBuffer.parseObject(uartReader.buffer());
    // Create Response object
    JsonObject& resp = outputJsonBuffer.createObject();

    if(req.success()) {
      String CMD_Type=req["CMD"];

      if(CMD_Type=="RESET") {
        sampleMPU6050 = false;
        // generate a response
        ack(req, resp);
      }
      
      if (CMD_Type=="MOTOR"){
        int Motor_Left_Val=req["MOVE"][0];
        int Motor_Right_Val=req["MOVE"][1];
        // send command to motors
        //move(0, Motor_Left_Val);
        //move(1, Motor_Left_Val);

        // generate a response
        ack(req, resp);
      }
      
      if (CMD_Type=="LED"){
        //for (int ii=0; ii<NUMPIXELS; ii++){
        //  int RGB_Table_Index=req["SET"][ii];
        //  if ((RGB_Table_Index>=0)&&(RGB_Table_Index<RGB_TABLE_SIZE))
        //    pixels.setPixelColor(ii,RGB_TABLE[RGB_Table_Index]); // Set pixel color. 
        //}
        //pixels.show(); // Updated pixel color Hardware.

        // generate a response
        ack(req, resp);
      }

      if (CMD_Type=="SCAN"){
        //Scan_I2C();
        // generate a response
        JsonArray& ADDR_I2C_Data = resp.createNestedArray("I2C_ADDR");
        //for (int ii=0; ii<Num_I2C_Availble; ii++){
        //  ADDR_I2C_Data.add(Availble_I2C[ii]);
        //}
        ADDR_I2C_Data.add(MPU6050_ADDR); // Add the MPU to the 
        ack(req, resp);
      }

     if (CMD_Type=="MPU6050"){
        MPU6050Period = (unsigned long)req["MSEC"];
        sampleMPU6050 = (MPU6050Period > 0);
        if(sampleMPU6050 && MPU6050Period < 100) 
          MPU6050Period = 100;
        // generate a response
        ack(req, resp);
      }

    }// end json parse
}

