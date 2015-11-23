#include "UARTReader.h"


UARTReader::UARTReader(Stream& serialPort, char *startDelimiter, char stopCharacter) : serialPort(serialPort) {
  this->startDelimiter = new char[1 + strlen(startDelimiter)];
  strcpy(this->startDelimiter, startDelimiter);
  this->stopCharacter = stopCharacter;
}

int UARTReader::getPacket() {

  int nBytes, i;
  char c;
  
  if(!(nBytes = serialPort.available()))
    return mode;

  i = 0;
  while( i < nBytes ) {
    switch(mode) {
      case SYNCING:
        for( ; i < nBytes; i++) {
          c = (char)serialPort.read();
          count = (c == startDelimiter[count] ? count + 1 : 0);
          if(count == strlen(startDelimiter)) {  // sync'ed //
            //debug("sync'ed");
            index = count = 0;
            mode = RECEIVING;
            break; // inner for loop //
          }
        }
        break;
      case RECEIVING:
        for( ; i < nBytes; i++) {
          c = (char)serialPort.read();
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
      break;
    } // switch //
  } // for //
  return mode; // SYNCING OR RECEIVING
}


