
// ----------- puerto lm35 -------
// conexion: 1 - +VS, 2 - VOUT, 3 - GND
const int analogInPin = A0;   // pin sensor
float sensorValue = 0;        // variable para guardar sensor

void setup() {
  // inicio en envio de datos
  Serial.begin(9600);
}

void loop() {
  // leeo el sensor
  sensorValue = analogRead(analogInPin);

  // print the results to the serial monitor:
  Serial.print("sensor = " );
  Serial.print(sensorValue);
  
  // -> relacion 10mV/C
  // ADC 1024 -> 5V |-> 5V/1024 -> 0.0048828 -> 0.0048828/0.01 -> 0.48828 
  
  sensorValue = sensorValue*0.48828;
  
  Serial.print("\t output = ");
  Serial.print(sensorValue);
  
  Serial.println();
  
  delay(2);
}
