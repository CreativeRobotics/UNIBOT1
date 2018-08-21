//steering angles and speeds are 0-1023
#define MAX_STEERING_TRIES 4 //number of times to try when getting steering errors

void setsteeringTrim(){
  //copy trim data to robot object
  int n;
  for(n = 0; n < 4; n++){
    myUnibot.wheelUnit[n].wheelSteeringTrim = wheelSteeringTrim[n];
  }
}



int setSteeringPosition(int wheelNo, int steeringPosition, int steeringSpeed){
  unsigned char actID;
  int dynamixelErrorReturn;
  int expectedChecksumResult = 0;
  //wheels are numbered 0-3
  //steering actuator ID's are 1-4
  actID = wheelNo+1;
  expectedChecksumResult = (~(actID + 2))&0xFF;
  dynamixelErrorReturn = Dynamixel.moveSpeed( actID, steeringPosition, steeringSpeed);
  //Serial.print("DM=");
  //Serial.print(dynamixelErrorReturn);
  //Serial.print(" Chk=");
  //Serial.println(expectedChecksumResult);
  if(dynamixelErrorReturn < 0) return 1;
  if(expectedChecksumResult != dynamixelErrorReturn) return 2;
  return 0;
  
}


boolean setSteeringAngle(int wheelNo, float angle, float maxSpeed){
  //sets the steering using an angle in degrees (+ or -135 degrees)
  //300 degree range/1024 values
  //speed is in the range 0.0 - 1.0
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
    returnErr =  setSteeringPosition(wheelNo, (int)clippedAngle, sSpeed);
    n++;
    if(n == MAX_STEERING_TRIES) return returnErr; //0!
  }
  return returnErr;
}
