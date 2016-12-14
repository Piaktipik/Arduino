/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */
byte values[1];
// the setup routine runs once when you press reset:
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int valor = analogRead(A0);
  values[0]=bytesh(valor);
  values[1]=bytesl(valor);
  Serial.print(valor);
  Serial.print("\t");
  Serial.print(values[0]);
  Serial.print("\t");
  Serial.print(values[1]);
  Serial.println("\t");
  delay(100);        // delay in between reads for stability
}

// conversion de 10bits to 2 bytes
  byte bytesh(int val){
    return val >> 8;
  }
  
   byte bytesl(int val){
     return val-1;
  }
