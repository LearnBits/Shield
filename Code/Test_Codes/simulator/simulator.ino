/*
 REMINDER
  Serial  = USB port to host
  Serial1 = UART to RPI
*/

#include <ArduinoJson.h>
#include "UARTReader.h"

HardwareSerial& Serial2RPI = Serial1;
Serial_&        Serial2USB = Serial;
char           msgStopChar = '\n';
char         msgStartStr[] = "json:";
UARTReader uartReader(Serial2Pi, msgStartStr, msgStopChar);



void setup() {
  // initialize both serial ports:
  Serial2USB.begin(115200);
  Serial2RPI.begin(57600);
}

void loop() {

  // Get message:
  int ret_code = uartReader.getMessage();
  
  // if message complete Parse message:
  if(ret_code == UARTReader::GOT_PACKET) {
    Parser_MSG();
  }
  if(ret_code == UARTReader::UART_BUFF_OVERFLOW) Serial.println("UART_BUFF_OVERFLOW");

  
  dtostrf(x, 7, 4, s);
  Serial.println(s);
  count = (count + 1) % 20;
  x = (float)count;
  delay(150 + 50 * (random(15)-2));
}

/*
 * float period = 5;
float A = 1.0;
float laps = 0.0;
float x = 0.0;
float f = 1.3;
char s[16];
int pause = 0;
int count = 0;
 */
/*
void loop() {
  
  if(!pause) {
    dtostrf(x, 7, 4, s);
    Serial1.println(s);
    delay(200);
    laps += 0.5;
    if(laps >= period) {
      laps = 0.0;
      x = abs(x - A) + 0.00001;
    }
  }

  if (Serial.available()) {
    int c = Serial.read();
    switch(c) {
      case 'u': A = A * f; break;
      case 'd': A = A / f; break;
      case 's': period = period * f; break;
      case 'f': period = period / f; if(period < 1.0) period=1.0; break;
      case 'p': pause = 1 - pause;
    }
    if(x > 0.00001) x = A;  
  }
}
*/
