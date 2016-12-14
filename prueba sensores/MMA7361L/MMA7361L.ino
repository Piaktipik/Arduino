// acelerometro mma7361l

// ------------ definicion de puertos ------------
const int pinX = A0;  // leemos puerto - acceleracion en X
const int pinY = A1;  // leemos puerto - acceleracion en Y
const int pinZ = A2;  // leemos puerto - acceleracion en Z

int intX = 0;         // valor leido - acceleracion en X
int intY = 0;         // valor leido - acceleracion en Y
int intZ = 0;         // valor leido - acceleracion en Z

float valX = 0;       // valor - acceleracion en X
float valY = 0;       // valor - acceleracion en Y
float valZ = 0;       // valor - acceleracion en Z

// parametros medicion
const int volt = 5;  // voltaje
const int ofX = 200;
const int ofY = 200;
const int ofZ = 200;
// ------------ configuraciones iniciales ------------
void setup() {
  Serial.begin(9600);       // inicio comunicacion serial a 9600
}


void loop() {
  // leo el puerto analogo
  intX = analogRead(pinX);
  intY = analogRead(pinY);
  intZ = analogRead(pinZ);

  // imprimo el valor leido por el ADC y luego el voltaje:
  if(1){
    Serial.print("X = " );
    Serial.print(intX);
    Serial.print("\t Y = " );
    Serial.print(intY);
    Serial.print("\t Z = " );
    Serial.print(intZ);
  }else{
    
    valX = float(intX*volt)/1023;
    valY = float(intY*volt)/1023;
    valZ = float(intZ*volt)/1023;
    
    Serial.print("X = " );
    Serial.print(valX);
    Serial.print("\t Y = " );
    Serial.print(valY);
    Serial.print("\t Z = " );
    Serial.print(valZ);
  }
  
  Serial.println(); // salto a la siguiente linea
  // espero 2 ms
  delay(2);
}
