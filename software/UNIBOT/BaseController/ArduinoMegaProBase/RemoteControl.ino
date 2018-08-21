#define RC_POWERMAX 500

/*int doRC(){
  //collect pulses for a while and average
  int commErrors = 0;
  globalWheelSpeed = 0;
  globalSteeringAngle = 0;
  turningAngle = 0;
  
  for(n = 0; n < 20; n++){
    globalWheelSpeed += -getScaledPulse(1);
    globalSteeringAngle += (float)getScaledPulse(0);
    turningAngle += (float)getScaledPulse(2);
    delay(5);
  }
  globalWheelSpeed/=20;
  globalSteeringAngle/=20;
  turningAngle/=20;
  
  if(globalWheelSpeed < -RC_POWERMAX || globalWheelSpeed > RC_POWERMAX) globalWheelSpeed = 0;
  
  if(globalSteeringAngle < -RC_POWERMAX || globalSteeringAngle > RC_POWERMAX) globalSteeringAngle = 0.0;
  if(turningAngle < -RC_POWERMAX || turningAngle > RC_POWERMAX) turningAngle = 0.0;
  
  

  if(globalWheelSpeed < 100 && globalWheelSpeed > -100) globalWheelSpeed = 0;
*/

  /*Serial.print("Speed: ");
  Serial.print(globalWheelSpeed);
  Serial.print(" Steering: ");
  Serial.print(globalSteeringAngle);
  Serial.print(" Turning: ");
  Serial.print(turningAngle);
  Serial.print(" Switch: ");
  Serial.println(getScaledPulse(3));*/
  /*
  if(getScaledPulse(3) > 100) spinOnTheSpot(-((int)turningAngle*3));
  else applyHoloSteering(turningAngle/12, globalWheelSpeed*3, globalSteeringAngle/5);
  commErrors = refreshMotion();
  //if(commErrors) printCommError(1, commErrors);
  return commErrors;
}*/
