#define MOTOR_PWM1   9
#define MOTOR_PWM2   10
#define MOTOR_D1     12
#define MOTOR_ND1    13
#define MOTOR_ENABLE 4
#define RS485TX_EN   7
#define RS485RX_EN   8
#define PULSE        5
#define CHAIN        6

#define QEI_A_PIN    3 //int1
#define QEI_B_PIN    2 //int0
#define QEI_IDX_PIN x
//Analog Pins:
#define MOTOR_DRIVER_FAULT_FB 0
#define MOTOR_CURRENT_FB 2
#define SUPPLY_VOLTAGE_FB 1
#define ON_BOARD_THERMAL 3

#define VOLTAGE_MULTIPLIER 2.7334852

long int lastEncoderTime = 0;
long int lastEncoderCount = 0;
long int elapsedCounts = 0;
long int elapsedMicros = 0;
long int qeiCounts = 0;
long int motorSpeed = 0;
boolean motorDirection = 0;
const int motorSpeedDivisor = 75;

boolean safeMode = 0;

int rawMotorPower = 0;
int motorSpeedTarget = 0;
boolean invertControls = 0; //for left/right sided wheels - inverts all incoming speed commands and outgoing encoder data
int newMotorPower = 0;
boolean freewheelState = 1;
//int motorControlMode = 0;
int iterator = 8;
int limit = 1024;
int timeCounter = 0;

#define CURRENT_SAMPLE_LENGTH 10
int motorCurrentAverager[CURRENT_SAMPLE_LENGTH];
int mcIndex = 0;
int mcSum = 0;
int motorCurrent = 0;
int supplyVoltage = 0;
int boardTemp = 0;


char serialResult = 0;
boolean newData = 0;


long currentTime, lastTime;
const int controlLoopDeltaT = 25; //The update frequency of the control loop in milliseconds

#define RAW_POWER_MODE 0
#define  PID_MODE 1
int motorControlMode = PID_MODE;

//PID speed control variables

