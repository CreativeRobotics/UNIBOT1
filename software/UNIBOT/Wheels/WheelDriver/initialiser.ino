void initialiseBoard(){
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_ND1, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(PULSE, INPUT_PULLUP);
  pinMode(CHAIN, INPUT_PULLUP);
  pinMode(RS485TX_EN, OUTPUT);
  pinMode(RS485RX_EN, OUTPUT);
  
  pinMode(QEI_A_PIN, INPUT_PULLUP);
  pinMode(QEI_B_PIN, INPUT_PULLUP);
  
  while(digitalRead(PULSE)){} //wait here if the software reset input is held high
  resetVariables();
  //setPwmFrequency(MOTOR_PWM1, 8);
  //setPwmFrequency(MOTOR_PWM2, 8);
  Timer1.initialize(256);
  //Serial.begin(57600);
  Serial.begin(WHEEL_COMM_BAUD);
  setup485();
  
  freewheelState = 1;
  initialiseDriver();
  initialiseQEI();
  //Serial.println("Initialised");
}

void resetVariables(){
  lastEncoderTime = 0;
  lastEncoderCount = 0;
  elapsedCounts = 0;
  elapsedMicros = 0;
  qeiCounts = 0;
  motorSpeed = 0;
  motorDirection = 0;
  safeMode = 0;
  rawMotorPower = 0;
  motorSpeedTarget = 0;
  invertControls = 0; //for left/right sided wheels - inverts all incoming speed commands and outgoing encoder data
  newMotorPower = 0;
  freewheelState = 1;
  //int motorControlMode = 0;
  iterator = 8;
  limit = 1024;
  timeCounter = 0;
  for(int n = 0; n < CURRENT_SAMPLE_LENGTH; n++) motorCurrentAverager[n] = 0;
  mcIndex = 0;
  mcSum = 0;
  motorCurrent = 0;
  supplyVoltage = 0;
  boardTemp = 0;
  serialResult = 0;
  newData = 0;
  motorControlMode = PID_MODE;
}



