//I2C Addresses:
//// I2C Devices ADDR:
//#define SlidePot_ADDR  0x50 // 80
//#define MPU6050_ADDR   0x68 // 104
//#define BMP180_ADDR    0x77 // 119

// Motor Control:
// input:  json:{"CMD":"MOTOR","MOVE":[<val_left>,<val_right>,"REQ_ID":<REQ_ID>]}\n
// input:  json:{"CMD":"MOTOR","MOVE":[50,100],"REQ_ID":80}\n
// output       {"RESP":"MOTOR","REQ_ID":<REQ_ID>}\n

// LED update:
// input:  json:{"CMD":"LED","SET":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>],"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"LED","SET":[87,87,87,87,87,87,87,87],"REQ_ID":80}\n
// input:  json:{"CMD":"LED","SET":[87,31,87,31,87,31,87,31],"REQ_ID":80}\n
// output       {"RESP":"LED","REQ_ID":<REQ_ID>}\n

// SCAN _ Discover:
// input:  json:{"CMD":"SCAN","REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"SCAN","REQ_ID":80}\n
// output       {"RESP":"SCAN","I2C_ADDR":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>],"REQ_ID":<REQ_ID>}\n

// Reset Sensors
// input:  json:{"CMD":"RESET","REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"RESET","REQ_ID":80}\n
// output       {"RESP":"RESET","REQ_ID":<REQ_ID>}\n

// Sensor Setup Update
//#define MPU6050_ADDR 0x68
// input:  json:{"CMD":"MPU6050","FREQ":<VAL_HZ>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"MPU6050","MSEC":<Milli_Sec>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"MPU6050","FREQ":5,"REQ_ID":70}\n
// input:  json:{"CMD":"MPU6050","MSEC":1000,"REQ_ID":70}\n
// output       {"RESP":"MPU6050","REQ_ID":<REQ_ID>}\n

// sensor stream update
// output       {"SAMPLE_ID":"MPU6050","COUNT":<VAL>,"VAL":[<Ax>,<Ay>,<Az>,<Gx>,<Gy>,<Gz>]}\n


// Sensor Setup Update:
//#define BMP180_ADDR 0x77
// input:  json:{"CMD":"BMP180","FREQ":<VAL_HZ>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"BMP180","MSEC":<Milli_Sec>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"BMP180","FREQ":5,"REQ_ID":70}\n
// input:  json:{"CMD":"BMP180","MSEC":1000,"REQ_ID":70}\n
// output       {"RESP":"BMP180","REQ_ID":<REQ_ID>}\n


// sensor stream update
// output       {"SENSOR":"BMP180","COUNT":<VAL>,"VAL":[<Temperature>,<Pressure>]\n

// Sensor Setup Update:
//#define SlidePot_ADDR 0x50 // 80
// input:  json:{"CMD":"SLIDEPOT","FREQ":<VAL_HZ>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"SLIDEPOT","MSEC":<Milli_Sec>,"REQ_ID":<REQ_ID>}\n
// input:  json:{"CMD":"SLIDEPOT","FREQ":5,"REQ_ID":70}\n
// input:  json:{"CMD":"SLIDEPOT","MSEC":1000,"REQ_ID":70}\n
// output       {"RESP":"SLIDEPOT","REQ_ID":<REQ_ID>}\n


// sensor stream update
// output       {"SENSOR":"SLIDEPOT","COUNT":<VAL>,"VAL":<ADC Value>\n
