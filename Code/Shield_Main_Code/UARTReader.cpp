#include "UARTReader.h"
#define DEBUGPRINT

UARTReader::UARTReader(Stream& serialPort, char *startStr, char stopChar, int maxBufSize) : serialPort(serialPort) {
	this->UARTMode  = UART_SYNCING;
  this->bufIndex = 0; // static ptr for when data comes in separate chunks
  this->int syncSteps = 0;
  this->startStr = new char[1 + strlen(startStr)];
  strcpy(this->startStr, startStr);
	this->syncDone = (int)strlen(startStr);
  this->stopChar = stopChar;
  this->inputBuffer = new char[maxBufSize];
}

UARTReader::~UARTReader() {
  delete this->startStr;
  delete this->inputBuffer;
}

int UARTReader::getPacket() {

  int nBytes, bytesRead;
  char c;
  
  if(!(nBytes = serialPort.available()))
    return UARTMode;

<<<<<<< HEAD
  bytesRead = 0;
  while( bytesRead < nBytes ) {
    switch(UARTMode) {
      case UART_SYNCING:
        while(bytesRead < nBytes && UARTMode == UART_SYNCING) {
          c = (char)serialPort.read();
					bytesRead++;
          syncSteps = (c == startStr[syncSteps] ? syncSteps + 1 : 0);
          if(syncSteps == syncDone) {  // sync'ed //
            bufIndex = syncSteps = 0;
            UARTMode = UART_RECEIVING;
          }
        }
        break;
      case UART_RECEIVING:
        while(bytesRead < nBytes && UARTMode == UART_RECEIVING) {
          c = (char)serialPort.read();
          inputBuffer[bufIndex] = c;
					bytesRead++;
					bufIndex++;
          if(c == stopChar) {  // we got a packet!! //
            bufIndex = 0;
            UARTMode = UART_SYNCING;
            return UART_GOT_PACKET;
=======
  i = 0;
  while( i < nBytes ) {
    if(mode==SYNCING) {
        for( ; i < nBytes; i++) {
          c = (char)serialPort.read(); 
#ifdef DEBUGPRINT
            Serial.write(c);
#endif 
          stepCounter = (c == startDelimiter[stepCounter] ? stepCounter + 1 : 0);
          if(stepCounter == completed) {  // sync'ed //
            //debug("sync'ed");
#ifdef DEBUGPRINT
            Serial.write("Package");
#endif 
            index = stepCounter = 0;
            mode = RECEIVING;
            i++;
            break; // inner for loop //
          }
        }
    }else if(mode==RECEIVING){
        for( ; i < nBytes; i++) {
          c = (char)serialPort.read();
#ifdef DEBUGPRINT
            Serial.write(c);
#endif 
          inputBuffer[index++] = c;
          if(c == stopCharacter) {  // we got a packet!! //
            //debug("got a packet!!"); inputBuffer[index] = '\0'; debug(buf);
            index = 0;
            mode = SYNCING;
            return GOT_PACKET;
>>>>>>> origin/master
          }
          if(bufIndex == MAX_INPUT_BUFFER-1) { // overflow //
            bufIndex = 0;
            UARTMode = UART_SYNCING;
            return UART_OVERFLOW;
          } 
        }
<<<<<<< HEAD
      break;
    } // switch //
  } // while //
  return UARTMode; // UART_SYNCING or UART_RECEIVING
=======
    }
  } // while //
  return mode; // SYNCING OR RECEIVING
>>>>>>> origin/master
}


