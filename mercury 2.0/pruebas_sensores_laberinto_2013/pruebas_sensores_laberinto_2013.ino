 /*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */
const byte eco1 = 31; // 31 35 39
const byte tri1 = 33; // 33 37 41

const byte eco2 = 35; // 31 35 39
const byte tri2 = 47; // 33 37 41

const byte eco3 = 39; // 31 35 39
const byte tri3 = 41; // 33 37 41

// ----- sevo-motores -----
#include <Servo.h> 
Servo servo1;
Servo servo2;


unsigned int enco1 = 0;
unsigned int enco2 = 0;

void setup() {
  // ------ servo-motores ------
  servo1.attach(8);servo1.write(0);
  servo2.attach(9);servo2.write(0);
  
  
  attachInterrupt(0, in_enco1,CHANGE); // DERECHA
  attachInterrupt(1, in_enco2,CHANGE); // IZQUIERDA
  
  pinMode(4,0);
  pinMode(5,0);
  pinMode(6,0);
  pinMode(7,0);
  
  pinMode(tri1,1);
  pinMode(eco1,0);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  digitalWrite(4,0);
  digitalWrite(7,0);
  analogWrite(5,0);
  analogWrite(6,0);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int an0 = analogRead(A0);
  int an1 = analogRead(A1);
  int an2 = analogRead(A2);
  int an3 = analogRead(A3);
  // encoders
  int an4 = analogRead(A4);
  int an5 = analogRead(A5);
  // pelota
  int an8 = analogRead(8);
  int an9 = analogRead(9);
  int an10 = analogRead(10);
  int an11 = analogRead(11);
  
  int in1 = digitalRead(10);
  int in2 = digitalRead(11);
  
  // print out the value you read:
  Serial.print("an0:");
  Serial.print(an0);
  Serial.print("\t");
  Serial.print("an1:");
  Serial.print(an1);
  Serial.print("\t");
  Serial.print("an2:");
  Serial.print(an2);
  Serial.print("\t");
  Serial.print("an3:");
  Serial.print(an3);
  Serial.print("\t");
  // econders
  Serial.print("enc1:");
  Serial.print(an4);
  Serial.print("\t");
  Serial.print("enc2:");
  Serial.print(an5);
  Serial.print("\t");
  // sensores de pelota
  Serial.print("an8:");
  Serial.print(an8>120);
  Serial.print("\t");
  Serial.print("an9:");
  Serial.print(an9);
  Serial.print("\t");
  Serial.print("an9:");
  Serial.print(600 < an9 && an9< 750);
  Serial.print("\t");
  Serial.print("an10:");
  Serial.print(an10>500);
  Serial.print("\t");
  Serial.print("an11:");
  Serial.print(an11>500);
  Serial.print("\t");
  Serial.print("in1:");
  Serial.print(in1);
  Serial.print("\t");
  Serial.print("in2:");
  Serial.print(in2);
  
  Serial.print("\t");
  Serial.print("enco1:");
  Serial.print(enco1);
  Serial.print("\t");
  Serial.print("enco2:");
  Serial.print(enco2);
  
  Serial.print("\t");
  Serial.print("dis1:");
  Serial.print(sultra1());
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("dis2:");
  Serial.print(sultra2());
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("dis3:");
  Serial.print(sultra3());
  Serial.println("\t");
  delay(10);        // delay in between reads for stability
  
  
  delay(100);
  // subo el servo
  if(600 < an9 && an9< 750){
      subirpinza();
      delay(2000);
      bajarpinza();
  }
  
  if(an10 > 500 && an11 > 500 && an8 > 500){
      descargarpelota();
  }
  
}


// ------------------------------------ servo-motores ------------------------------------ 
void subirpinza(){
  int pos;
  for(pos = 0; pos < 180; pos += 1)  {                                  
    servo1.write(pos);delay(2);
  }
}
void bajarpinza(){
  int pos;
  for(pos = 180; pos > 0; pos -= 1)  {                                  
    servo1.write(pos);delay(2);
  }
}
void descargarpelota(){
  int pos;
  for(pos = 0; pos < 90; pos += 2)  {                                  
    servo2.write(pos);delay(1);
  }
  delay(2000);
  for(pos = 90; pos > 0; pos -= 2)  {                                  
    servo2.write(pos);delay(1);
  }
}

int sultra1(){
  unsigned long dur ;
  // ---------- sensor 2 ----------
  digitalWrite(tri1, LOW);      // mando el pulso de para iniciar medicion
  delayMicroseconds(2);
  digitalWrite(tri1, HIGH);
  delayMicroseconds(10);
  digitalWrite(tri1, LOW);
  dur = pulseIn(eco1,HIGH,15000); // mido hasta 280cm
  if (dur > 0){
    return dur/58;
  }else{
    return 280;
  }
}

int sultra2(){
  unsigned long dur ;
  // ---------- sensor 2 ----------
  digitalWrite(tri2, LOW);      // mando el pulso de para iniciar medicion
  delayMicroseconds(2);
  digitalWrite(tri2, HIGH);
  delayMicroseconds(10);
  digitalWrite(tri2, LOW);
  dur = pulseIn(eco2,HIGH,15000); // mido hasta 280cm
  if (dur > 0){
    return dur/58;
  }else{
    return 280;
  }
}

int sultra3(){
  unsigned long dur ;
  // ---------- sensor 2 ----------
  digitalWrite(tri3, LOW);      // mando el pulso de para iniciar medicion
  delayMicroseconds(2);
  digitalWrite(tri3, HIGH);
  delayMicroseconds(10);
  digitalWrite(tri3, LOW);
  dur = pulseIn(eco3,HIGH,15000); // mido hasta 280cm
  if (dur > 0){
    return dur/58;
  }else{
    return 280;
  }
}

void in_enco1(){enco1++;}
void in_enco2(){enco2++;}
