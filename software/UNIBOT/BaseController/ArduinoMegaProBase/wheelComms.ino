//#include <UniBotComms.h>

//Wheel comm functions
/*
#define START_OF_PACKET '#'
#define END_OF_PACKET '\n'

#define SET_MOTOR_POWER 'M'
#define READ_ENCODER_COUNT 'C'
#define GET_BOARD_TEMPERATURE 'T'
#define DISENGAGE_MOTOR 'D'
#define ENGAGE_MOTOR 'E'
#define HALT_MOTOR 'H'
#define RESET_ENCODER 'R'
#define RAW_MODE 'V'
#define PID_MODE 'P'
*/
  /*commands:
  S = Set Motor Power
  C = Get Encoder Count
  T = Get PCB Temperature Data
  */
  
/*
A delay is required before the RS485 IC can be set to reciever after sending data
The serial flush command us used to wait for the TX buffer to empty but an additional 200Usecs is required after this returns
*/
#define wheelCommRxDelay 300 //usecs
  
void wheelCommTxEnable(){
  digitalWrite(RS485_WHEEL_RECIEVER_ENABLE, RS485_RECIEVER_DISABLE);
  delayMicroseconds(100);
  digitalWrite(RS485_WHEEL_DRIVER_ENABLE, RS485_DRIVER_ENABLE);
  delayMicroseconds(50);
}
void wheelCommTxEnableLoopBack(){
  digitalWrite(RS485_WHEEL_RECIEVER_ENABLE, RS485_RECIEVER_ENABLE);
  digitalWrite(RS485_WHEEL_DRIVER_ENABLE, RS485_DRIVER_ENABLE);
  delayMicroseconds(50);
}
void wheelCommRxEnable(){
  digitalWrite(RS485_WHEEL_DRIVER_ENABLE, RS485_DRIVER_DISABLE); //disable driver first
  delayMicroseconds(100);
  digitalWrite(RS485_WHEEL_RECIEVER_ENABLE, RS485_RECIEVER_ENABLE);
  delayMicroseconds(50);
}

void wheelCommDisable(){
  digitalWrite(RS485_WHEEL_RECIEVER_ENABLE, RS485_RECIEVER_DISABLE);
  digitalWrite(RS485_WHEEL_DRIVER_ENABLE, RS485_DRIVER_DISABLE);
  delayMicroseconds(50);
}

//=======================================================
void sendPacketHeader(char wheelID){
  //send everything for the start of a packet
  WHEEL_COMM.print('#');
  WHEEL_COMM.print(wheelID);
}

void sendEndPacket(){
  WHEEL_COMM.write(END_OF_PACKET);
  WHEEL_COMM.flush();
  delayMicroseconds(wheelCommRxDelay);
}
//=======================================================

int readWheelEncoder(char wheelID){
  int replyErrors = 0;
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(READ_ENCODER_COUNT);
  sendEndPacket();
  wheelCommRxEnable();
  replyErrors = listenForData( &myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelEncoderCount );
  //if(replyErrors == 0) wheelEncoderValues[getWheelIDIndex(wheelID)] = latestWheelRSData;
  return replyErrors;
  
}
//=======================================================

int readBoardTemp(char wheelID){
  int replyErrors = 0;
  long int tempLong;
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(GET_BOARD_TEMPERATURE);
  sendEndPacket();
  wheelCommRxEnable();
  replyErrors = listenForData( &tempLong );
  
  if(replyErrors == 0) myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelBoardTemp = (int)tempLong;
  return replyErrors;
  
}
//=======================================================

int readBoardVoltage(char wheelID){
  int replyErrors = 0;
  long int tempLong;
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(GET_SUPPLY_VOLTAGE);
  sendEndPacket();
  wheelCommRxEnable();
  replyErrors = listenForData( &tempLong );
  if(replyErrors == 0) myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelSupplyVoltage = (int)tempLong;
  return replyErrors;
  
}
//=======================================================
int readWheelCurrent(char wheelID){
  int replyErrors = 0;
  long int tempLong;
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(GET_MOTOR_CURRENT);
  sendEndPacket();
  wheelCommRxEnable( );
  replyErrors = listenForData( &tempLong);
  if(replyErrors == 0) myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelMotorCurrent = (int)tempLong;
  return replyErrors;
  
}

