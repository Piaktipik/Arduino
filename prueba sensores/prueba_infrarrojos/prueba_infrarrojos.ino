
byte senvela[20];

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  // seriales sensores de vela
  Serial1.begin(57600);
  Serial2.begin(57600);
  // apago ventilador
  pinMode(53,OUTPUT);
  digitalWrite(53,0);
}

void loop(){
  svela();
  delay(50);
 for(int i=0;i<20;i++){
   Serial.print(senvela[i]);
   Serial.print("\t");
 } 
 Serial.println();
}

void svela(){
  Serial1.print("R");
  Serial1.print("L");
}

void serialEvent1(){
  Serial1.read();
  for(int i=0;i<10;i++){
    senvela[i] = Serial1.read();
  }
 while(Serial1.available()>0){Serial1.read();}  
}
void serialEvent2(){
  for(int i=10;i<20;i++){
    senvela[i] = Serial2.read();
  }
 while(Serial2.available()>0){Serial2.read();}  
}

