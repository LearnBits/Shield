// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1/LC I2C Scanner
// 08Mar13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C master device which will scan the address space and report all
// devices which ACK.  It does not attempt to transfer data, it only reports which devices
// ACK their address.
//
// Pull the control pin low to initiate the scan.  Result will output to Serial.
//
// This example code is in the public domain.
// -------------------------------------------------------------------------------------------

#include <i2c_t3.h>

// Function prototypes
void print_scan_status(uint8_t target, bool all);

void setup()
{

    Serial.begin(57600);
    //I2C_PULLUP_EXT
    // Setup for Master mode, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
}

void loop()
{
    uint8_t target; // slave addr

    //
    // Test1/2 - scan I2C addresses
    //         - pull pin 12 low to show ACK only results
    //         - pull pin 11 low for a little more verbose result (shows both ACK and NACK)
    //
        Serial.print("---------------------------------------------------\r\n");
        static int iScan=0;
        Serial.print("Starting scan... ");
        Serial.print(iScan);
        Serial.print("\r\n");
        iScan++;
        for(target = 1; target <= 0x7F; target++) // sweep addr, skip general call
        {
            Wire.beginTransmission(target);       // slave addr
            Wire.endTransmission();               // no data, just addr
            print_scan_status(target, 0);
        }
        Serial.print("---------------------------------------------------\r\n");

        //delay(500); // delay to space out tests
}

//
// print scan status
//
void print_scan_status(uint8_t target, bool all)
{
    switch(Wire.status())
    {
    case I2C_WAITING:
        Serial.print("Addr 0x");
        Serial.print(target,HEX);
        Serial.print(" ACK\r\n");
        break;
    case I2C_ADDR_NAK:
        if(all)
        {
            Serial.print("Addr 0x");
            Serial.print(target,HEX);
            Serial.print("\r\n");
        }
        break;
    default:
        break;
    }
}
