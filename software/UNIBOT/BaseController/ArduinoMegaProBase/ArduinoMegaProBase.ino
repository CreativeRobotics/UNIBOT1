#include <DynamixelSerial3.h>
#include <UniBotComms.h>
#include <UniBotCommands.h>
#include <UnibotWheel.h>
#include <UniBot.h>

#include "UnibotPins.h"
#include "UnibotGlobals.h"
#include "simpleSerialParsing.h"

int timerup1 = 0;
UniBotclass myUnibot; 

int updateLaserScan = 0;
//==========================================================================
void setup(){
  initialiseController();
  startupCheck();
  Serial.println("#>Startup OK!");
}

//==========================================================================
void loop(){

  long int elapsedTime = 0;
  int serialResult;

  safetyCheck();

  serialResult = checkMainSerial(); //returns 1 if new data has arrived
  if(serialResult)doComms(); //this processes anything except drive commands
  if(AUX_COMM.available()){
    while(AUX_COMM.available()){
      //copy incoming chars to the main serial port
      Serial.write(AUX_COMM.read());
    }
  }
  readAnalogSensors();
  //Check the pause state:
  //If 0 then you are running
  //if 1 then halt motor activity but keep everything the same
  switch(robotPauseState){
    case 0:
      if(changeRobotPauseState){
        for(int n = 0; n < 4; n++){
          engageWheelMotor(n);
        }
        changeRobotPauseState = 0;
      }
      doAction();
      
      if(updateLaserScan == 0){
        if(laserEnabled) laserScanStep(0);
        updateLaserScan = 4;
      }
      updateLaserScan--;
      
      break;
    case 1:
      if(changeRobotPauseState){
        for(int n = 0; n < 4; n++){
          disengageWheelMotor(n);
        }
        changeRobotPauseState = 0;
      }
      break;
    case 2:
      if(changeRobotPauseState){
        //for(int n = 0; n < 4; n++){
          //resetWheel(n);
        //}
        changeRobotPauseState = 0;
      }
      //Reset each wheel
      break;
    default:
      if(changeRobotPauseState){
        for(int n = 0; n < 4; n++){
          disengageWheelMotor(n);
        }
        changeRobotPauseState = 0;
      }
      break;
  }
  elapsedTime = millis();

  delay(10);

}
//==========================================================================
void doAction(){
  //Use the serial command to run a control function
  int commErr = 0;
  
  //Ignore serial commands that are not about drive modes
  switch(serialCommandMode){
    case COMMAND_DIFFDRIVE:
      myUnibot.diffDrive(robotSpeed, diffSteeringRate);
      serialCommandMode = COMPLETED;
      break;
    case COMMAND_DIFFDRIVE_IN:
      myUnibot.diffSpinDrive(robotSpeed, diffSteeringRate);
      serialCommandMode = COMPLETED;
      
      break;
    case COMMAND_ACKERMANN_VARIABLE:
      myUnibot.ackermannVariable(robotSpeed, ackermannSteeringTurn);
      serialCommandMode = COMPLETED;
      break;
    case COMMAND_REVERSE_ACKERMANN:
      myUnibot.ackermannFixed(robotSpeed, ackermannSteeringTurn, 0);
      serialCommandMode = COMPLETED;
      break;
    case COMMAND_ACKERMANN:
      myUnibot.ackermannFixed(robotSpeed, ackermannSteeringTurn, 1);
      serialCommandMode = COMPLETED;
      break;
    case COMMAND_VECTOR_DRIVE:
      myUnibot.vectorDrive(robotSpeed, vectorDriveDistance, vectorDriveAngle);
      serialCommandMode = COMPLETED;
      break;
    case COMMAND_VECTOR_DRIVE_IN:
      myUnibot.vectorSpinDrive(robotSpeed, vectorDriveInternalX, vectorDriveInternalY);
      serialCommandMode = COMPLETED;
      break;
    case START_SCANNING:
      laserEnabled = 1;
      laserScanStep(1);
      serialCommandMode = COMPLETED;
      break;
  }
  commErr = refreshMotion();
  if(commErr !=0) {
    Serial.print("#>Wheel Comm Error:");
    Serial.println(commErr);
  }
}
//==========================================================================
void doComms(){
    //Ignore serial commands that are not about drive modes
  switch(serialCommandMode){
    case SET_AXLE_POSITION:
      myUnibot.setAxle(ackermannAxlePos);
      serialCommandMode = COMPLETED;
      break;
    case SET_STEERING_SPEED:
      for(int n = 0; n < 4; n++){
        myUnibot.wheelUnit[n].steeringRate = robotSteeringSpeed;
        serialCommandMode = COMPLETED;
      }
      break;
    case READ_IMU:
      sendReadIMU();
      serialCommandMode = COMPLETED;
      break;
    case SET_WHEEL_MODE:
      //set the wheel mode for each wheel
      setWheelMode(wheelMode);
      serialCommandMode = COMPLETED;
      break;
  }
}
//==========================================================================
void wait(int delayTime){
  unsigned long currentMillis;
  currentMillis = millis();
  while(millis() < currentMillis+delayTime){}
}

void sendReadIMU(){
  AUX_COMM.println("#I");
}

