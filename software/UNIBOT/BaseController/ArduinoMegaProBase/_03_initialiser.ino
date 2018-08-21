void initialiseController(){
  //initialise all the main functions
  initPins();
  Dynamixel.begin(1000000, RS485_DMX_RECIEVER_ENABLE, RS485_DMX_DRIVER_ENABLE);
  WHEEL_COMM.begin(WHEEL_COMM_BAUD); //WHEEL_COMM_BAUD is in the global wheel comm header file
  MAIN_COMM.begin(57600);
  //MAIN_COMM.begin(250000);
  AUX_COMM.begin(57600);
  myUnibot.initialise(30, 37);
}

void initPins(){
  //initialise all the pins
  pinMode(SAFETY1PIN, OUTPUT);
  pinMode(SAFETY2PIN, INPUT);
  pinMode(SAFETY3PIN, INPUT);
  pinMode(SAFETY4PIN, INPUT);
  
  
  pinMode(RS485_DMX_RECIEVER_ENABLE, OUTPUT);
  digitalWrite(RS485_DMX_RECIEVER_ENABLE, RS485_RECIEVER_DISABLE);
  pinMode(RS485_DMX_DRIVER_ENABLE, OUTPUT);
  digitalWrite(RS485_DMX_DRIVER_ENABLE, RS485_DRIVER_DISABLE);
  pinMode(RS485_WHEEL_RECIEVER_ENABLE, OUTPUT);
  digitalWrite(RS485_WHEEL_RECIEVER_ENABLE, RS485_RECIEVER_DISABLE);
  pinMode(RS485_WHEEL_DRIVER_ENABLE, OUTPUT);
  digitalWrite(RS485_WHEEL_DRIVER_ENABLE, RS485_DRIVER_DISABLE);
  
  pinMode(MOTOR1_PULSE, OUTPUT);
  pinMode(MOTOR1_CHAIN, OUTPUT);
  pinMode(MOTOR2_PULSE, OUTPUT);
  pinMode(MOTOR2_CHAIN, OUTPUT);
  pinMode(MOTOR3_PULSE, OUTPUT);
  pinMode(MOTOR3_CHAIN, OUTPUT);
  pinMode(MOTOR4_PULSE, OUTPUT);
  pinMode(MOTOR4_CHAIN, OUTPUT);
  
  digitalWrite(MOTOR1_PULSE, 0);
  digitalWrite(MOTOR2_PULSE, 0);
  digitalWrite(MOTOR3_PULSE, 0);
  digitalWrite(MOTOR4_PULSE, 0);
  
  digitalWrite(MOTOR1_CHAIN, 0);
  digitalWrite(MOTOR2_CHAIN, 0);
  digitalWrite(MOTOR3_CHAIN, 0);
  digitalWrite(MOTOR4_CHAIN, 0);
}

int initStartingPosition(){
  //initialise or reinitialise to a  starting position for the wheels
  int startError = 0;
  setsteeringTrim();
  for(int n = 0; n < 4; n++) {
    myUnibot.setSteeringAngle(n, 0.0);
    resetWheel(n);
  }
  startError = initialiseWheel(WHEEL1, 0, 0, 1);
  //if(startError) printCommError(WHEEL1, startError);
  startError += initialiseWheel(WHEEL2, 0, 0, 1);
  //if(startError) printCommError(WHEEL2, startError);
  startError += initialiseWheel(WHEEL3, 0, 1, 1);
  //if(startError) printCommError(WHEEL3, startError);
  
  startError += initialiseWheel(WHEEL4, 0, 1, 1);
  //if(startError) printCommError(WHEEL4, startError);
  
  for(int n = 0; n < 4; n++) {
    myUnibot.wheelUnit[n].steeringRate = robotSteeringSpeed;
  }
  
  
  if(laserEnabled) {
    startError += initLaser();
    laserScanStep(1);
  }
  if(startError){
    Serial.print("#>Start Error:");
    Serial.println(startError);
  }
  else refreshMotion();
  return startError;
}


void resetControlVariables(){
  robotSteeringSpeed = 1.0;
  diffSteeringRate = 0.0;
  robotSpeed = 0.0;
  crabAngle = 0.0;
  vectorDriveInternalX = 0.0;
  vectorDriveInternalY = 0.0;
  //vectorDriveRotation = 0.0;
  vectorDriveDistance = 0.0;
  vectorDriveAngle = 0.0;
  ackermannSteeringTurn = 0.0;
  
  myUnibot.reset();
}