int sendDisengageMotor(char wheelID){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(DISENGAGE_MOTOR);
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int sendEngageMotor(char wheelID){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(ENGAGE_MOTOR);
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int sendHaltMotor(char wheelID){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(HALT_MOTOR); 
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int sendResetEncoder(char wheelID){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(RESET_ENCODER);
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int sendInvertDirection(char wheelID, boolean inverted){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  if(inverted)  WHEEL_COMM.write(INVERT_CONTROLS);
  else  WHEEL_COMM.write(NORMAL_CONTROLS);
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int sendSetMode(char wheelID, char wheelMode){
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  if(wheelMode == SET_PID_MODE){//PID
    WHEEL_COMM.write(SET_PID_MODE);
  }
  else{//No PID
    WHEEL_COMM.write(SET_RAW_MODE);
  }
  sendEndPacket();
  wheelCommRxEnable();
  return listenForAck();
}
//=======================================================

int setWheelMotorSpeed(char wheelID, int wheelSpeed, boolean getFeedback){
  //transmit a message to the appropriate wheel
  int replyErrors = 0;
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  if(getFeedback == 0) WHEEL_COMM.write(SET_MOTOR_POWER);
  else WHEEL_COMM.write(SET_MOTOR_POWER_GET_ENCODER);
  WHEEL_COMM.print(wheelSpeed);
  sendEndPacket();
  wheelCommRxEnable();
  if(getFeedback == 0) return listenForAck();
  replyErrors = listenForData( &myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelEncoderCount );
  //replyErrors = listenForData( &wheelEncoderValues[getWheelIDIndex(wheelID)] );
  //if(replyErrors == 0) wheelEncoderValues[getWheelIDIndex(wheelID)] = latestWheelRSData;
  return replyErrors;
}

int setWheelRefreshSensors(char wheelID, int wheelSpeed){
  //transmit a message to the appropriate wheel with the speed and get a full sensor packet bacl
  int replyErrors = 0;
  long int recievedData[4];
  wheelCommTxEnable();
  sendPacketHeader(wheelID);
  WHEEL_COMM.write(SET_MOTOR_POWER_GET_FEEDBACK);
  WHEEL_COMM.print(wheelSpeed);
  sendEndPacket();
  wheelCommRxEnable();
  replyErrors = listenForLongData(recievedData);
  if(replyErrors == 0) {
    myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelEncoderCount = recievedData[0];
    myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelMotorCurrent = recievedData[1];
    myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelSupplyVoltage = recievedData[2];
    myUnibot.wheelUnit[getWheelIDIndex(wheelID)].wheelBoardTemp = recievedData[3];
    
    //wheelEncoderValues[getWheelIDIndex(wheelID)] = recievedData[0];
    //wheelCurrentDraw[getWheelIDIndex(wheelID)] = recievedData[1];
    //wheelSupplyVoltage[getWheelIDIndex(wheelID)] = recievedData[2];
    //wheelBoardTemps[getWheelIDIndex(wheelID)] = recievedData[3];
  }
  return replyErrors;
}

//=======================================================

int listenForAck(){
  //wait for acknowledge message
  //return -6;
  char winByte;
  int wrxState = 0;
  int timeOut = 0;
  int bytesRecieved = 0;
  while(timeOut < 50){
    if(WHEEL_COMM.available()>0) {
      //capture the latest byte
       
      winByte = WHEEL_COMM.read();
      bytesRecieved++;
      //Serial.write('(');
      //Serial.print((byte)winByte);
      //Serial.write(')');
      //Serial.write(winByte);
      //Serial.println(" (RX)");
      //check the parser state
      switch (wrxState){
        case 0:
          if(winByte == '#') wrxState++;
          else wrxState = 0;
           //Serial.print("WRX=");
           //Serial.println(wrxState);
          break;
        case 1:
          if(winByte == '0') wrxState++;
          else wrxState = 0;
           //Serial.print("WRX=");
           //Serial.println(wrxState);
          break;
        case 2:
          if(winByte == 'K'){
            wrxState = 0;
             //Serial.print("WRX=");
             //Serial.println(wrxState);
            return 0; //Ack Recieved - no errors
          }
          else if(winByte == '!'){
            wrxState = 0;
            //Serial.println("NAK");
            return 1; //NACK recieved
          }
          else wrxState = 0;
          return -1;
          break;
        default:
          return -3; //garbage
      }
    }
    timeOut++;
    delayMicroseconds(100);
  }
  //Serial.print("(RX)");
  if(bytesRecieved == 0) return -10; //no data
  else return -2; //timeout with garbage
}
//----------------------------------------------
int listenForData(long int *recievedData){
  
  char wdinByte;
  int wrxDataState = 0;
  int datatimeOut = 0;
  #define TEMPSERIALBUFFERSIZE 32
  char tempSerialBuffer[TEMPSERIALBUFFERSIZE];
  int tempWriteIndex = 0;
  //return 6;
  int bytesRecieved = 0;
  //int recievedVariable = 0;
  while(datatimeOut < 200){
    if(WHEEL_COMM.available()>0) {
      wdinByte = WHEEL_COMM.read();
      bytesRecieved++;
      //check the parser state
      switch (wrxDataState){
        case 0:
          if(wdinByte == '#') wrxDataState++;
          else wrxDataState = 0;
          break;
          
        case 1:
          if(wdinByte == '0') wrxDataState++;
          else wrxDataState = 0;
          break;
          
        case 2:
          if(checkValidCommand(wdinByte) == 1){
            wrxDataState++;
          }
          else{
            wrxDataState = 0;
            return -1; //Invalid Command
          }
          break;
        case 3:
          //collect incoming bytes
          //terminate on buffer full or '/r' char
          //if incoming bytes are NOT a numeral or newline terminate with an error
          if( (wdinByte >= '0' && wdinByte <= '9') || wdinByte == '-'){
            tempSerialBuffer[tempWriteIndex] = wdinByte;
            tempWriteIndex++;
          }
          else if(wdinByte == END_OF_PACKET){
            //parse the packet
            //assign the variable based on the comand and the wheel ID
            *recievedData = parseInt32(tempSerialBuffer, 0, tempWriteIndex-1);
            wrxDataState = 0;
            return 0;
          }
          else{
            //invalid data - return an error
            wrxDataState = 0;
            return -4;
          }
          if(tempWriteIndex == TEMPSERIALBUFFERSIZE) return -5;//buffer overflow
          break;
        default:
          return -3; //garbage
          break;
      }
    }
    datatimeOut++;
    delayMicroseconds(100);
  }
  //Serial.print("(RX)");
  if(bytesRecieved == 0) return -6; //no data
  else return -2; //timeout with garbage
}
//----------------------------------------------

int listenForLongData(long int dataArray[]){
  /*listens for a series of variables from the wheel
  used to colect full wheel status data in a single packet
  Data order is:
  EncoderCount
  Motor Current
  Board Temperature
  Board Voltage
  */
  char wdinByte;
  int wrxDataState = 0;
  int datatimeOut = 0;
  #define TEMPSERIALBUFFER2SIZE 64
  char tempSerialBuffer2[TEMPSERIALBUFFER2SIZE];
  int tempWriteIndex = 0;
  boolean waitingForData = 1;
  //loop here whilst you wait for incoming chars
  //if you get a newline then exit the loop
  //if you get a timeout or overflow then return an error
  //Serial.println("Waiting For Data");
  while(waitingForData){
    if(WHEEL_COMM.available()){
      //Serial.println("Recieving ..");
      tempSerialBuffer2[tempWriteIndex] = WHEEL_COMM.read();
      if(tempSerialBuffer2[tempWriteIndex] == END_OF_PACKET) waitingForData = 0;
      tempWriteIndex++;
      if(tempWriteIndex >=TEMPSERIALBUFFER2SIZE) return -5; //overflow
    }
    else{
      datatimeOut++;
      if(datatimeOut > 200) return -2;
      delayMicroseconds(100);
    }
  }
  //Serial.println("Packet Complete ...");
  //for(int n = 0; n < tempWriteIndex; n++){
  //  Serial.write(tempSerialBuffer2[n]);
  //}
  int readIndex1 = 0;
  int startInt, endInt;
  //if you get to here then you have a complete data packet
  //find the packet start byte
  while(tempSerialBuffer2[readIndex1] != '#'){
    readIndex1++;
    if(readIndex1 == tempWriteIndex) return -2;
  }
  //Serial.println("Found #");
  readIndex1++;
  //check node number is zero
  if(tempSerialBuffer2[readIndex1] != '0') return -2;
  //Serial.println("Valid Node");
  readIndex1++;
  //then get the command byte
  if(checkValidCommand(tempSerialBuffer2[readIndex1]) != 1) return -2;
  //Serial.println("Valid Command");
  readIndex1++;
  //then look for the first int (collect numerals and a minus sign
  
  for(int n = 0; n < 4; n++){
    startInt = -1;
    while(startInt < 0){
      if((tempSerialBuffer2[readIndex1] >= '0' && tempSerialBuffer2[readIndex1] <= '9') || tempSerialBuffer2[readIndex1] == '-') startInt = readIndex1;
      readIndex1++;
      if(readIndex1 == tempWriteIndex) return -2;
    }
    //Serial.println("Found Int Start");
    endInt = -1;
    while(endInt < 0){
      if(tempSerialBuffer2[readIndex1] < '0' || tempSerialBuffer2[readIndex1] > '9') endInt = readIndex1-1;
      readIndex1++;
      if(readIndex1 > tempWriteIndex) return -2;
    }
    //Serial.println("Found Int End");
    //for(int n = startInt; n < endInt; n++){
      //Serial.write(tempSerialBuffer2[n]);
    //}
    //Serial.write('\n');
    //parse the int
    //Serial.print("Parsing: ");
    //Serial.println(n);
    //Serial.print("Value: ");
    //Serial.println( parseInt32(tempSerialBuffer2, startInt, endInt) );
    dataArray[n] = parseInt32(tempSerialBuffer2, startInt, endInt);
  }
  //repeat three more times
  return 0;
}
  
//----------------------------------------------
boolean checkValidCommand(char comByte){
  //check that the command is valid
  switch (comByte){
    case READ_ENCODER_COUNT:
      return 1;
      break;
    case GET_BOARD_TEMPERATURE:
      return 1;
      break;
    case GET_SUPPLY_VOLTAGE:
      return 1;
      break;
    case GET_MOTOR_CURRENT:
      return 1;
      break;
    case SET_MOTOR_POWER_GET_ENCODER:
      return 1;
      break;
    case GET_FEEDBACK:
      return 1;
      break;
      
    default:
      return 0;
  }
  return 0;
}
//----------------------------------------------    
int getWheelIDIndex(char wheelID){
  if(wheelID > '0') return wheelID-49;
  return 0;
}
//----------------------------------------------
char getWheelIDChar(int wheelIndex){
  //returns the wheel ID char from the wheel data array indes (An int from 0-3)
  //Returns a char from '1'-'4'
  char wID = 1;
  wID = 49+(wheelIndex);
  return wID;
  
}

void resetWheel(int wheelIndex){
  switch(wheelIndex){
    case 0:
      digitalWrite(MOTOR1_PULSE, HIGH);
      delay(100);
      digitalWrite(MOTOR1_PULSE, LOW);
      return;
    case 1:
      digitalWrite(MOTOR2_PULSE, 1);
      delay(100);
      digitalWrite(MOTOR2_PULSE, 0);
      return;
    case 2:
      digitalWrite(MOTOR3_PULSE, 1);
      delay(100);
      digitalWrite(MOTOR3_PULSE, 0);
      return;
    case 3:
      digitalWrite(MOTOR4_PULSE, 1);
      delay(100);
      digitalWrite(MOTOR4_PULSE, 0);
      return;
  }
  return;
}
