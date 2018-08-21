

int checkMainSerial(){
  //empty the arduino serial buffer and look for a packet of data
  int bytesRead = 0;
  boolean parseError = 0;
  boolean mainBufferNewData = 0;
  if(Serial.available() == 0) return 0;
  while(Serial.available()){
    //Serial.print("[1]");
    mainBufferNewData = processSerial( (unsigned char)Serial.read() );
    if(mainBufferNewData) break;
    //Serial.print("Returned:");
    //Serial.println(mainBufferNewData);
  }
  if(mainBufferNewData){
    parseError = parseMainSerialBuffer();
    resetMainSerialBuffer();
    if(parseError) return 0;
    return 1;
  }
  return 0;
}

boolean processSerial(unsigned char incomingByte){
  static int readState = 0;
  boolean newDataFlag = 0;
  //Serial.write(incomingByte);
  
  switch (readState){
    case 0:
      if(incomingByte == MAINPACKETSTART){
        readState++;
        //Serial.println("PKT START");
      }
      break;
    case 1:
      //Serial.print("STATE 1 ");
      if(incomingByte == NEWLINE || incomingByte == CARRAIGERETURN){
        readState = 0; //no data so ignore
        //Serial.println("RESET");
      }
      else{
        //Serial.print("ADDING1:");
        mainSerialBuffer[mainWriteIndex] = incomingByte;
        mainWriteIndex++;
        //Serial.write(incomingByte);
        if(mainWriteIndex >mainOverflowLimit) mainBufferOverflow = 1;
        readState++;
        //data was received
      }
      break;
    case 2:
      //Serial.print("STATE 2 ");
      if(incomingByte == NEWLINE || incomingByte == CARRAIGERETURN){
         mainSerialBuffer[mainWriteIndex] = incomingByte;
         mainWriteIndex++;
         if(mainWriteIndex >mainOverflowLimit) mainBufferOverflow = 1;
        //end of packet received
        //Serial.println("END");
        readState = 0; //reset the buffer
        newDataFlag = 1; //return flag for new data in buffer
      }
      else{
        //Serial.print("ADDING2:");
        mainSerialBuffer[mainWriteIndex] = incomingByte;
        mainWriteIndex++;
        if(mainWriteIndex >mainOverflowLimit) mainBufferOverflow = 1;
        //Serial.write(incomingByte);
        //data was received, add to buffer
      }
      break;
    default:
      //Serial.println("DEFAULT");c
      readState = 0; //reset the buffer
      newDataFlag = 0;
      resetMainSerialBuffer();
      break;
    }
    return newDataFlag;
}

