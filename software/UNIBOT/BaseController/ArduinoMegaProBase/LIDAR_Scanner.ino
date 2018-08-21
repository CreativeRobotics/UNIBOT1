//functions for controlling the laser rangefinder

#define MAX_LASER_TRIES 3


float readLaser(){
  return (float)analogRead(LASER_RANGER);
}


float setLaserGetDistance(float laserAngle, int delayTime){
  float analogResult;
  boolean sErr = 0;
  sErr = setLaserAngle(laserAngle, 1.0);
  //if(sErr) Serial.println("#>Serr");
  delay(delayTime);
  analogResult = (float)analogRead(LASER_RANGER);
  
  
  return analogResult/100;
}



int setLaserPosition(int laserPosition, int motionSpeed){
  unsigned char actID;
  int dynamixelErrorReturn;
  int expectedChecksumResult = 0;
  actID = 5;
  expectedChecksumResult = (~(actID + 2))&0xFF;
  dynamixelErrorReturn = Dynamixel.moveSpeed( actID, laserPosition, motionSpeed);
  //Serial.print("DM=");
  //Serial.print(dynamixelErrorReturn);
  //Serial.print(" Chk=");
  //Serial.println(expectedChecksumResult);
  if(dynamixelErrorReturn < 0) return 1;
  if(expectedChecksumResult != dynamixelErrorReturn) return 2;
  return 0;
  
}

boolean setLaserAngle(float angle, float maxSpeed){
  boolean returnErr = 1;
  int n = 0;
  float clippedAngle = 0;
  int sSpeed;
  sSpeed = (int)(maxSpeed*1023);
  if(sSpeed > 1023) sSpeed = 1023;
  if(sSpeed < 0) sSpeed = 0;
  clippedAngle = angle;
  if(clippedAngle > 150)clippedAngle = 150;
  if(clippedAngle < -150)clippedAngle = -150;
  clippedAngle+=150;
  clippedAngle *= 3.41;
  while(returnErr){
    returnErr =  setLaserPosition((int)clippedAngle, sSpeed);
    n++;
    if(n == MAX_LASER_TRIES) return returnErr;
  }
  return returnErr;
}

boolean initLaser(){
  
  return setLaserAngle(0.0, 0.5);
}

boolean laserScanStep(boolean starting){
  //if starting then setup otherwise step the process
  static float currentPosition;
  static int currentIndex = 0;
  if(starting){
    for(int n = 0; n < laserSamples; n++){
      laserValues[n] = 0.0;
    }
    currentPosition = -MAX_LASER_ANGLE;
    currentIndex = 0;
    return setLaserAngle(currentPosition, 1.0);
  }
  laserValues[currentIndex] = setLaserGetDistance(currentPosition, 10);
  currentPosition += laserAngleStep; 
  if(currentPosition >= MAX_LASER_ANGLE){
    laserAngleStep = -laserAngleStep;
    currentPosition = MAX_LASER_ANGLE;
  }
  if(currentPosition <= -MAX_LASER_ANGLE ){
    laserAngleStep = -laserAngleStep;
    currentPosition = -MAX_LASER_ANGLE;
  }
  currentIndex = (int)((currentPosition+MAX_LASER_ANGLE)/laserAngleStep);
  if(currentIndex > laserSamples) currentIndex = laserSamples;
  if(currentIndex < 0 ) currentIndex = 0;
}
