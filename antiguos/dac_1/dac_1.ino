// por: Julian Galvez Serna, juan diego cardenas
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ------------------------------------------------------ motores y actuadores ------------------------------------------------------ 
// ----------- conexion motores ----------- 
// ----- motores ----- 
// motor izquierdo - m2
const byte pin_dire_moto_izqu = 4;
const byte pin_velo_moto_izqu = 5;

// motor derecho - m1
const byte pin_dire_moto_dere = 7;  
const byte pin_velo_moto_dere = 6;

// variables que contiene la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_izqu = 70; // izquierdo
byte velo_moto_dere = 75; // derecho

// variables para tiempos de muestreo
float vel_medida = 0; 
int enco1_p = 0;

// ------------- variables encoders ------------- 
int enco1 = 0;
int enco2 = 0;

bool a1 = true,a2 = true;
// ------------------------------------------------------ programa principal ------------------------------------------------------ 

// tiempo de serial
unsigned int timeserial = 0;                      // lo uso para tener presente el tiempo  trascurrido


// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {
  
  // ----------------- comunicaciones seriales -----------------
  Serial.begin(9600);       // comunicacion con el pc - pruebas
  
  // ----------------- seriales sensores de vela ----------------- 
  
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);
  
  parar(); // inicializo el robot quieto
  
  // ------------ lo uso para detectar los ecnoders -----------------
  // attachInterrupt(pin, ISR, mode); -> solo en due
  attachInterrupt(0, in_enco1a, CHANGE);
  //attachInterrupt(23, in_enco1b, CHANGE);
  attachInterrupt(1, in_enco2a, CHANGE);
  //attachInterrupt(25, in_enco2b, CHANGE);
  
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- programacion lectura ----------------------------------------------------- 
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // tiempo en milisegundos
  // porcentaje de PWM 
  // porcentaje de velocidad angular
  if (millis() > timeserial + 20){
    Serial.print(millis());
    Serial.print(',');
    Serial.print(enco1);
    Serial.print(',');
    vel_medida = (enco1_p - enco1)/(millis() - timeserial);
    enco1_p = enco1;
    timeserial = millis();
    Serial.print(vel_medida);
  }
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- fin programacion lectura -----------------------------------------------------  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // recivo comandos por serial
  if(Serial.available() > 0){
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
   
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  void datoserial(char dato){
    if(dato == 'a'){
      adelante();
    }
     if(dato == 'i'){
      izquierda();
    } 
    if(dato == 'd'){
      derecha();
    }
    if(dato == 'p'){
      parar();
    }
    if(dato == 'h'){
      atras();
    }
    if(dato == 'v'){
      delay(50);
      velo_moto_izqu = Serial.read(); // vel izquierda
      //velo_moto_dere = Serial.read(); // vel derecha
    }  
  }

// conversion de 10bits to 2 bytes
  byte bytesh(int val){
    return val/256;
  }
  
   byte bytesl(int val){
     return val;
  }
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz sub- funciones sensores zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz encoders zzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// incremento los respectivos contadores de los encoders
void in_enco1a(){
  if(a1){enco1++;}
  else{enco1--;}
}

void in_enco2a(){
  if(a2){enco2++;}
  else{enco2--;}
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void adelante(){
  a1=1;a2=1;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras(){
  a1=0;a2=0;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha(){
  a1=1;a2=0;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void izquierda(){
  a1=0;a2=1;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}

