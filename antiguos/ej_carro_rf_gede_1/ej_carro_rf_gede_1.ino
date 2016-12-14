
// se crea una variable para guardar los datos enviados por serial
char dato; 

//----------------------ubico motores-----------------------
// motor derecho
const int m1a = 9;
const int m1b = 10;
// motor izquierdo
const int m2a = 11;
const int m2b = 12;

//---------------configuro serial y puertos-----------------
void setup() {
  // incializo serial 
  Serial. begin(9600);
  // configuro las salidas para el l293 que controla los motores
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT); 
}
//-------------------programa principal---------------------
void loop() {
  // leo el dato y lo convierto a caracter
  dato = (char)Serial.read();
  
  // dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  if(dato == 'a'){
  digitalWrite(m1a, 1);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 1);
  digitalWrite(m2b, 0);
  }
   if(dato == 'i'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 1);
  digitalWrite(m2a, 1);
  digitalWrite(m2b, 0);
  } 
  if(dato == 'd'){
  digitalWrite(m1a, 1);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 1);
  }
  if(dato == 'p'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 0);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 0);
  }
  if(dato == 'h'){
  digitalWrite(m1a, 0);
  digitalWrite(m1b, 1);
  digitalWrite(m2a, 0);
  digitalWrite(m2b, 1);
  }  
}