//#define MAINSERIALDEBGUG
boolean parseMainSerialBuffer(){
  //a new data line has been recieved
  //the first char will be the command
  //the last char will be a newline or CR
  //return 1 if fail
  //String tempData;
  char scommand;
  int readIndex = 0;
  float dataBuffer[8];
  int numberExtracted = 0;
  
  int tempVar = -1;
  //boolean failStatus = 0;
  //command = mainSerialBuffer[readIndex];
  scommand = mainSerialBuffer[readIndex];
  //Serial.print("String:");
  //Serial.write(scommand);
  readIndex++;
  
  //Serial.println("Parsing");
  //if there is only one char in the buffer and it isn't a PING request, return an error
  if( mainWriteIndex < 2) return 1;
  
  switch (scommand){
    case REQUEST_PING:
    
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>Ping");
        #endif
      
      sendAck();
      break;
      
    case COMMAND_DIFFDRIVE:
      serialCommandMode = COMMAND_DIFFDRIVE;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>Diff Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 3, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) diffSteeringRate = dataBuffer[1];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_DIFFDRIVE_IN:
      serialCommandMode = COMMAND_DIFFDRIVE_IN;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>Diff IN Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 3, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) diffSteeringRate = dataBuffer[1];
      if(numberExtracted > 2) crabAngle = dataBuffer[2];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_ACKERMANN_VARIABLE:
      serialCommandMode = COMMAND_ACKERMANN_VARIABLE;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>ACKERMAN Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 2, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) ackermannSteeringTurn = dataBuffer[1];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_ACKERMANN:
      serialCommandMode = COMMAND_ACKERMANN;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>ACKERMAN Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 2, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) ackermannSteeringTurn = dataBuffer[1];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_REVERSE_ACKERMANN:
      serialCommandMode = COMMAND_REVERSE_ACKERMANN;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>ACKERMAN FIXED Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 2, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) ackermannSteeringTurn = dataBuffer[1];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_VECTOR_DRIVE:
      serialCommandMode = COMMAND_VECTOR_DRIVE;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>Vector Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 3, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) vectorDriveDistance = dataBuffer[1];
      if(numberExtracted > 2) vectorDriveAngle = dataBuffer[2];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_VECTOR_DRIVE_IN:
      serialCommandMode = COMMAND_VECTOR_DRIVE_IN;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>Vector IN Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 3, dataBuffer);
      if(numberExtracted > 0) robotSpeed = dataBuffer[0];
      if(numberExtracted > 1) vectorDriveInternalX = dataBuffer[1];
      if(numberExtracted > 2) vectorDriveInternalX = dataBuffer[2];
      sendStatusPacket(serialReplyPacket);
      break;
      
    case COMMAND_FULL_SET:
      serialCommandMode = COMMAND_FULL_SET;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>COMMAND_FULL_SET Command");
        #endif
        
      //extract eight floats, in the order Wheel0Speed, Wheel0Angle, Wheel1Speed ...etc
      numberExtracted = extractFloats(mainSerialBuffer, 8, dataBuffer);
      break;
      
    case ROBOT_DATA_PACKET:
      serialCommandMode = ROBOT_DATA_PACKET;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>ROBOT_DATA_PACKET Command");
        #endif
        
      if(mainSerialBuffer[readIndex] == ' ') readIndex++;
      tempVar = (int)mainSerialBuffer[readIndex]-48;
      if(tempVar > -1 && tempVar < 10) {
        sendStatusPacket(tempVar);
      }
      else sendNack();
      break;
      
      
    case SET_DATA_PACKET_TYPE:
      serialCommandMode = SET_DATA_PACKET_TYPE;
      if(mainSerialBuffer[readIndex] == ' ') readIndex++;
      tempVar = (int)mainSerialBuffer[readIndex]-48;
      if(tempVar > -1 && tempVar < 10) {
        serialReplyPacket = tempVar;
        sendAck();
      }
      else sendNack();
      break;
      
    case SET_RANGER:
      serialCommandMode = SET_RANGER;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>SET_RANGER Command");
        #endif
      
      numberExtracted = extractFloats(mainSerialBuffer, 1, dataBuffer);
      if(numberExtracted > 0) setLaserAngle(dataBuffer[0], 1023);
      else sendNack();
      break;
      
    case RANGE_CHECK:
      serialCommandMode = RANGE_CHECK;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>RANGE_CHECK Command");
        #endif
      laserRangeReading = readLaser();
      sendRanger();
      break;
      
    case COMMAND_WHEEL_STATUS:
      serialCommandMode = COMMAND_WHEEL_STATUS;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>COMMAND_WHEEL_STATUS Command");
        #endif
      
      if(mainSerialBuffer[readIndex] == ' ') readIndex++;
      tempVar = (int)mainSerialBuffer[readIndex]-48;
      if( sendWheelStatus(tempVar) ) sendNack();
      break;
    
    case READ_IMU:
      serialCommandMode = READ_IMU;
      //The reply to this command is sent straight from the IMU - recieved data from the IMU gets passed from that serial port to the main one.
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>READ_IMU Command");
        #endif
      //print out the encoder readings
      break;
      
    case SET_PAUSE_STATE:
      serialCommandMode = SET_PAUSE_STATE;
      if(mainSerialBuffer[readIndex] == ' ') readIndex++;
      tempVar = (int)mainSerialBuffer[readIndex]-48;
      if(tempVar >=0 && tempVar <10) {
        robotPauseState = tempVar;
        sendAck();
      }
      else sendNack();
      
        #ifdef MAINSERIALDEBGUG
        Serial.print("#>SET_PAUSE_STATE");
        Serial.println(robotPauseState);
        #endif
      
      if(robotPauseState == 0) setSafetyState(0);
      changeRobotPauseState = 1;
      break;
      
    case SET_AXLE_POSITION:
      serialCommandMode = SET_AXLE_POSITION;
      if(extractFloats(mainSerialBuffer, 1, dataBuffer)){
        ackermannAxlePos = dataBuffer[0];
        sendAck();
      }
      else sendNack();
      break;
      
    case SET_STEERING_SPEED:
      serialCommandMode = SET_STEERING_SPEED;
      if(extractFloats(mainSerialBuffer, 1, dataBuffer)){
        robotSteeringSpeed = dataBuffer[0];
        sendAck();
      }
      else sendNack();
      break;
    
    case SET_WHEEL_MODE:
      serialCommandMode = SET_WHEEL_MODE;
      if(mainSerialBuffer[readIndex] == ' ') readIndex++;
      if(mainSerialBuffer[readIndex] == '0')  {
        wheelMode = 0;
        sendAck();
      }
      else if(mainSerialBuffer[readIndex] == '1')  {
        wheelMode = 1;
        sendAck();
      }
      else sendNack();
      break;

    case START_SCANNING:
      serialCommandMode = START_SCANNING;
      laserEnabled = 1;
      sendAck();
      break;

    case STOP_SCANNING:
      serialCommandMode = STOP_SCANNING;
      laserEnabled = 0;
      sendAck();
      break;

      
    case EMERGENCY_STOP:
      //setSafetyState(1); //cut power NOW!!!
      serialCommandMode = EMERGENCY_STOP;
      
        #ifdef MAINSERIALDEBGUG
        Serial.println("#>EMERGENCY_STOP");
        #endif
      
      setSafetyState(1);
      robotPauseState = 2;
      changeRobotPauseState = 1;
      resetControlVariables();
      sendAck();
      break;
      
    default:
      sendNack();
      return 1;
  }
  return 0;
}
//==========================================================================


