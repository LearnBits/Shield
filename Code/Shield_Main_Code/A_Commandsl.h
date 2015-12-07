// Motor Control:
// input:  json:{"CMD":"MOTOR","MOVE":[<val_left>,<val_right>]}\n
// input:  json:{"CMD":"MOTOR","MOVE":[50,100]}\n
// output       {"RESP":"MOTOR","MOVE":[<val_left>,<val_right>]}\n

// LED update:
// input:  json:{"CMD":"LED","SET":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>]}\n
// input:  json:{"CMD":"LED","SET":[87,87,87,87,87,87,87,87]}\n
// input:  json:{"CMD":"LED","SET":[87,31,87,31,87,31,87,31]}\n
// output       {"RESP":"LED","COLOR":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>]}\n

// Discover:
// input:  json:{"CMD":"DISC"}\n
// output       {"RESP":"I2C","ADDR":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>]}\n


// Sensor Setup Update:
// input:  json:{"CMD":"MPU6050","FREQ":<VAL_HZ>}\n
// input:  json:{"CMD":"MPU6050","MSEC":<Milli_Sec>}\n
// input:  json:{"CMD":"MPU6050","FREQ":5}\n
// input:  json:{"CMD":"MPU6050","MSEC":1000}\n
// output       {"RESP":"MPU6050","MSEC":<Milli_Sec>}\n

// output       {"SENSOR":"MPU6050","VAL":[<val>,<val>,<val>,<val>,<val>,<val>]\n
