void sendPacketStart(){
  RADIO.write('#');
}

void sendPacketEnd(){
  RADIO.write('\n');
  RADIO.write('\r');
}


void sendDiffDrive(float robotSpeed, float robotSteering, boolean internal){
  sendPacketStart();
  if(internal)  RADIO.write(COMMAND_DIFFDRIVE_IN);
  else RADIO.write(COMMAND_DIFFDRIVE);
  RADIO.print(robotSpeed, 4);
  RADIO.write(' ');
  RADIO.print(robotSteering, 4);
  sendPacketEnd();
}

void sendAckermannDrive(float robotSpeed, float robotSteering, boolean front){
  sendPacketStart();
  if(front)  RADIO.write(COMMAND_ACKERMANN);
  else RADIO.write(COMMAND_REVERSE_ACKERMANN);
  RADIO.print(robotSpeed, 4);
  RADIO.write(' ');
  RADIO.print(robotSteering, 4);
  sendPacketEnd();
}

void sendAckermannVariableDrive(float robotSpeed, float robotSteering){
  sendPacketStart();
  RADIO.write(COMMAND_ACKERMANN_VARIABLE);
  RADIO.print(robotSpeed, 4);
  RADIO.write(' ');
  RADIO.print(robotSteering, 4);
  sendPacketEnd();
}

void sendVectorDrive(float robotSpeed, float turnRate, float turnAngle){
  sendPacketStart();
  RADIO.write(COMMAND_VECTOR_DRIVE);
  RADIO.print(robotSpeed, 4);
  RADIO.write(' ');
  RADIO.print(turnRate, 4);
  RADIO.write(' ');
  RADIO.print(turnAngle, 4);
  sendPacketEnd();
}

void sendVectorDriveInternal(float robotSpeed, float xDist, float yDist){
  sendPacketStart();
  RADIO.write(COMMAND_VECTOR_DRIVE);
  RADIO.print(robotSpeed, 4);
  RADIO.write(' ');
  RADIO.print(xDist, 4);
  RADIO.write(' ');
  RADIO.print(yDist, 4);
  sendPacketEnd();
}

void sendSetPauseState(char pState){
  sendPacketStart();
  RADIO.write(SET_PAUSE_STATE);
  RADIO.write(pState);
  sendPacketEnd();
}

void sendEmergencyStop(){
  sendPacketStart();
  RADIO.write('X');
  sendPacketEnd();
}
