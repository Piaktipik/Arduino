// por: Julian Galvez Serna
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
byte velo_moto_izqu = 100; // izquierdo
byte velo_moto_dere = 100; // derecho

bool obstaculo = false;

// ------------------------------------------------------ ultrasonidos ------------------------------------------------------ 
// ----------- conexion sensores ultrasonido ----------- 

byte inByte1 = 0;  // primer bit SRF02
byte inByte2 = 0;  // segundo bit SRF02

long dur = 0;     // lo uso para el SRF05
byte c = 0;        // contador de espera -> 2 bytes por serial

// ----------- conexion sensores ultrasonido ----------- 

const int eco = 22;       // pin eco SRF05
const int trigger = 24;   // pin disparo SRF05

// ------------- medida distancia sensores ultrasonido --------------- 
float medi_dist_ultr_dere,medi_dist_ultr_fron,medi_dist_ultr_izqu;
// parametros de deteccion de objeto

int dist_mini_fron = 30;       // distancia minima frontal
int dist_mini_late = 30;       // distancia minima lateral

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {

  // ----------------- comunicaciones seriales -----------------
  Serial.begin(9600);       // comunicacion con el pc - pruebas
  
  // ----------------- seriales sensores de ultrasonido laterales ----------------- 
  Serial1. begin(9600);     // serial para sensores ultasonido SRF02
  // ----------- ultasonidos - SRF05 ----------- 
  pinMode(eco,INPUT);       // pin eco SRF05
  pinMode(trigger,OUTPUT);  // pin disparo SRF05
  
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);
  
  parar(); // inicializo el robot quieto
  
  
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- programacion reactiva ----------------------------------------------------- 
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // leo sensores de ultrasonido
  
  sultraf();          // reviso sensores de ultrasonido
  sultrai();          // reviso sensores de ultrasonido
  sultrad();          // reviso sensores de ultrasonido
  
  // ------------------------------------ evado obstaculos ------------------------------------  
  obstaculo = false; // empiezo diciendo que no hay obstaculos
  // derecho
  if(medi_dist_ultr_dere < dist_mini_late) {izquierda();obstaculo = true;}
  // izquierdo
  if(medi_dist_ultr_izqu < dist_mini_late) {derecha();obstaculo = true;}
    // frontal
  if(medi_dist_ultr_fron < dist_mini_fron) {atras();delay(500);escapar();delay(500);obstaculo = true;}
  
  // si no hay obtaculo voy hacia adelante
  if(!obstaculo){adelante();}   
    
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- fin programacion reactiva -----------------------------------------------------  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
   // --------- mido los sensores de ultrasonido ---------> minimo cada 60ms 
//  if((aux_tiem_medi_ultr + 20) < millis()){
//    // prueba de los ultrasonidos
//    if(cont_orde_ultr==0){srf05_1();}
//    if(cont_orde_ultr==1){srf05_2();}
//    if(cont_orde_ultr==2){srf05_3();}
//    cont_orde_ultr=cont_orde_ultr+1;´o
//    2wif(cont_orde_ultr>=3){cont_orde_ultr=0;}
//    aux_tiem_medi_ultr = millis(); // actualizo el contador
//  }
   //delay(50);              // delay a tener en cuenta para la estabilidad de las medidas
   mostrar_serial();
  // recivo comandos por serial
  if(Serial.available() > 0){
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
   
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // funcion que se usa para ir al espacio con mayor distancia
void escapar(){
  if(medi_dist_ultr_dere <= medi_dist_ultr_izqu){izquierda();}
  else{derecha();}
}
    
void mostrar_serial(){   
  Serial.print(medi_dist_ultr_dere);
  Serial.print(" ");
  Serial.print(medi_dist_ultr_fron);
  Serial.print(" ");
  Serial.print(medi_dist_ultr_izqu);
  Serial.println();
}
   
  void datoserial(char dato){
    if(dato == 'a'){
      adelante();
      Serial.println('a');
    }
     if(dato == 'i'){
      izquierda();
      Serial.println('i');
    } 
    if(dato == 'd'){
      derecha();
      Serial.println('d');
    }
    if(dato == 'p'){
      parar();
      Serial.println('p');
    }
    if(dato == 'h'){
      atras();
      Serial.println('h');
    }
    
    if(dato == 'v'){
      delay(100); // cambiar por espera  de buffer while con tiempo
      velo_moto_izqu = Serial.read(); // vel izquierda
      velo_moto_dere = Serial.read(); // vel derecha
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

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz ultrasonidos zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

void sultrai(){
  // ---------- sensor 1 ---------- 
  // envio la orden al SRF02 de la izquierda para que mande la distancia en cm
  Serial1.write(0);
  Serial1.write(84);
  // espero que llegen los datos durante 80ms de lo contrario pongo distancia maxima
  c = 0;
  while(Serial1.available() < 2){                                      // espero a que llegen los datos
    if(c > 80){c = 0; medi_dist_ultr_izqu = 700;break;}else{delay(1);c++;}    // aborto si demora mas de 80ms
  }
  // si c > 0 fue porque llegaron los 2 datos
  if (c > 0) {
    inByte1 = Serial1.read();
    inByte2 = Serial1.read();
    medi_dist_ultr_izqu = inByte1*256 + inByte2;
  }
  if (medi_dist_ultr_izqu == 0){medi_dist_ultr_izqu = 700;}
}

void sultraf(){
  // ---------- sensor 2 ----------
  digitalWrite(trigger, LOW);      // mando el pulso de para iniciar medicion
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  medi_dist_ultr_fron = 700;
  dur = 0;
  dur = pulseIn(eco, HIGH, 50000); // cuento el tiempo que demora el pulso en alto, el proceso no puede demorar mas de 50ms
  if (dur != 0){
    medi_dist_ultr_fron = dur/58.30;
  }
}

void sultrad(){
  // ---------- sensor 3 ---------- 
  // envio la orden al SRF02 de la derecha para que mande la distancia en cm
  Serial1.write(1);
  Serial1.write(84);
  // espero que llegen los datos durante 80ms de lo contrario pongo distancia maxima
  c = 0;
  while(Serial1.available() < 2){                                      // espero a que llegen los datos
    if(c > 80){
      c = 0;
      medi_dist_ultr_dere = 700;
      break;
    }else{
      delay(1);c = c + 1;
    }    // aborto si demora mas de 80ms
  }
  // si c > 0 fue porque llegaron los 2 datos
  if (c > 0) {
    inByte1 = Serial1.read();
    inByte2 = Serial1.read();
    medi_dist_ultr_dere = inByte1*256 + inByte2;
  }
  if (medi_dist_ultr_dere == 0){medi_dist_ultr_dere = 700;}
}


// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void adelante(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras(){
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void izquierda(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha(){
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
