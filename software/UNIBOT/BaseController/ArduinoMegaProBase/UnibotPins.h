#include <arduino.h>

#define SAFETY_ON 1
#define SAFETY_OFF 0
#define SAFETY1PIN 38
#define SAFETY1_BIT 0
#define SAFETY2PIN 39
#define SAFETY2_BIT 1
#define SAFETY3PIN 40
#define SAFETY3_BIT 2
#define SAFETY4PIN 41
#define SAFETY4_BIT 3

#define SERVO1PIN 42
#define SERVO1PIN 43
#define SERVO1PIN 47
#define SERVO1PIN 48

#define PLED1  4
#define PLED2  5
#define PLED3  6
#define PLED4  7

#define PWMPIN1 8
#define PWMPIN2 9
#define PWMPIN3 10
#define PWMPIN4 11
#define PWMPIN5 12
#define PWMPIN6 13

#define MAINPOWERVOLTAGE_ADC 15

#define RS485_RECIEVER_ENABLE     0
#define RS485_RECIEVER_DISABLE    1
#define RS485_DRIVER_ENABLE  1
#define RS485_DRIVER_DISABLE 0

#define RS485_DMX_RECIEVER_ENABLE   22
#define RS485_DMX_DRIVER_ENABLE     23

#define RS485_WHEEL_RECIEVER_ENABLE 24
#define RS485_WHEEL_DRIVER_ENABLE   25

#define DYNAMIXEL_SERIAL_PORT 3
#define DYNAMIXEL_COMM        Serial3
//#define DYNAMIXEL_COMM_EVENT      serialEvent3

#define WHEEL_SERIAL_PORT     2
#define WHEEL_COMM            Serial2
//#define WHEEL_COMM_EVENT      serialEvent2

#define AUX_SERIAL_PORT       1
#define AUX_COMM              Serial1
//#define AUX_COMM_EVENT      serialEvent1
#define MAIN_SERIAL_PORT      0
#define MAIN_COMM             Serial
//#define MAIN_COMM_EVENT      serialEvent

#define RAZOR_DTR      34

#define CHAIN1         35
#define SNNAP_LINK     36
#define SNNAP_PULSE    37


//ADC
#define MAIN_VOLTAGE_SENSOR 15


//Wheel4 (PCB Motor1)
#define MOTOR4_PULSE 27
#define MOTOR4_CHAIN 26

//Wheel3 (PCB Motor2)
#define MOTOR3_PULSE 29
#define MOTOR3_CHAIN 28

//Wheel2 (PCB Motor3)
#define MOTOR2_PULSE 31
#define MOTOR2_CHAIN 30

//Wheel1 (PCB Motor4)
#define MOTOR1_PULSE 33
#define MOTOR1_CHAIN 32

