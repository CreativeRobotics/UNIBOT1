//some external int functions used for measuring hobby RC pulses
#define int0pin 2
#define int1pin 3
#define int2pin 21
#define int3pin 20
#define RC_CENTRE 1564;


volatile long pulse0;
volatile long pulse1;
volatile long pulse2;
volatile long pulse3;

void setupExtInts(){
  pinMode(int0pin, INPUT);
  pinMode(int1pin, INPUT);
  pinMode(int2pin, INPUT);
  pinMode(int3pin, INPUT);
  attachInterrupt(0, ExtInt0, CHANGE);
  attachInterrupt(1, ExtInt1, CHANGE);
  attachInterrupt(2, ExtInt2, CHANGE);
  attachInterrupt(3, ExtInt3, CHANGE);
}
void ExtInt0(){
  static long lasttime = 0;
  long currentTime = 0;
  //if pin goes high
  if(digitalRead(int0pin) == 1) lasttime = micros();
  //if pin goes low
  else  pulse0 = micros()-lasttime;

  //Serial.println("Pulse0");
}
  
void ExtInt1(){
  static long lasttime = 0;
  long currentTime = 0;
  //if pin goes high
  if(digitalRead(int1pin) == 1) lasttime = micros();
  //if pin goes low
  else  pulse1 = micros()-lasttime;

  //Serial.println("Pulse1");
}

void ExtInt2(){
  static long lasttime = 0;
  long currentTime = 0;
  //if pin goes high
  if(digitalRead(int2pin) == 1) lasttime = micros();
  //if pin goes low
  else  pulse2 = micros()-lasttime;

  //Serial.println("Pulse2");
}


void ExtInt3(){
  static long lasttime = 0;
  long currentTime = 0;
  //if pin goes high
  if(digitalRead(int3pin) == 1) lasttime = micros();
  //if pin goes low
  else  pulse3 = micros()-lasttime;

  //Serial.println("Pulse2");
}
long getPulse(int pulseNo){
  if(pulseNo == 0) return pulse0;
  else return pulse1;
}

int getScaledPulse(int pulseNo){
  switch(pulseNo){
    case 0:
      return (int)pulse0-RC_CENTRE;
      break;
    case 1:
      return (int)pulse1-RC_CENTRE;
      break;
      
    case 2:
      return (int)pulse2-RC_CENTRE;
      break;
      
    case 3:
      return (int)pulse3-RC_CENTRE;
      break;
    default:
      return 0;
  }
}

void resetPulses(){
  pulse0 = 0;
  pulse1 = 0;
  pulse2 = 0;
}
