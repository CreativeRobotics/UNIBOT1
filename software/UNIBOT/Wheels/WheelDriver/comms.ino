

#define SERIALBUFFERSIZE 32
char serialBuffer[SERIALBUFFERSIZE];
int bufferReadIndex = 0;
int bufferWriteIndex = 0;
int charsInBuffer = 0;
byte incomingState = 0;


char motorID = '3';

/*
Comms [#][motorID][Command][data ...][cr]
*/



#define RS485_RECIEVER_ENABLE     0
#define RS485_RECIEVER_DISABLE    1
#define RS485_DRIVER_ENABLE  1
#define RS485_DRIVER_DISABLE 0

#define commEndDelay 300
#define commReplyDelay 500 //delay in us

//=======================================================
void setup485(){
  resetBuffer();
  commRXEnable();
  //for testing use:
  //commDisable();
}
//=======================================================
void commTXEnable(){
  digitalWrite(RS485RX_EN, RS485_RECIEVER_DISABLE);
  delayMicroseconds(50);
  digitalWrite(RS485TX_EN, RS485_DRIVER_ENABLE);
  delayMicroseconds(50);
}
//=======================================================
void commRXEnable(){
  digitalWrite(RS485TX_EN, RS485_DRIVER_DISABLE);
  delayMicroseconds(50);
  digitalWrite(RS485RX_EN, RS485_RECIEVER_ENABLE);
  delayMicroseconds(50);
}
//=======================================================
void commDisable(){
  digitalWrite(RS485TX_EN, RS485_DRIVER_DISABLE);
  digitalWrite(RS485RX_EN, RS485_RECIEVER_DISABLE);
}

//=======================================================
void serialEvent(){
  //read serial chars into the buffer ...
  //look for start of packet char
  //if start of packet found then try and match the next byte to the ID
  //if matched then fill the buffer until the end of packet is recieved
  //if not matched then ignore and start looking for start of packet char
  //Serial.println("!");
  byte inByte;
  inByte = Serial.read();
  //Serial.println(millis());
  switch (incomingState){
    case 0:
      if(inByte == START_OF_PACKET) incomingState++;
      break;
    case 1:
      if(inByte == motorID) incomingState++;
      else incomingState = 0;
      break;
    case 2:
      serialBuffer[bufferReadIndex] = inByte;
      //incomingState++;
      bufferReadIndex++;
      charsInBuffer++;
      //Serial.print("Chars");
      //Serial.println(charsInBuffer);
      
      if(charsInBuffer == SERIALBUFFERSIZE-1) {
        resetBuffer(); //buffer overflow
        //Serial.println("BuffResetOverflow");
      }
      if(inByte == END_OF_PACKET && charsInBuffer > 1){
        newData = 1;
        incomingState++;
        //Serial.println("GotNewData");
      }
      break;
    case 3:
      //do nothing until newData is cleared
      if(newData == 0) resetBuffer();
    default:
      resetBuffer();
      break;
  }
}
//=======================================================
void resetBuffer(){
  bufferReadIndex = 0;
  bufferWriteIndex = 0;
  charsInBuffer = 0;
  incomingState = 0;
  newData = 0;
}

//=======================================================
char processSerial(){
  //unpack the serial data

  //The first byte in the buffer will be the command
  char command;
  int value;
  bufferReadIndex = 0;
  command = serialBuffer[bufferReadIndex];
  switch (command){
    case SET_MOTOR_POWER:
      //extract signed integer
      //send ACK
      newMotorPower =   parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) );
      if(invertControls) newMotorPower = -newMotorPower;
      sendACK();
      break;
    case SET_MOTOR_POWER_GET_ENCODER:
      //extract signed integer
      //send back the motor power
      //Serial.println(millis());
      newMotorPower =   parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) );
      if(invertControls) newMotorPower = -newMotorPower;
      sendEncoderCount();
      //Serial.println(millis());
      break;
    case SET_MOTOR_POWER_GET_FEEDBACK:
      //extract signed integer
      //send back the motor power
      //Serial.println(millis());
      newMotorPower =   parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) );
      if(invertControls) newMotorPower = -newMotorPower;
      sendFeedback();
      //Serial.println(millis());
      break;
    case READ_ENCODER_COUNT:
      sendEncoderCount();
      break;
    case READ_SPEED:
      sendSpeed();
      break;
    case READ_SPEED_ERROR:
      sendSpeedError();
      break;
    case GET_BOARD_TEMPERATURE:
      sendPCBTemp();
      break;
    case GET_SUPPLY_VOLTAGE:
      sendBoardVoltage();
      break;
      
    case GET_MOTOR_CURRENT:
      sendMotorCurrent();
      break;
    case GET_FEEDBACK:
      sendFeedback();
      break;
    case HALT_MOTOR: //zero the motor power
      newMotorPower = 0;
      sendACK();
      break;
    case DISENGAGE_MOTOR:
      setMotorFreewheel(1);
      sendACK();
      break;
    case ENGAGE_MOTOR:
      setMotorFreewheel(0);
      sendACK();
      break;
    case RESET_ENCODER:
      resetQEI();
      sendACK();
      break;
    case SET_RAW_MODE:
      motorControlMode = RAW_POWER_MODE;
      sendACK();
      break;
      
    case SET_PID_MODE:
      motorControlMode = PID_MODE;
      sendACK();
      break;
      
    case SET_KP:
      setKp( parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) ) );
      sendACK();
      break;
      
    case SET_KD:
      setKd( parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) ) );
      sendACK();
      break;
      
    case SET_KI:
      setKi( parseInt16(serialBuffer, bufferReadIndex+1, findNumeralEnd(serialBuffer, charsInBuffer, bufferReadIndex+1) ) );
      sendACK();
      break;
      
    case SET_DEFAULT_PID_PARAM:
      setPIDDefaults();
      sendACK();
      break;
      
    case INVERT_CONTROLS:
      invertControls = 1;
      sendACK();
      break;
      
    case NORMAL_CONTROLS:
      invertControls = 0;
      sendACK();
      break;
     
    default:
      command = 'X'; //unrecognised command
      sendNACK();
      break;
  }
  resetBuffer();
  return command;
}



