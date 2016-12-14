
// ------------ definicion de puertos ------------
const int analogInPin = A0;  // leemos puerto analogo A0


int sensorValue = 0;         // valor leido del puerto

float outputValue = 0;       // valor de voltaje


// ------------ configuraciones iniciales ------------
void setup() {
  Serial.begin(57600);       // inicio comunicacion serial a 9600
}


void loop() {
  // leo el puerto analogo
  sensorValue = analogRead(analogInPin);

  outputValue = float(sensorValue*5)/1023;

  // imprimo el valor leido por el ADC y luego el voltaje:
  Serial.print("sensor = " );
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.print(outputValue);
  
  Serial.println(); // salto a la siguiente linea
  // espero 2 ms
  delay(2);
}
