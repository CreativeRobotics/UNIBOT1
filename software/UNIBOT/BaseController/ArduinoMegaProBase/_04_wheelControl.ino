//wheel control functions
//these use the basic wheel comm functions but add repeat tries when encountering errors
//Always use these functions of possible

//some useful definitions
#define WHEEL_TOP_SPEED_RAW_VALUE 2342 //the number of encoder 'ticks' every second at full power
#define WHEEL_CIRCUMFERENCE_MM 471
#define WHEEL_DIAMETER_MM 150
#define COUNTS_PER_REVOLUTION 1224
#define TOP_SPEED_RPM 128
#define SPEED_TO_RPM_MULTIPLIER 8

#define MAX_WHEEL_TRIES 4 //number of ties to try when getting comm errors
//*****Somethings not quite right here ...***
//#define TOP_SPEED_RPS 2
//#define speedDiv 75

//individual functions - each carries our one comm session to set or fetch a variable or state
//These functions try again of there is a comm error and save the last comm error value in a global variable
//Functions return 1 if they fail to communicate properly
//==========================================================================
boolean engageWheelMotor(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendEngageMotor(wID);
  while(lastWheelCommError) {
    lastWheelCommError = sendEngageMotor(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean disengageWheelMotor(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendDisengageMotor(wID);
  while(lastWheelCommError) {
    lastWheelCommError = sendDisengageMotor(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean setWheelSpeed(int wheelNo, int wheelSpeed, boolean getFeedback){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = setWheelMotorSpeed(wID, wheelSpeed, getFeedback);
  while(lastWheelCommError) {
    lastWheelCommError = setWheelMotorSpeed(wID, wheelSpeed, getFeedback);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean setRefreshWheel(int wheelNo, int wheelSpeed){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = setWheelRefreshSensors(wID, wheelSpeed);
  while(lastWheelCommError) {
    lastWheelCommError = setWheelRefreshSensors(wID, wheelSpeed);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean setPIDWheelMode(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendSetMode(wID, SET_PID_MODE);
  while(lastWheelCommError) {
    lastWheelCommError = sendSetMode(wID, SET_PID_MODE);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean setRAWWheelMode(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendSetMode(wID, SET_RAW_MODE);
  while(lastWheelCommError) {
    lastWheelCommError = sendSetMode(wID, SET_RAW_MODE);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}

//==========================================================================
boolean resetWheelEncoder(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendResetEncoder(wID);
  while(lastWheelCommError) {
    lastWheelCommError = sendResetEncoder(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean invertedWheelDirection(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendInvertDirection(wID, 1);
  while(lastWheelCommError) {
    lastWheelCommError = sendInvertDirection(wID, 1);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean normalWheelDirection(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = sendInvertDirection(wID, 0);
  while(lastWheelCommError) {
    lastWheelCommError = sendInvertDirection(wID, 0);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean refreshWheelCurrent(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = readWheelCurrent(wID);
  while(lastWheelCommError) {
    lastWheelCommError = readWheelCurrent(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean refreshWheelVoltage(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = readBoardVoltage(wID);
  while(lastWheelCommError) {
    lastWheelCommError = readBoardVoltage(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean refreshWheelBoardTemp(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = readBoardTemp(wID);
  while(lastWheelCommError) {
    lastWheelCommError = readBoardTemp(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}
//==========================================================================
boolean refreshWheelEncoder(int wheelNo){
  int n = 0;
  char wID;
  wID = getWheelIDChar(wheelNo);
  lastWheelCommError = readWheelEncoder(wID);
  while(lastWheelCommError) {
    lastWheelCommError = readWheelEncoder(wID);
    n++;
    if(n == MAX_WHEEL_TRIES) return 1;
  }
  if(lastWheelCommError) return 1;
  return 0;
}

//Group functions
//These call the functions above but use more than one - like reading all the wheel variables at once


int initialiseWheel(int wheelNo, int controlMode, boolean inverted, boolean resetEncoder){
  /*
  Control Mode 0 = Raw speed, 1 = PID control
  returns the number of comm errors
  */
  int comErr = 0;
  //zero the speed value
  comErr += setWheelSpeed(wheelNo, 0, 0);
  //set PID or normal mode
  if(controlMode) comErr += setPIDWheelMode(wheelNo);
  else comErr += setRAWWheelMode(wheelNo);
  if(resetEncoder) comErr += resetWheelEncoder(wheelNo);
  //engage the motor driver
  comErr += engageWheelMotor(wheelNo);
  if(inverted) comErr += invertedWheelDirection(wheelNo);
  else comErr += normalWheelDirection(wheelNo);
  return comErr;
}

//==========================================================================
int refreshWheelFeedback(int wheelNo){
  
  int comErr = 0;
  //returns the number of comm errors
  //Serial.print(" 1 ");
  comErr += refreshWheelEncoder(wheelNo);
  //Serial.print("2 ");
  comErr += refreshWheelCurrent(wheelNo);
  //Serial.print("3 ");
  comErr += refreshWheelBoardTemp(wheelNo);
  //Serial.print("4 ");
  comErr += refreshWheelVoltage(wheelNo);
  return comErr;
}



//==========================================================================
boolean setWheelRPM(int wheelNo, int wheelRPM, boolean getFeedback){
  //RPM
  /*
  speed value is 0-1023
  This maps to 0-120RPM approx - but ONLY when in PID mode
  */
  return setWheelSpeed(wheelNo, wheelRPM*SPEED_TO_RPM_MULTIPLIER, getFeedback);
}

//==========================================================================
int setWheelMode(boolean wMode){
  int returnErrors = 0;
  if(wMode == 0){
    for(int n = 0; n < 4; n++){
      returnErrors += setRAWWheelMode(n);
    }
  }
  else{
    for(int n = 0; n < 4; n++){
      returnErrors += setPIDWheelMode(n);
    }
  }
  return returnErrors;
}