int extractFloats(char characterBuffer[], int numberOfFloats, float dataArray[]){
  //extracts floating point values to the array
  //find a valid char
  int index = 0;
  String tempS = "";
  char tempString[32];
  int tempSIndex = 0;
  //tempString[tempSIndex];
  float tempFloat;
  boolean sign = 0; //1 = inverted
  int numberExpected = 0;
  int numberExtracted = 0;
  boolean startFound = 0;
  
  for(numberExpected = 0; numberExpected < numberOfFloats; numberExpected++){
    while(index < MAINBUFFERSIZE){
      if(characterBuffer[index] > 47 && characterBuffer[index] < 58){
        startFound = 1;
        break; //first valid number char found ...
      }
      else if(characterBuffer[index] == '-') sign = 1; //it is a negative number
      index++;
      //Serial.print("Scanning, Index = ");
      //Serial.println(index);
    }
    
    //if(index == bufferLength) return numberExtracted;
    while(index < MAINBUFFERSIZE){
      if(characterBuffer[index] > 47 && characterBuffer[index] < 58){
        tempString[tempSIndex]=characterBuffer[index];
        tempSIndex++;
      }
      else if(characterBuffer[index] == '.'){
        tempString[tempSIndex]=characterBuffer[index];
        tempSIndex++;
      }
      else break;
      index++;
    }
    
    if(startFound == 0) return numberExtracted;
    //if you get here then you have some valid characters in tempS
    //send them for extaction!!!11!!
    //set the current position to null (it is the one after the last valid char
    tempString[tempSIndex] = NULL;
    tempFloat = atof(tempString);
    if(sign) dataArray[numberExtracted] = -tempFloat;
    else dataArray[numberExtracted] = tempFloat;
    numberExtracted++;
    //Serial.print("String=");
    //Serial.print(tempS);
    //Serial.print(" Float=");
    //Serial.println(dataArray[numberExtracted], 4);
    tempSIndex = 0;
    sign = 0;
    startFound = 0;
  }
  return numberExtracted;
}


