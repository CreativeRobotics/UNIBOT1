#define MAX_VALUE 32767

#define KP_DEFAULT 10
#define KD_DEFAULT 6
#define KI_DEFAULT 2
int Kp = KP_DEFAULT, Kd = KD_DEFAULT, Ki = KI_DEFAULT;

int tempKd = Kd;
int integralSaturation = 100;
int divisor = 10;
int pidError;
int pidOldError;
int pidErrorDerivative = 0;
int pidErrorIntegral = 0;



int applyPID(int pidTargetSpeed, int pidCurrentSpeed){  
  long int sumVariable;
  static int reverserTime = 0;
  pidOldError          = pidError;
  pidError             = pidTargetSpeed - pidCurrentSpeed;
  pidErrorDerivative   = pidError - pidOldError;
  pidErrorIntegral     = constrain((pidErrorIntegral+pidError), -integralSaturation, integralSaturation);
  
  //ensure that the integral returns to zero when there is no error
  if(pidError == 0){
    if(pidErrorIntegral > 0)pidErrorIntegral--;
    else if(pidErrorIntegral < 0)pidErrorIntegral++;
  }
  //apply compensation for when reducing speed - motor braking causes the motor to slow much faster than it can speed up
  if( (pidTargetSpeed > 0 && pidTargetSpeed < pidCurrentSpeed) || (pidTargetSpeed < 0 && pidTargetSpeed > pidCurrentSpeed ) )tempKd = Kd/2;
  
  sumVariable          = pidError*Kp + pidErrorDerivative*tempKd + pidErrorIntegral*Ki;
  sumVariable         /= divisor;
  
  
  /*if(freewheelState == 0){
    Serial.print("Err,");
    Serial.print(pidError);
    
    Serial.print(", ErrDt,");
    Serial.print(pidErrorDerivative);
    
    Serial.print(", Erri,");
    Serial.print(pidErrorIntegral);
    
    Serial.print(", SUM,");
    Serial.print(sumVariable);
    
    
    Serial.print(", Kp,");
    Serial.print(Kp);
    
    Serial.print(", Kd,");
    Serial.print(Kd);
    
    Serial.print(", Ki,");
    Serial.print(Ki);
  }*/
  //apply braking when reversing
  //if reversing ...
  if( (pidTargetSpeed < 0 && pidCurrentSpeed > 0) || (pidTargetSpeed > 0 && pidCurrentSpeed < 0) ){
    //incriment timer
    reverserTime++;
    //has timer overflowed?
    if(reverserTime > 99){
      reverserTime = 99;
      //set normal PID mode and try to reverse using power
      return pidTargetSpeed + (int)constrain(sumVariable, -MAX_VALUE, MAX_VALUE);
    }
    //otherwise return zero for motor braking only
    return 0;
  }
  //if you get here then you are not reversing so apply normal power and zero the timer
  reverserTime = 0;
  return pidTargetSpeed + (int)constrain(sumVariable, -MAX_VALUE, MAX_VALUE);
}


void setKp(int value){
  if(value < 1000)  Kp = value;
  else Kp = 1000;
}

void setKd(int value){
  if(value < 1000)  Kd = value;
  else Kd = 1000;
}

void setKi(int value){
  if(value < 1000)  Ki = value;
  else Ki = 1000;
}

void setPIDDefaults(){
  Kp = KP_DEFAULT;
  Kd = KD_DEFAULT;
  Ki = KI_DEFAULT;
}

int getLastSpeedError(){
  return pidError;
}

