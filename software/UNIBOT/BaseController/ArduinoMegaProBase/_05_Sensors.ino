
void readMainBoardSensors(){
  //reads various voltages
  //12V raw reading is 662
  float tempVar = 0;
  tempVar = (float)analogRead(MAINPOWERVOLTAGE_ADC);
  
  mainBoardVoltage = (int)(tempVar * 1.81);

  
}

void readAnalogSensors(){
  sonarSensors[0] = analogRead(SONAR1);
  sonarSensors[1] = analogRead(SONAR2);
  sonarSensors[2] = analogRead(SONAR3);
  sonarSensors[3] = analogRead(SONAR4);
  
}

