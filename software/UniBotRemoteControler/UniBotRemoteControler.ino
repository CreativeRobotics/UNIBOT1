
int n = 0;

void setup(){
  Serial1.begin(57600);
}


void loop(){
  if(Serial1.available()){
    Serial1.write(Serial1.read());
  }
  delay(1);
  Serial1.print("Testing ");
  Serial1.println(n);
  n++;
}

