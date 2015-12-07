// Motor Control:
// input:  json:{"CMD":"MOTOR","MOVE":[<val_left>,<val_right>]}\n
// input:  json:{"CMD":"MOTOR","MOVE":[50,100]}\n
// output       {"RESP":"MOTOR","MOVE":[<val_left>,<val_right>]}\n

// LED update:
// input:  json:{"CMD":"LED","SET":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>]}\n
// input:  json:{"CMD":"LED","SET":[87,87,87,87,87,87,87,87]}\n
// input:  json:{"CMD":"LED","SET":[87,31,87,31,87,31,87,31]}\n
// output       {"RESP":"LED","COLOR":[<val>,<val>,<val>,<val>,<val>,<val>,<val>,<val>]}\n
