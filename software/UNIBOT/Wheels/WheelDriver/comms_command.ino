//Comm command and response functions


//=======================================================
void sendPacketHeader(){
  //send everything for the start of a packet
   Serial.write('#');
   Serial.write('0');
}

void sendEndPacket(){
  Serial.write(END_OF_PACKET);
  Serial.flush();
  delayMicroseconds(commEndDelay);
}
  
//=======================================================
void sendACK(){
  //send an acknowledge (a K for OK)
  //
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.write('K');
  sendEndPacket();
  commRXEnable();
}

void sendNACK(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.write('!');
  sendEndPacket();
  commRXEnable();

}
void sendSpeed(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.write(READ_SPEED);
  if(invertControls) Serial.print(-(motorSpeed/motorSpeedDivisor));
  else Serial.print(motorSpeed/motorSpeedDivisor);
  sendEndPacket();
  commRXEnable();
}
//=======================================================
void sendSpeedError(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.write(READ_SPEED_ERROR);
  if(invertControls) Serial.print(-getLastSpeedError());
  else Serial.print(getLastSpeedError());
  sendEndPacket();
  commRXEnable();
}
//=======================================================
void sendEncoderCount(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.print(READ_ENCODER_COUNT);
  if(invertControls) Serial.print(-qeiCounts);
  else Serial.print(qeiCounts);
  sendEndPacket();
  commRXEnable();
}
//=======================================================
void sendPCBTemp(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.print(GET_BOARD_TEMPERATURE);
  Serial.print(boardTemp);
  sendEndPacket();
  commRXEnable();
}

void sendBoardVoltage(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.print(GET_SUPPLY_VOLTAGE);
  Serial.print(supplyVoltage);
  sendEndPacket();
  commRXEnable();
}

void sendMotorCurrent(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.print(GET_MOTOR_CURRENT);
  Serial.print(motorCurrent);
  sendEndPacket();
  commRXEnable();
}

void sendFeedback(){
  delayMicroseconds(commReplyDelay);
  commTXEnable();
  sendPacketHeader();
  Serial.print(GET_FEEDBACK);
  if(invertControls) Serial.print(-qeiCounts);
  else Serial.print(qeiCounts);
  Serial.write(' ');
  Serial.print(motorCurrent);
  Serial.write(' ');
  Serial.print(supplyVoltage);
  Serial.write(' ');
  Serial.print(boardTemp);
  sendEndPacket();
  commRXEnable();
}
