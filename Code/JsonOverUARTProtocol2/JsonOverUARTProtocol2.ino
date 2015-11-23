#include <ArduinoJson.h>
#include "UARTReader.h"



/* 
 *  Static Memory allocation for Json Parser
 * 
 *  Memory for all objects:        4 + 8 * N
 *  
 *  Memory for all Array elements: 4 + 10 * N
 *  
 */
const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(16) + JSON_ARRAY_SIZE(16);

//Serial_&        Serial = Serial;
//HardwareSerial& uart2RPI = Serial1;
UARTReader uartReader(Serial, (char*)"json:", '\n');


void setup() {
  
  // delay(5000); <--needed for some boards (like Teensy)
  Serial.begin(57600); // USB serial port (from Mac)
  Serial1.begin(57600);// UART to RPI
}

// sample usage
// input:  json:{"cmd":5,"symbol":"#"}\n
// output       {"code":"#####"}


void loop() {

  int ret_code = uartReader.getPacket();
  
  if(ret_code == UARTReader::GOT_PACKET) {
    // Json buffers must be declared locally so it's allocated on the stack automatically    
    StaticJsonBuffer<JSON_BUFFER_SIZE>  inputJsonBuffer; 
    StaticJsonBuffer<JSON_BUFFER_SIZE> outputJsonBuffer;
  
    // Parse cmd
    JsonObject& json = inputJsonBuffer.parseObject(uartReader.buffer());
    if(json.success()) {
      int n = json["cmd"];
      char sym = ((const char*)json["symbol"])[0];
      Serial.print(sym);Serial.print(' ');Serial.println(n);

      JsonObject& jsonResp = outputJsonBuffer.createObject();
      jsonResp["code"] = replicateSymbol(sym, n);
      jsonResp.printTo(Serial);
      Serial.println(); // must end with a '\n'
    }
  }
  
  if(ret_code == UARTReader::BUFF_OVERFLOW)
    blinkLED();

}

const char* replicateSymbol(char symbol, int n) {
  static char resp[50];
  for(int i=0; i<n && i<50; i++)
    resp[i] = symbol;
  resp[n] = '\0';
  return resp;
}

#define LED_PIN 13
void blinkLED() {
  pinMode(LED_PIN, OUTPUT);
  while(1) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}

void debug(int n) {
  Serial.println(n);
  Serial.flush();
}
void debug(char *s) {
  Serial.println(s);
  Serial.flush();
}

