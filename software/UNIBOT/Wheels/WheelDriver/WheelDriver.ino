#include <UniBotComms.h>

#include <TimerOne.h>
#include "globals.h"
#include "simpleSerialParsing.h"
//UniBot Power Wheel board
//Arduino Pro-Mini 5V
//Version 0.1A
//defines
//QUADRATURE
/*
Stripes per rev = 6
Ticks per motor rev at max resolution 24
Gearbox Ratio 51:1
Ticks per wheel revolution at max resolution 24*51=1224
Wheel diameter 150mm
Wheel Circumference 471.23mm
Approx ticks per mm = 1224/471=2.6
Ticks per degree = 1224/360=3.4

*/

//#define COMMDEBUG

long int loopTestTime = 0;

void setup(){
  initialiseBoard();
  currentTime = millis();
  lastTime = currentTime;
  //MOTOR IS INITIATED TO FREEWHEEL MODE - USE ENGAGE COMMAND TO GET IT MOVING!!!
}

//=======================================================
void loop(){
  if(newData){
    serialResult = processSerial();
    if(serialResult == SET_MOTOR_POWER || serialResult == HALT_MOTOR || serialResult == SET_MOTOR_POWER_GET_ENCODER || serialResult == SET_MOTOR_POWER_GET_FEEDBACK){
      switch (motorControlMode) {
        case RAW_POWER_MODE:
          rawMotorPower = constrain(newMotorPower, -1023, 1023);
          break;
        case PID_MODE:
          //impliment a PID function ...?
          motorSpeedTarget = constrain(newMotorPower, -1023, 1023);
          //setMotorPwm(rawMotorPower);
          break;
        default:
          break;
      }
    }    
  }

  currentTime = millis();
  if( (currentTime-lastTime) > controlLoopDeltaT){
    lastTime = currentTime;
    readSensors();
    if(supplyVoltage < 700) setSafeMode();
    if(supplyVoltage > 720 && safeMode) unsetSafeMode();
    if(!safeMode){
      calculateMotorSpeed();
      applyControlMode();
    }
  }
  checkResetIn();
 // sendACK();
 // delay(10);
}

void setSafeMode(){
  //if a problem is detected or the supply voltage is too low then disabe the motor and reset the motor power
  disableMotorDriver();
  freewheelState = 1;
  rawMotorPower = 0;
  motorSpeedTarget = 0;
  newMotorPower = 0;
  setMotorFreewheel(freewheelState);
  setMotorPwm(rawMotorPower);
  safeMode = 1;
}

void unsetSafeMode(){
  enableMotorDriver();
  safeMode = 0;
}
  
void calculateMotorSpeed(){
  elapsedCounts = qeiCounts - lastEncoderCount;
  elapsedMicros = micros()-lastEncoderTime;
  if(elapsedMicros > 65534){
    elapsedMicros = 65534;
  }
  elapsedMicros /= 64;
  lastEncoderCount = qeiCounts;
  motorSpeed = (abs(elapsedCounts)*1023)+(1023-elapsedMicros);
  if(!motorDirection) motorSpeed=-motorSpeed;
}


void applyControlMode(){
  switch (motorControlMode) {
    case RAW_POWER_MODE:
      //Do nothing apart from
      setMotorPwm(rawMotorPower);
      break;
    case PID_MODE:
      //the speed target and speed range are scaled to match the motor power range
      rawMotorPower = applyPID(motorSpeedTarget, motorSpeed/motorSpeedDivisor);
      setMotorPwm(rawMotorPower);
      break;
    default:
      break;
  }
}


//=======================================================
void readSensors(){
  float tempFloat = 0;
  //motorCurrent = analogRead(MOTOR_CURRENT_FB);
  motorCurrentAverager[mcIndex] = analogRead(MOTOR_CURRENT_FB);
  mcSum += motorCurrentAverager[mcIndex];
  if(mcIndex < CURRENT_SAMPLE_LENGTH-1) mcIndex++;
  else mcIndex = 0;
  mcSum -= motorCurrentAverager[mcIndex];
  motorCurrent = mcSum/10;
  
  supplyVoltage = analogRead(SUPPLY_VOLTAGE_FB);
  boardTemp = analogRead(ON_BOARD_THERMAL);
  tempFloat = (float)supplyVoltage;
  tempFloat*=VOLTAGE_MULTIPLIER;
  supplyVoltage = tempFloat;
  
}

void checkResetIn(){
  if(digitalRead(PULSE)) softReset();
}

void softReset(){
  Serial.end();
  asm volatile ("  jmp 0");
}

