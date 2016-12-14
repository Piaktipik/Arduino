int sensorPin = A1;    // select the input pin for the potentiometer
//int ledPin = 13;      // select the pin for the LED
int analoga = 0;  // variable to store the value coming from the sensor
int maximo=0, minimo=1000;
int ultimo=0;
//int inByte = 0;         // incoming serial byte

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); 
  pinMode(13, OUTPUT); 
}

void loop() {
  //analogWrite(2,175);
  // read the value from the sensor:1
  
  analoga = analogRead(sensorPin);
  
  if (analoga > 800){
    digitalWrite(13,LOW);  
  }
  else{
    digitalWrite(13,HIGH);
  }
  
//  if(analoga > ultimo +5 || analoga < ultimo -5){ultimo=analoga;}
//  else{
//    if (analoga > maximo){
//    maximo = analoga;
//    }
//    if (analoga < minimo) {
//    minimo=analoga;
//    } 
//  } 
//  Serial.print(" ");
//  Serial.print(minimo);
//  Serial.print(" , ");
//  Serial.print(maximo);
//  Serial.print(" , ");

  Serial.println(analoga);
}
