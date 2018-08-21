#include <UniBotCommands.h>

//mega pro remote

#define RADIO Serial
float x1, y1, x2, y2;
int selectorSwitch = 0;
boolean mode = 0; //0 = pause, 1 = run
#define JX1 0
#define JY1 1
#define JX2 2
#define JY2 3

#define SELECTOR0  8
#define SELECTOR1  9
#define SELECTOR2  10
#define SELECTOR3  11
#define SELECTOR4  12
#define SELECTOR5  13
#define BUTTON1    6
#define BUTTON2    7

#define STOPBUTTON BUTTON2
#define STARTBUTTON BUTTON1
void setup(){
  Serial.begin(57600);
  Serial1.begin(57600);
  pinMode(SELECTOR0, INPUT_PULLUP);
  pinMode(SELECTOR1, INPUT_PULLUP);
  pinMode(SELECTOR2, INPUT_PULLUP);
  pinMode(SELECTOR3, INPUT_PULLUP);
  pinMode(SELECTOR4, INPUT_PULLUP);
  pinMode(SELECTOR5, INPUT_PULLUP);
  
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
}


void loop(){
  //sendSetPauseState('0');
  readJoysticks();
  selectorSwitch = readSelector();
  //Serial.print("Switch:");
  //Serial.println(selectorSwitch);
  
  switch(selectorSwitch){
    case 10:
      sendEmergencyStop();
      mode = 0;
      break;
    case 20:
      sendSetPauseState('0');
      mode = 1;
      break;
    case 0:
      if(mode) sendDiffDrive(y1, x2, 0);
      break;
    case 1:
      if(mode) sendDiffDrive(y1, x2, 0);
      break;
    case 2:
      if(mode) sendDiffDrive(y1, x2, 1);
      break;
    case 3:
      if(mode) sendAckermannDrive(y1, x2, 1);
      break;
    case 4:
      if(mode) sendAckermannDrive(y1, x2, 0);
      break;
    case 5:
      if(mode) sendVectorDrive(y1, x2, y2*90.0);
      break;
    case 6:
      if(mode) sendVectorDriveInternal(y1, x2, y2);
      break;
    default:
      
      break;
  }
  delay(200);
  
}

void printJoysticks(){
  Serial.print("X1 ");
  Serial.print(x1);
  
  Serial.print(" Y1 ");
  Serial.print(y1);
  
  Serial.print(" X2 ");
  Serial.print(x2);
  
  Serial.print(" Y2 ");
  Serial.println(y2);
}

void readJoysticks(){
  x1 = (float)analogRead(JX1);
  y1 = (float)analogRead(JY1);
  x2 = (float)analogRead(JX2);
  y2 = (float)analogRead(JY2);
  
  x1 = scaleJoy(x1);
  y1 = -scaleJoy(y1);
  x2 = scaleJoy(x2);
  y2 = -scaleJoy(y2);
  
 // printJoysticks();
}

float scaleJoy(float initialVal){
  return (initialVal-512)/512;
}

int readSelector(){
  if(!digitalRead(STOPBUTTON)) return 10;
  else if(!digitalRead(STARTBUTTON)) return 20;
  else if(!digitalRead(SELECTOR5)) return 1;
  else if(!digitalRead(SELECTOR4)) return 2;
  else if(!digitalRead(SELECTOR3)) return 3;
  else if(!digitalRead(SELECTOR2)) return 4;
  else if(!digitalRead(SELECTOR1)) return 5;
  else if(!digitalRead(SELECTOR0)) return 6;
  else return 0;
}

