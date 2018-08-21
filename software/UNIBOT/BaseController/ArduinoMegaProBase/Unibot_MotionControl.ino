//motion control functions
#define STEERINGSPEEDSCALE 5



int refreshMotion(){
  int commErrors = 0;
  int wheelCount;
  for(wheelCount = 0; wheelCount < 4; wheelCount++){
    commErrors += setWheelSpeedAngle(wheelCount,
                                      myUnibot.wheelUnit[wheelCount].wheelSpeed, 
                                      myUnibot.wheelUnit[wheelCount].steeringAngle,
                                      myUnibot.wheelUnit[wheelCount].steeringRate,
                                      getWheelFeedback );
  }
  return commErrors;
}


int refreshWheelData(){
  //update the wheel internal feedback data
  int commErrors = 0;
  int n;
  for(n = 0; n < 4; n++){
    commErrors += refreshWheelFeedback(n);
  }
  return commErrors;
}


boolean setWheelRPMAngle(int wheelNo, float wheelRPM, float steeringAngle, float steeringSpeed, boolean getFeedback){
  //sets the wheel speed in RPM*10 and the steering angle in degrees (+/-150 range)
  int comError = 0;
  int rpmTarget;
  rpmTarget = (int)(wheelRPM*1023);
  comError += setSteeringAngle(wheelNo, steeringAngle, steeringSpeed);
  comError += setWheelRPM(wheelNo, rpmTarget, getFeedback);
  return comError;
}



int setWheelSpeedAngle(int wheelNo, float wheelSpeed, float steeringAngle, float steeringSpeed, boolean getFeedback){
  //sets the wheel speed in RAW speed and the steering angle in degrees (+/-150 range)
  //returns number of errors
  int comError = 0;
  int rawSpeed;
  rawSpeed = (int)(wheelSpeed*1023);
  comError += setSteeringAngle(wheelNo, steeringAngle, steeringSpeed);
  //if(comError){
   //  Serial.print("DynamixelError: ");
   //  Serial.println(comError);
  //}
  //comError += setWheelSpeed(wheelNo, rawSpeed, getFeedback);
  comError += setRefreshWheel(wheelNo, rawSpeed);
  return comError;
}


