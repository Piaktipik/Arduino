
// se crea una variable para guardar los datos enviados por serial
char dato; 
int m1p = 255; // (0-255)
int m2p = 255; // (0-255)
//----------------------ubico motores-----------------------
// abilitar motores
const int m1e = 10;
const int m2e = 11;

// motor derecho
const int m1a = 4;
const int m1b = 5;
// motor izquierdo
const int m2a = 6;
const int m2b = 7;

//---------------configuro serial y puertos-----------------
void setup() {
  // incializo serial 
  Serial. begin(57600);
  // configuro las salidas para el l293 que controla los motores
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT); 
  pinMode(m1e, OUTPUT);
  pinMode(m2e, OUTPUT); 
}
//-------------------programa principal---------------------
void loop() {
//  analogWrite(m1e, m1p);
//  analogWrite(m2e, m2p);
  digitalWrite(m1e, 1);
  digitalWrite(m2e, 1);
  // leo el dato y lo convierto a caracter
  if(Serial.available() > 0){
  dato = (char)Serial.read();
  Serial.print(dato);
  }
  // dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  if(dato == 'a'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 1);
  digitalWrite(m2a, 1);
  digitalWrite(m2b, 0);
  //Serial.print('a');
  }
   if(dato == 'i'){
  digitalWrite(m1a, 1);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 1);
  digitalWrite(m2b, 0);
  //Serial.print('i');
  } 
  if(dato == 'd'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 1);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 1);
  //Serial.print('d');
  }
  if(dato == 'p'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 0);
  //Serial.print('p');
  }
  if(dato == 'h'){
  digitalWrite(m1a, 1);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 1);
  //Serial.print('h');
  }  
}
