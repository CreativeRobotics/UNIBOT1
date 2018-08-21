#define INTMAX 32,767



void initialiseQEI(){
  attachInterrupt(0, QEI_ISR_A, CHANGE);
  attachInterrupt(1, QEI_ISR_B, CHANGE);
  qeiCounts = 0;
}

void resetQEI(){
  qeiCounts = 0;
}


void QEI_ISR_A()
{
  //a fast(ish) QEI function
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(QEI_A_PIN);
  stateB = digitalRead(QEI_B_PIN);
  if(stateA){
    lastEncoderTime = micros();
    if(stateB){
      qeiCounts++;
      motorDirection = 1;
      return;
    }
    qeiCounts--;
    motorDirection = 0;
    return;
  }
  //high to low
  if(stateB){
    qeiCounts--;
    return;
  }
  qeiCounts++;
  return;
}

void QEI_ISR_B()
{
  //a fast(ish) QEI function
  boolean stateA = 0, stateB = 0;
  stateA = digitalRead(QEI_A_PIN);
  stateB = digitalRead(QEI_B_PIN);
  if(stateB){
    //low to high
    if(stateA){
      qeiCounts--;
      return;
    }
    qeiCounts++;
    return;
  }
  //high to low
  if(stateA){
    qeiCounts++;
    return;
  }
  qeiCounts--;
  return;
}

