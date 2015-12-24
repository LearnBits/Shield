// I2C ADC Devices from seeedstudio : http://www.seeedstudio.com/wiki/Grove_-_I2C_ADC
// Chip Datasheet: http://www.ti.com/lit/ds/symlink/adc121c021-q1.pdf

#ifndef I2C_ADC_H_
#define I2C_ADC_H_

/////////////////////////
//  Included Libraries //
/////////////////////////

/////////////////
//  Variables: //
/////////////////
// ADDR_ADC121             0x50-0x5A (9)

#define ADC_V_REF 3.00

#define ADC_REG_ADDR_RESULT         0x00
#define ADC_REG_ADDR_ALERT          0x01
#define ADC_REG_ADDR_CONFIG         0x02
#define ADC_REG_ADDR_LIMITL         0x03
#define ADC_REG_ADDR_LIMITH         0x04
#define ADC_REG_ADDR_HYST           0x05
#define ADC_REG_ADDR_CONVL          0x06
#define ADC_REG_ADDR_CONVH          0x07
#define ADC_REG_ADDR_CONV_INT       0x20 //3.4 ksps

///////////////////////
// Declare Functions //
///////////////////////

#endif /* I2C_ADC_H_ */