/*
float convertToFloat(String tempString ){
  
  char floatbuf[32]; // make this at least big enough for the whole string
  tempString.toCharArray(floatbuf, sizeof(floatbuf));
  return atof(floatbuf);
  
}*/

void resetMainSerialBuffer(){
  //mainBufferChars = 0;
  for(int n = 0; n < MAINBUFFERSIZE; n++) mainSerialBuffer[n] = 0;
  mainWriteIndex = 0;
  mainBufferOverflow = 0;
}
//==========================================================================
void sendAck(){
  //send an ack to the main serial port
  Serial.print('#');
  Serial.println(ACK);
}
//==========================================================================
void sendNack(){
  //send an ack to the main serial port
  Serial.print('#');
  Serial.println(NACK);
}
//==========================================================================
void sendReplyStart(char commandChar){
  Serial.write(MAINPACKETSTART);
  Serial.write(commandChar);
  Serial.write(' ');
}
//==========================================================================
void sendReplyEnd(){
  Serial.write(NEWLINE);
}
//==========================================================================
void sendStatusPacket(int packetNo){
  //reply with data packet
  switch(packetNo){
    case 0:
      sendAck();
      break;
    case 1:
      sendEncoderSonarData();
      break;
    case 2:
      sendEncoders();
      break;
    case 3:
      sendSonar();
      break;
    case 4:
      sendRangeArray();
      break;
    case 5:
      sendReadIMU();
      break;
    default:
    sendReplyEnd();
  }
}

//==========================================================================
void sendEncoderSonarData(){
  //send encoder and sonar data
  sendReplyStart(ROBOT_DATA_PACKET);
  for(int n = 0; n < 4; n++){
     Serial.print(myUnibot.wheelUnit[n].wheelEncoderCount);
     Serial.write(' ');
  }
  for(int n = 0; n < 4; n++){
     Serial.print(sonarSensors[n]);
     if(n == 3) sendReplyEnd();
     else Serial.write(' ');
  }
}
//==========================================================================
void sendSonar(){
  //send data on the robot state
  sendReplyStart(ROBOT_DATA_PACKET);
  for(int n = 0; n < 4; n++){
     Serial.print(sonarSensors[n]);
     if(n == 3) sendReplyEnd();
     else Serial.write(' ');
  }
}
//==========================================================================
void sendEncoders(){
  //send data on the robot state
  sendReplyStart(ROBOT_DATA_PACKET);
  for(int n = 0; n < 4; n++){
     Serial.print(myUnibot.wheelUnit[n].wheelEncoderCount);
     if(n == 3) sendReplyEnd();
     else Serial.write(' ');
  }
}

//==========================================================================
void sendRangeArray(){
  //send data on the robot state
  sendReplyStart(ROBOT_DATA_PACKET);
  for(int n = 0; n < laserSamples+1; n++){
     Serial.print(laserValues[n]);
     if(n == laserSamples) sendReplyEnd();
     else Serial.write(' ');
  }
}

//==========================================================================
void sendRanger(){
  //send data on the robot state
  sendReplyStart(RANGE_CHECK);
  Serial.print(laserRangeReading);
  sendReplyEnd();
}
//==========================================================================

boolean sendWheelStatus(int wheelNo){
  if(wheelNo < 0 || wheelNo > 3) return 1;
  sendReplyStart(COMMAND_WHEEL_STATUS);
  Serial.print(myUnibot.wheelUnit[wheelNo].wheelEncoderCount);
  Serial.write(' ');
  Serial.print(myUnibot.wheelUnit[wheelNo].wheelMotorCurrent);
  Serial.write(' ');
  Serial.print(myUnibot.wheelUnit[wheelNo].wheelSupplyVoltage);
  Serial.write(' ');
  Serial.print(myUnibot.wheelUnit[wheelNo].wheelBoardTemp);

  sendReplyEnd();
  return 0;
}
