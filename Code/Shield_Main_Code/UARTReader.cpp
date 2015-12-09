#include "UARTReader.h"
#define DEBUGPRINT

UARTReader::UARTReader(Stream& serialPort, char *startDelimiter, char stopCharacter, int maxBufferSize) : serialPort(serialPort) {
  this->startDelimiter = new char[1 + strlen(startDelimiter)];
  strcpy(this->startDelimiter, startDelimiter);
	this->completed = strlen(startDelimiter);
  this->stopCharacter = stopCharacter;
  this->inputBuffer = new char[maxBufferSize];
}

UARTReader::~UARTReader() {
  delete this->startDelimiter;
  delete this->inputBuffer;
}

int UARTReader::getPacket() {

  int nBytes, i;
  char c;
  
  if(!(nBytes = serialPort.available()))
    return mode;

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
          }
          if(index == MAX_INPUT_BUFFER-1) { // overflow //
            index = 0;
            mode = SYNCING;
            return OVERFLOW;
          } 
        }
    }
  } // while //
  return mode; // SYNCING OR RECEIVING
}


