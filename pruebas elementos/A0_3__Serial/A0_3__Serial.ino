


// ------------ configuraciones iniciales ------------
void setup() {
  Serial.begin(9600);       // inicio comunicacion serial a 9600
}


void loop() {
  // leo el puerto analogo
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  sensorValue3 = analogRead(analogInPin3);

  outputValue1 = float(sensorValue1*5)/1023;
  outputValue2 = float(sensorValue2*5)/1023;
  outputValue3 = float(sensorValue3*5)/1023;

  // imprimo el valor leido por el ADC y luego el voltaje:
  Serial.print("s1 = " );
  Serial.print(sensorValue1);
  Serial.print("\t out1 = ");
  Serial.print(outputValue1);
  
  Serial.print("\t s2 = " );
  Serial.print(sensorValue2);
  Serial.print("\t out2 = ");
  Serial.print(outputValue2);
  
  Serial.print("\t s3 = " );
  Serial.print(sensorValue3);
  Serial.print("\t out3 = ");
  Serial.print(outputValue3);
  
  Serial.println(); // salto a la siguiente linea
  // espero 2 ms
  delay(2);
}
