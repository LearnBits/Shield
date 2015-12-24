//I2C Addresses:
//// I2C Devices ADDR:
//#define MPU6050_ADDR 0x68
//#define BMP180_ADDR 0x77

// Motor Control:
// input:  json:{"CMD":"MOTOR","MOVE":[<val_left>,<val_right>,"ID":<ID>]}\n
// input:  json:{"CMD":"MOTOR","MOVE":[50,100],"ID":80}\n
// output       {"RESP":"MOTOR","ID":<ID>}\n

// LED update:
// input:  json:{"CMD":"LED","SET":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>],"ID":<ID>}\n
// input:  json:{"CMD":"LED","SET":[87,87,87,87,87,87,87,87],"ID":80}\n
// input:  json:{"CMD":"LED","SET":[87,31,87,31,87,31,87,31],"ID":80}\n
// output       {"RESP":"LED","ID":<ID>}\n

// SCAN _ Discover:
// input:  json:{"CMD":"SCAN","ID":<ID>}\n
// input:  json:{"CMD":"SCAN","ID":80}\n
// output       {"RESP":"SCAN","I2C_ADDR":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>],"ID":<ID>}\n

// Reset Sensors
// input:  json:{"CMD":"RESET","ID":<ID>}\n
// input:  json:{"CMD":"RESET","ID":80}\n
// output       {"RESP":"RESET","ID":<ID>}\n

// Sensor Setup Update
//#define MPU6050_ADDR 0x68
// input:  json:{"CMD":"MPU6050","FREQ":<VAL_HZ>,"ID":<ID>}\n
// input:  json:{"CMD":"MPU6050","MSEC":<Milli_Sec>,"ID":<ID>}\n
// input:  json:{"CMD":"MPU6050","FREQ":5,"ID":70}\n
// input:  json:{"CMD":"MPU6050","MSEC":1000,"ID":70}\n
// output       {"RESP":"MPU6050","ID":<ID>}\n

// sensor stream update
// output       {"SAMPLE_ID":"MPU6050","COUNT":<VAL>,"VALUES":[<Ax>,<Ay>,<Az>,<Gx>,<Gy>,<Gz>]}\n


// Sensor Setup Update:
//#define BMP180_ADDR 0x77
// input:  json:{"CMD":"BMP180","FREQ":<VAL_HZ>,"ID":<ID>}\n
// input:  json:{"CMD":"BMP180","MSEC":<Milli_Sec>,"ID":<ID>}\n
// input:  json:{"CMD":"BMP180","FREQ":5,"ID":70}\n
// input:  json:{"CMD":"BMP180","MSEC":1000,"ID":70}\n
// output       {"RESP":"BMP180","ID":<ID>}\n


// sensor stream update
// output       {"SENSOR":"BMP180","COUNT":<VAL>,"VAL":[<Temperature>,<Pressure>]\n
