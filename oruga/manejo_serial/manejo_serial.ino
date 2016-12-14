// por: Julian Galvez Serna
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// sensores:
// - 2 srf 02
// - 1 hc-srf 04

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ------------------------------------------------------ motores y actuadores ------------------------------------------------------ 


// varialbles que contiene la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_izqu = 100; // izquierdo
byte velo_moto_dere = 100; // derecho

bool obstaculo = false;
bool evado = false;
// ------------------------------------------------------ ultrasonidos ------------------------------------------------------ 
// ----------- conexion sensores ultrasonido ----------- 

byte inByte1 = 0;  // primer bit SRF02
byte inByte2 = 0;  // segundo bit SRF02

long dur = 0;     // lo uso para el SRF05
int c = 0;        // contador de espera -> 2 bytes por serial

// ----------- conexion sensores ultrasonido ----------- 

const int eco = 22;       // pin eco SRF05
const int trigger = 24;   // pin disparo SRF05

// ------------- medida distancia sensores ultrasonido --------------- 
float medi_dist_ultr_dere,medi_dist_ultr_fron,medi_dist_ultr_izqu;
// parametros de deteccion de objeto

int dist_mini_fron = 50;       // distancia minima frontal
int dist_mini_late = 50;       // distancia minima lateral

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {

  // ----------------- comunicaciones seriales -----------------
  Serial.begin(9600);       // comunicacion con el pc - pruebas
  Serial3. begin(115200);     // serial para sensores ultasonido SRF02
  // ----------------- seriales sensores de ultrasonido laterales ----------------- 
  Serial1. begin(9600);     // serial para sensores ultasonido SRF02
  // ----------- ultasonidos - SRF05 ----------- 
  pinMode(eco,INPUT);       // pin eco SRF05
  pinMode(trigger,OUTPUT);  // pin disparo SRF05
  
  // ----------------- motores ----------------- 
  iniciar_motores();
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- programacion reactiva ----------------------------------------------------- 
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // leo sensores de ultrasonido
  
  if(evado){
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
  }  
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
//   mostrar_serial();
  // recivo comandos por serial
  if(Serial3.available() > 0){
    datoserial(Serial3.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
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
    if(dato == 'e'){
      evado = !evado;
      parar();
    }
    if(dato == 'v'){
      delay(100);
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
  Serial1.write(1);
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
  Serial1.write(0);
  Serial1.write(84);
  // espero que llegen los datos durante 80ms de lo contrario pongo distancia maxima
  c = 0;
  while(Serial1.available() < 2){                                      // espero a que llegen los datos
    if(c > 80){c = 0; medi_dist_ultr_dere = 700;break;}else{delay(1);c = c + 1;}    // aborto si demora mas de 80ms
  }
  // si c > 0 fue porque llegaron los 2 datos
  if (c > 0) {
    inByte1 = Serial1.read();
    inByte2 = Serial1.read();
    medi_dist_ultr_dere = inByte1*256 + inByte2;
  }
  if (medi_dist_ultr_dere == 0){medi_dist_ultr_dere = 700;}
}



