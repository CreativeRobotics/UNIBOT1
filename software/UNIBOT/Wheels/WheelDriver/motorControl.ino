

void initialiseDriver(){
  
  setMotorFreewheel(freewheelState);
  Timer1.pwm(MOTOR_PWM1, 0);
  Timer1.pwm(MOTOR_PWM2, 0);
  //setMotorPwm(0);
  enableMotorDriver();
}


//=======================================================
void setMotorFreewheel(boolean state){
  //send 1 to freewheel (suspends motor operation)
  freewheelState = state;
  if(freewheelState == 0){
    digitalWrite(MOTOR_ND1, 1);
    digitalWrite(MOTOR_D1,  0);
  }
  else{
    digitalWrite(MOTOR_ND1, 0);
    digitalWrite(MOTOR_D1,  1);
  }
}

//=======================================================
void enableMotorDriver(){
  digitalWrite(MOTOR_ENABLE, 1);
}


//=======================================================
void disableMotorDriver(){
  digitalWrite(MOTOR_ENABLE, 0);
}


//=======================================================
void setMotorPwm(int mPower){
  //basic raw motor power mode
  int rawPower;
  rawPower = abs(mPower);
  if(rawPower > 1023) rawPower = 1023;
  if(mPower > 0){
    Timer1.setPwmDuty(MOTOR_PWM1, rawPower); 
    Timer1.setPwmDuty(MOTOR_PWM2, 0); 
  }
  else if(mPower < 0){
    Timer1.setPwmDuty(MOTOR_PWM2, rawPower); 
    Timer1.setPwmDuty(MOTOR_PWM1, 0); 
  }
  else{
    Timer1.setPwmDuty(MOTOR_PWM1, 0); 
    Timer1.setPwmDuty(MOTOR_PWM2, 0); 
  }
}


