//prueba de sensor de corriente ACS711

const int analogInPin = A0;  // pin de lectura de el sensor
const int x = 529 ;          // valor i=0
float sensorValue = 0;       // valor de la lectura

void setup() {
  // inicio comunicacion serial
  Serial.begin(9600); 
}

void loop() {
  // leo el valor analogo
  sensorValue = analogRead(analogInPin);
  
  // mapeo los datos
  if (sensorValue > x)
  sensorValue = ((sensorValue-x)/(1024-x))*(12+1.9);
  
  else
  sensorValue = ((sensorValue-x)/(x))*(12+2.65);
  
  // muestro por pantalla
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);      

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                     
}
