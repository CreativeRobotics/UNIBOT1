//safety system for UNIBot

void startupCheck(){
  //returns when the startup checks have completed
  //check that there is enough power
  if(!powerStatusCheck()) setSafetyState(1);
  //get the full safety status and return if zero
  if(!getSafetyStatus()) {
    Serial.println("#>Starting ...");
    setSafetyState(0);
    delay(1000);
    if( !initStartingPosition() ){
      delay(100);
      return;
    }
  }
  Serial.print("#>Safe Mode Activated: ");
  Serial.println(getSafetyStatus());
  //engage safe mode
  while(getSafetyStatus()) {
    if(powerStatusCheck()) setSafetyState(0);
    delay(10);//wait here
    checkMainSerial(); //check it and allow the function to respond but do nothing else
  }
  setSafetyState(0);
  
  delay(1000);
  Serial.println("#>Starting ...");
  if( !initStartingPosition() ){
    delay(100);
    return;
  }
  setSafetyState(1);
}


void safetyCheck(){
  //Serial.println("Checking");
  //Check that the main battery is over 9V
  if(!powerStatusCheck()) setSafetyState(1);
  //get the full safety status and return if zero
  if(!getSafetyStatus()) return;
  Serial.print("#>Safe Mode Activated: ");
  Serial.println(getSafetyStatus());
  //engage safe mode
  while(getSafetyStatus()) {
    delay(10);//wait here
    if(checkMainSerial())doComms(); //check it and allow the function to respond but do nothing else
  }
  setSafetyState(0);
  delay(1000);
  Serial.println("#>Re-Initialising");
  if( !initStartingPosition() ){
    delay(100);
    return;
  }
  setSafetyState(1);
}

byte getSafetyStatus(){
  //returns a bit field of the three safety inputs plus its own status
  byte sStatus = 0;
  //internal safety state is active high (Safety is active if it is == 1)
  if(internalSafetyStatus == 1)  bitSet(sStatus, SAFETY1_BIT);
  //External Safeties are active LOW (If the pin is LOW then safe state is active)
  if(!digitalRead(SAFETY2PIN)) bitSet(sStatus, SAFETY2_BIT);
  if(!digitalRead(SAFETY3PIN)) bitSet(sStatus, SAFETY3_BIT);
  if(!digitalRead(SAFETY4PIN)) bitSet(sStatus, SAFETY4_BIT);
  return sStatus;
}

void setSafetyState(boolean newSafeState){
  if(newSafeState == 1){
    digitalWrite(SAFETY1PIN, 0);
    internalSafetyStatus = SAFETY_ON;
  }
  else{
    digitalWrite(SAFETY1PIN, 1);
    internalSafetyStatus = SAFETY_OFF;
  }
}


boolean powerStatusCheck(){
  //return 0 if main power is OFF
  readMainBoardSensors();
  //Serial.print("Board V=");
  //Serial.println(mainBoardVoltage);
  if(mainBoardVoltage < 900) return 0;
  return 1;
}
