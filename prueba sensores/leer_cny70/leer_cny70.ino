float sensorValue = 0.0;  // variable to store the value coming from the sensor

void setup() {
  pinMode(13, OUTPUT); 
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT: 
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(A0);
  sensorValue = (sensorValue/1023)*5.0;
  //delay(1); 
  if (sensorValue > 4.05){
  digitalWrite(13,HIGH);
  }
  else{
  digitalWrite(13,LOW);
  }
  Serial.println(sensorValue); 
}
