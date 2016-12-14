
byte puerto_digital = 8;  //configuro el puerto a enviar

// ----- configuraciones iniciales  ----- 

void setup() {
  Serial.begin(9600);
  pinMode(puerto_digital,INPUT);
}

// --------- cogido principal ---------

void loop() {
  // leeo el puerto
  Serial.print(digitalRead(puerto_digital)); 
  // salto a la siguiente linea
  Serial.println();
  // espero un tiempo para no saturar el puerto serial
  delay(2);
}
