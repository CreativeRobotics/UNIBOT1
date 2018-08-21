#include <arduino.h>

//WHEEL values are the indexes for array storage and calling high level command functions
#define WHEEL1 0
#define WHEEL2 1
#define WHEEL3 2
#define WHEEL4 3

//wheel locations for driving
#define FRONT_LEFT  1
#define FRONT_RIGHT 2
#define REAR_LEFT   0
#define REAR_RIGHT  3

boolean getWheelFeedback = 1;
boolean echoMainSerial = 1;
boolean pause = 1;
boolean internalSafetyStatus = 0;
int mainBoardVoltage = 0;

boolean wheelMode = 0; //0 = Raw PWM, 1 = PID speed control
float robotSteeringSpeed = 1.0;
float diffSteeringRate = 0.0;
float robotSpeed = 0.0;
float crabAngle = 0.0;
float vectorDriveInternalX = 0.0;
float vectorDriveInternalY = 0.0;
//float vectorDriveRotation = 0.0;
float vectorDriveDistance = 0.0;
float vectorDriveAngle = 0.0;
float ackermannSteeringTurn = 0.0;
float ackermannAxlePos = 0.0;

float laserTargetAngle = 0.0;
float laserRangeReading = 0.0;

//long int wheelEncoderValues[4];
//int wheelCurrentDraw[4];
//int wheelSupplyVoltage[4];
//int wheelBoardTemps[4];
//const int wheelSteeringTrim[4] = {2,2,0,-18};
float wheelSteeringTrim[4] = {0.0, 0.0, 0.0, -4.2785};
int lastWheelCommError;
int sonarSensors[4];
#define SONAR1 0
#define SONAR2 1
#define SONAR3 2
#define SONAR4 3

boolean laserEnabled = 0;
#define LASER_RANGER 4

#define MAX_LASER_ANGLE 40
const int laserSamples = 8;
float laserValues[laserSamples+1];
float laserAngleStep = (MAX_LASER_ANGLE*2)/(float)laserSamples;

int robotPauseState = 1; //robot paused with power on
int changeRobotPauseState = 0; //This is set to 1 by a serial command to flag a pause state change
int serialReplyPacket = 1; //determines what packet to reply with when responding to motion commands (0 = just an ack)
unsigned char serialCommandMode = 0;
#define MAINBUFFERSIZE 256
const int mainOverflowLimit = MAINBUFFERSIZE-5;
char mainSerialBuffer[MAINBUFFERSIZE];
int mainWriteIndex;
boolean mainBufferOverflow = 0;


