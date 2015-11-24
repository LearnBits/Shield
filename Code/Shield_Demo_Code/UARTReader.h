/*
  UARTReadBytes.h - Library for non-blocking read from UART comm
  Created by David Kadoucn, November 22, 2015
  Copyright Learnbits
*/

#ifndef UARTReader_h
#define UARTReader_h
#include <Arduino.h>

/* Structure of a single frame:
 *
 * delimiter + json message (max 512) + '\n' (1)
 * 
 */


/* 
 *  Buffer size including delimiters: 512 bytes 
 */
#define MAX_INPUT_BUFFER 512

class UARTReader
{
  public:
    UARTReader(Stream& serialPort, char *startDelim, char stopChar, int maxBufferSize=MAX_INPUT_BUFFER);
   ~UARTReader();
    int getPacket();
    char* buffer() { return inputBuffer; }

    static int const GOT_PACKET = 0;
    static int const SYNCING = 1;
    static int const RECEIVING = 2;
    static int const BUFF_OVERFLOW = 3;
    
  private:
    char* inputBuffer;
    char* startDelimiter; // = "json:";
    char stopCharacter; 
    
    int  mode  = SYNCING;
    int  index = 0; // static ptr for when data comes in separate chunks
    unsigned int  count = 0;
    
    Stream& serialPort;

};

#endif

