// por: Julian Galvez Serna - lucas tobon, juan diego cardenas, omar pino
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
#include <DueTimer.h>


// para guardar datos en la memoria sinq ue se borren al reiniciarlos
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

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

// velocidades
byte velo_moto_izqu = 60; // izquierdo
byte velo_moto_dere = 60; // derecho

// ------------- variables encoders ------------- 
int enco1 = 0;
int enco2 = 0;

bool a1 = true,a2 = true;

// ----------- conexion ventilador ----------- 
const byte fanpin = 53;    // pin ventilador

// ----------- conexion buzzer ----------- 
const byte buzpin = 51;    // pin buzzer


//------------------------------------------------------ variables infrarrojos  ------------------------------------------------------ 
 
// --------------------------------------- variables pines infrarrojos de vela --------------------------------------- 
byte senvela[19];

//byte rvela = dueFlashStorage.read(0);  // leo el valor de ajuste del ruido natural de la vela

byte ruid_vela = 10;                   // variable con el ruido normal de los sensores de vela  

bool apun_vela = false;                // variable que se activa cuando el robot ve la vela con los sensores laterales y esta rotando hacia la vela

bool apag_vela = false;                // apaganod vela

byte cont_vela = 0;                    // variable que uso para saber el numero de velas detectadas

byte aux_velo_giro = 0;                    // variable que uso para saber el numero de velas detectada

// --------------------------------------- conexion sensores infrarrojos pista  --------------------------------------- 

unsigned int aux_tiem_medi_infr = 0; // timepo de medicion infrarrojos

const byte nsen = 8;             // numero de sensores de infrarrojo

int rlinea = 2000;               // defino el timepo de espera de los sensores
//  variables usadas para conparar cada sensr de linea individualmente
int valo_refe_line1[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};
int valo_refe_line2[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};

bool medi_bool_line[15];            // vector con los valores booleanos de los sensores -> detecta o no


// -------------------- variables fisicas del sensor -------------------- 
// derecho
const byte sen_lineae1 = 32;                            // pin de abilitacion mosfet
const byte sen_linea1[] = {34,36,38,40,42,44,46,48};    // pines digitales del sensor en orden
unsigned long tiem_refl_1[nsen];                        // varialbe para calculo de la medida de la reflexion
unsigned int medi_refl_1[nsen];                         // contiene la medida de la relfexion
// izquierda
const byte sen_lineae2 = 33;                            // pin de abilitacion mosfet
const byte sen_linea2[] = {35,37,39,41,43,45,47,49};    // pines digitales del sensor
unsigned long tiem_refl_2[nsen];                        // variable para calculo de la medida de la reflexion
unsigned int medi_refl_2[nsen];                         // contiene la medida de la relfexion


byte cont_line=0;     // variable conteo sensores de linea, detectando linea
 
bool veo_line = false;                                   // se pone en alto cuando se detecta linea

// ------------------------------------------------------ ultrasonidos ------------------------------------------------------ 
// ----------- conexion sensores ultrasonido ----------- 

const byte pin_ultr_1 = 30;   // pin ultrasonido 1
const byte pin_ultr_2 = 28;   // pin ultrasonido 2
const byte pin_ultr_3 = 26;   // pin ultrasonido 3 

// ------------- variables sensores ultrasonido --------------- 
unsigned long medi_time_ultr_1,medi_time_ultr_2,medi_time_ultr_3;                      // valor que almacena el tiempo del falco en us
bool auxi_inte_ultr_1 = false,auxi_inte_ultr_2 = false,auxi_inte_ultr_3 = false;       // se usa para las interrupciones

int medi_dist_ultr_dere, medi_dist_ultr_fron, medi_dist_ultr_izqu;                     // valor que contiene la distancia medida

// parametros de deteccion de objeto

int dist_mini_fron = 15;       // distancia minima frontal
int dist_mini_late = 15;       // distancia minima lateral


byte cont_orde_ultr = 0;                     // la uso para manejar con una interrupcion los 3 sensores
unsigned int aux_tiem_medi_ultr = 0;         // lo uso para activar los ultrasonidos 

bool veo_obst = false;                       // varialbe que muestra cuando hay un objeto en frente de el robot

// ------------------------------------------------------ programa prinicipal ------------------------------------------------------ 
const byte xmax=50;
const byte ymax=74;

byte transitable[xmax][ymax];   // valor con las paredes incluidas
byte explorado[xmax][ymax];     // valor con las zonas ya exploradas
byte velas[xmax][ymax];         // valor donde se detecto una vela  


// tiempo de serial
unsigned int timeserial = 0;                      // lo uso para tener presente el tiempo  trascurrido

// pruebas - permite seleccionar diferentes fragmentos de codigo sin tener que comentar
byte tipoc = 2;

// 1 - odometria francis
// 2 - muestreo de sensores
// 3 - prueba codigo sin posicion robot
// 4 - prueba codigo con posicion robot
// 5 - 

// funcion para evitar el atrancamiento

unsigned int aux_tiem_atra = 0; 

int valo_pasa_1 = 0;
int valo_pasa_2 = 0;
int valo_pasa_3 = 0;

//bool robo_atra = false;
bool dire_busq_vela = false;
bool aux_dete_vela = true;

byte valo_masi_sens_dere = 0;
byte valo_masi_sens_izqu = 0;
// ------------------------------------------------------ variables odometria ------------------------------------------------------ 

float x_robot = float(dueFlashStorage.read(2));
float y_robot = float((dueFlashStorage.read(3)+dueFlashStorage.read(4)*255))/100;
float o_robot = float((dueFlashStorage.read(5)+dueFlashStorage.read(6)*255))/100; // posicion y orientacion de el robot


// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {
  // ----------------- incializo parametros de ubicacion de el robot ----------------- 
  x_robot = 0;
  y_robot = 0;
  o_robot = 0;
  
// ----------------- timer medicion de sensores ----------------- 
//  Timer3.attachInterrupt(srf05);
//  Timer3.start(50000); // Calls every 30ms

  // ----------------- timer algoritmo localizacion ----------------- 
//  Timer3.attachInterrupt(localizacion);
//  Timer3.start(100000); // Calls every 30ms
  
  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas
  
  // ----------------- seriales sensores de vela ----------------- 
  Serial1.begin(57600);       // lado derecho
  Serial2.begin(57600);       // lado izquierda
  
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);
  
  parar(); // inicializo el robot quieto
  
  // ------------ lo uso para detectar los ecnoders -----------------
  // attachInterrupt(pin, ISR, mode); -> solo en due
  attachInterrupt(22, in_enco1a, CHANGE);
  //attachInterrupt(23, in_enco1b, CHANGE);
  attachInterrupt(24, in_enco2a, CHANGE);
  //attachInterrupt(25, in_enco2b, CHANGE);
  
  // ----------------- vetilador ----------------- 
  pinMode(fanpin,OUTPUT);
  digitalWrite(fanpin,LOW);    // inicializo ventilador apagado
  
  // ----------------- buzzer ----------------- 
  pinMode(buzpin,OUTPUT);
  digitalWrite(buzpin,LOW);    // inicializo buzzer apagado
  
  // ----------- infrarrojos de pista ----------- 
  // declaro puertos de control del mosfet de los leds como salida
  pinMode(sen_lineae1,OUTPUT);
  pinMode(sen_lineae2,OUTPUT);
  
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- programacion reactiva ----------------------------------------------------- 
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // -objetivos: -buscar velas - evadir obstaculos -> (paredes, linea)
  // -variables de entrada 
  // ultrasonidos -> (medi_dist_ultr_dere,medi_dist_ultr_fron,medi_dist_ultr_izqu) der - fre -izq
  // infrarrojos
  //   vela ->     senvela[0 <-> 20] atraz - izq - dereha
  //   pista ->    medi_bool_line[0 <-> 15] derecho - izquierdo
  // - codigo: 
  
  //definir_dir(0); // funcion que uso para definir la direccion de movimiento
  
  
  // ------------ detecto lineas ------------ 
  
  veo_line = false;    // supongo que no estoy viendo linea
  cont_line = 0;     // variable conteo sensores de linea detectando linea
  
  for(int i=0;i<15;i++){
    if(medi_bool_line[i] == HIGH){cont_line++;}
  }
  
  if(cont_line > 1){veo_line = true;}        // si hay mas de 4 sensores de linea activos digo que veo linea
  
  // si veo vela la apago
  if((medi_dist_ultr_fron < 20 && veo_line) && apag_vela){
    atras();delayy(10);parar();fan(1);delayy(500);fan(0);atras();delayy(500);
    // voy al lugar de mayot distncia
    if(medi_dist_ultr_dere <= medi_dist_ultr_izqu && !(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela)){
      izquierda();delayy(100);
    }else{derecha();delayy(100);}
    aux_dete_vela = true;
  }  // apago las velas
  
  // ------------------------------------ evado obstaculos ------------------------------------  
  
  veo_obst = false;    // supongo que no estoy viendo obstaculos
  
  if(apun_vela){dist_mini_late = 5;}else{dist_mini_late = 15;}
  
  // derecho
  if(medi_dist_ultr_dere < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{izquierda();}veo_obst = true;}
  
  // izquierdo
  if(medi_dist_ultr_izqu < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{derecha();}veo_obst = true;}
  
    // frontal
  if(medi_dist_ultr_fron < dist_mini_fron) {atras();delayy(100);if(medi_dist_ultr_dere <= medi_dist_ultr_izqu){derecha();delayy(100);}else{izquierda();delayy(100);} veo_obst = true;}
  
  if (veo_obst && !apag_vela){goto evadiendoo;}  // si hay obstaculos no reviso la vela y evado obstaculo
  
   //  ------------ evito salirme de la pista o entrar a la vela ------------ 
  if (veo_line && !apag_vela){
    atras();velo_moto_izqu=50;velo_moto_dere=50;delayy(200);
    if(medi_dist_ultr_dere <= medi_dist_ultr_izqu){
      derecha();delayy(50);
    }else{izquierda();delayy(50);}
  } // si hay obstaculos salto el resto de codigo y los evado
  
  // ------------ busco velas ------------ 
  // deteccion de vela
  
  // aumentamos la velocidad de el robot en casi de que no vea ni linea ni vela
  
  if(!apag_vela && !apun_vela){velo_moto_izqu=50;velo_moto_dere=50;}
  
  apun_vela = false;          // suponemos que no estamos viendo vela
  apag_vela = false;          // no hay vela en frente
  
  // analizo sensores de la izquierda
  for(int i=0;i<10;i++){
    if(senvela[i] > ruid_vela){
      cont_vela++;
      aux_velo_giro = (50-(5*i)); // lo uso para girar despacion
      dire_busq_vela = false;//izquierda();
      apun_vela = true;
    }
    velo_moto_izqu = aux_velo_giro;
    velo_moto_dere = aux_velo_giro+10;
  }
  
  // analizo sensores de la derecha
  for(int i=19;i>10;i--){
    if(senvela[i]>ruid_vela){  // revisar error de velocidad
      cont_vela++;
      aux_velo_giro = (50-(5*(19-i))); // lo uso para girar despacion
      dire_busq_vela = true;//derecha();
      apun_vela = true;
    }
    velo_moto_izqu = aux_velo_giro+10;
    velo_moto_dere = aux_velo_giro;
  } 
  
  
  if (cont_vela > 0){
    // pregunto si es la primera deteccion
    if(aux_dete_vela && (senvela[4] > ruid_vela || senvela[5] > ruid_vela || senvela[6] > ruid_vela || senvela[14] > ruid_vela | senvela[15] > ruid_vela | senvela[16] > ruid_vela)){
     adelante();delayy(500);
     aux_dete_vela = false;
    } 
    // configuro la posicion
   if(dire_busq_vela){derecha();}else{izquierda();} 
  }
  
  
  
  //dist_mini_fron = 30;dist_mini_late = 30; ruid_vela=10;} 
  if (cont_vela >= 1){}//dist_mini_fron = 5;dist_mini_late = 5; ruid_vela=20;}   // disminullo la sensibilidad cuando encunetro velas

  if(senvela[10]>ruid_vela){buz(1);}else{buz(0);}
  
  if(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela){apag_vela = true;}
   
  if(senvela[10]>ruid_vela+20){velo_moto_izqu = 50;velo_moto_dere = 50; adelante();} 
  
  evadiendoo:
  
  if(!apun_vela && !veo_obst && !apag_vela && !veo_line){
    velo_moto_izqu=50;velo_moto_dere=50;adelante();
  }
  
  // funcion que revisa si el robot se ha boqueada
  
  
  delay(10);
  
  // --------- evito que el robot se atranque ---------> minimo cada 60ms 
  if((aux_tiem_atra+300) < millis()){
    if(medi_dist_ultr_dere == valo_pasa_1 && medi_dist_ultr_fron == valo_pasa_2 && medi_dist_ultr_izqu == valo_pasa_3){
      atras();velo_moto_izqu=50;velo_moto_dere=50;buz(1);delayy(200);
      if(medi_dist_ultr_dere <= medi_dist_ultr_izqu){
        izquierda();delayy(100);
      }else{derecha();delayy(100);}
      buz(0);
    }
    valo_pasa_1 = medi_dist_ultr_dere;
    valo_pasa_2 = medi_dist_ultr_fron;
    valo_pasa_3 = medi_dist_ultr_izqu;
    
    aux_tiem_atra = millis(); // actualizo el contador
  }
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- fin programacion reactiva -----------------------------------------------------  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // lo uso para mostrar los datos de los sensores
  if(tipoc == 2){
    mostrar_serial();
  }

  // lo uso para mostrar los datos en processing
  if(tipoc == 4){
    mostrar_processing();
  }
  
   // --------- mido los sensores de ultrasonido ---------> minimo cada 60ms 
  if((aux_tiem_medi_ultr+20) < millis()){
    // prueba de los ultrasonidos
    if(cont_orde_ultr==0){srf05_1();}
    if(cont_orde_ultr==1){srf05_2();}
    if(cont_orde_ultr==2){srf05_3();}
    cont_orde_ultr=cont_orde_ultr+1;
    if(cont_orde_ultr>=3){cont_orde_ultr=0;}
    aux_tiem_medi_ultr = millis(); // actualizo el contador
  }
  // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
  if((aux_tiem_medi_infr+50) < millis()){
    medirlinea();
    svela();
    aux_tiem_medi_infr = millis(); // actualizo el contador
  }
  
   delay(5);              // delay a tener en cuenta para la estabilidad de las medidas
   
  // recivo comandos por serial
  if(Serial.available() > 0){
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
   
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  void delayy(int tiempoaesperar){
    int contadort = 0;
    while (contadort<tiempoaesperar){
      if(contadort%20 == 0){
      // prueba de los ultrasonidos
      if(cont_orde_ultr==0){srf05_1();}
      if(cont_orde_ultr==1){srf05_2();}
      if(cont_orde_ultr==2){srf05_3();}
      cont_orde_ultr=cont_orde_ultr+1;
      if(cont_orde_ultr>=3){cont_orde_ultr=0;}
      }
    // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
      if(contadort%50 == 0){
        medirlinea();
        svela();
      } 
      delay(10);
      contadort += 10;
      if(tipoc == 4){
        mostrar_processing();
      }
      if(tipoc == 2){
        mostrar_serial();
      }
    }
  }
  
   
void mostrar_serial(){   
  Serial.print(medi_dist_ultr_dere);
  Serial.print(" ");
  Serial.print(medi_dist_ultr_fron);
  Serial.print(" ");
  Serial.print(medi_dist_ultr_izqu);
  Serial.print(" | ");
  
  // prueba infrarrojos de pista
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_1[i]);
    Serial.print(" ");
  }
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_2[i]);
    Serial.print(" ");
  }
  Serial.print(" | ");
  
  for(int i=0;i<16;i++){
    Serial.print(medi_bool_line[i]);
    Serial.print(" ");
  }
  
  Serial.print(" | ");
  // prueba infrarrojos de vela
  for(int i=0;i<20;i++){
    Serial.print(senvela[i]);
    Serial.print(" ");
  } 
  
  Serial.print(" | ");
  Serial.print("em1= ");
  Serial.print(enco1);
  Serial.print(" em2= ");
  Serial.print(enco2);
  Serial.println();
}
  
  void mostrar_processing(){
  
  if((timeserial+200) < millis()){
    
    Serial.write(bytesl(medi_dist_ultr_dere));
    Serial.write(bytesh(medi_dist_ultr_dere));
    Serial.write(bytesl(medi_dist_ultr_fron));
    Serial.write(bytesh(medi_dist_ultr_fron));
    Serial.write(bytesl(medi_dist_ultr_izqu));
    Serial.write(bytesh(medi_dist_ultr_izqu));
    
    // prueba infrarrojos de pista
    for(int i=0;i<16;i++){
      Serial.write(medi_bool_line[i]);
    }
    
    // prueba infrarrojos de vela
    for(int i=0;i<20;i++){
      Serial.write(senvela[i]);
    } 
    
    Serial.write(bytesl(enco1));
    Serial.write(bytesh(enco1));
    Serial.write(bytesl(enco2));
    Serial.write(bytesh(enco2));
    // mando x,y,o de el robot ()
    //float x_robot,y_robot,o_robot;
    Serial.write(bytesl(x_robot*100));
    Serial.write(bytesh(x_robot*100));
    Serial.write(bytesl(y_robot*100));
    Serial.write(bytesh(y_robot*100));
    Serial.write(bytesl(o_robot*100.0));
    Serial.write(bytesh(o_robot*100.0));
    
    Serial.print('F');
    timeserial = millis(); // actualizo el contador
  }
}
  
  
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
    if(dato == 'f'){
      fan(1); // prendo el ventilador
    }
    if(dato == 'o'){
      fan(0); // apago el ventilador
    }  
    if(dato == 'v'){
      delay(100);
      velo_moto_izqu = Serial.read(); // vel izquierda
      velo_moto_dere = Serial.read(); // vel derecha
      buz(1);
      delay(200);
      buz(0);
    }  
    if(dato == 'c'){
      delay(100);
      dueFlashStorage.write(0,Serial.read());
      dueFlashStorage.write(1,Serial.read());
      ruid_vela = dueFlashStorage.read(0);
      rlinea = 100*int(dueFlashStorage.read(1));
      for(int i=0;i<nsen;i++){
        valo_refe_line1[i] = rlinea;
        valo_refe_line2[i] = rlinea;
      }    
      buz(1);
      delay(200);
      buz(0);
    } 
    if(dato == 's'){
      delay(100);
      dueFlashStorage.write(2,Serial.read());
      
      dueFlashStorage.write(3,Serial.read());  // parte baja
      dueFlashStorage.write(4,Serial.read());  // parte alta  
      
      dueFlashStorage.write(5,Serial.read());
      dueFlashStorage.write(6,Serial.read());
      
      x_robot = float(dueFlashStorage.read(2));
      y_robot = float(dueFlashStorage.read(3)+dueFlashStorage.read(4)*255)/100;
      o_robot = float(dueFlashStorage.read(5)+dueFlashStorage.read(6)*255)*(PI/180)/100;
      //phi = o_robot;
      buz(1);
      delay(200);
      buz(0);
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

void srf05_1(){
  detachInterrupt(pin_ultr_1);
  medi_time_ultr_1 = 0;//medi_dist_ultr_dere = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_1, OUTPUT);digitalWrite(pin_ultr_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_1, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_1, INPUT);
  auxi_inte_ultr_1 = false;attachInterrupt(pin_ultr_1, func_ultr_1, RISING);auxi_inte_ultr_1 = true;
}
void func_ultr_1(){
  if (auxi_inte_ultr_1){
    if (medi_time_ultr_1 == 0){medi_time_ultr_1 = micros();attachInterrupt(pin_ultr_1, func_ultr_1, FALLING);} 
    else{medi_time_ultr_1 = micros()-medi_time_ultr_1; medi_dist_ultr_dere = medi_time_ultr_1/58.30;} 
  }
}
void srf05_2(){
  detachInterrupt(pin_ultr_2);
  medi_time_ultr_2 = 0;//medi_dist_ultr_fron = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_2, OUTPUT);digitalWrite(pin_ultr_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_2, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_2, INPUT);
  auxi_inte_ultr_2 = false;attachInterrupt(pin_ultr_2, func_ultr_2, RISING);auxi_inte_ultr_2 = true;
}
void func_ultr_2(){
  if (auxi_inte_ultr_2){
    if (medi_time_ultr_2 == 0){medi_time_ultr_2 = micros();attachInterrupt(pin_ultr_2, func_ultr_2, FALLING);}
    else{medi_time_ultr_2 = micros()-medi_time_ultr_2; medi_dist_ultr_fron = medi_time_ultr_2/58.30;}
  }
}
void srf05_3(){  
  detachInterrupt(pin_ultr_3);
  medi_time_ultr_3 = 0;//medi_dist_ultr_izqu = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_3, OUTPUT);digitalWrite(pin_ultr_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_3, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_3, INPUT);
  auxi_inte_ultr_3 = false;attachInterrupt(pin_ultr_3, func_ultr_3, RISING);auxi_inte_ultr_3 = true;
}
void func_ultr_3(){
  if (auxi_inte_ultr_3){
    if (medi_time_ultr_3 == 0){medi_time_ultr_3 = micros();attachInterrupt(pin_ultr_3, func_ultr_3, FALLING);}
    else{medi_time_ultr_3 = micros()-medi_time_ultr_3; medi_dist_ultr_izqu = medi_time_ultr_3/58.30;}
  }
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void medirlinea(){
  // enciendo leds
  digitalWrite(sen_lineae1,1);
  digitalWrite(sen_lineae2,1);
  // cargo los condesandores poniendo en alto los pines
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],OUTPUT);digitalWrite(sen_linea1[i],HIGH);
    pinMode(sen_linea2[i],OUTPUT);digitalWrite(sen_linea2[i],HIGH);
  }
  // espero 10us a que cargue el capacitor
  delayMicroseconds(20);
  // pongo los pines como entrada y espero a la interrupcion
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],INPUT);
    tiem_refl_1[i] = micros();medi_refl_1[i]=60000;medi_bool_line[i]=LOW;
    pinMode(sen_linea2[i],INPUT);
    tiem_refl_2[i] = micros();medi_refl_2[i]=60000;medi_bool_line[i+nsen]=LOW;
  }
  // activo las interupciones de los de pista s1
  attachInterrupt(sen_linea1[0],s1p0,FALLING);
  attachInterrupt(sen_linea1[1],s1p1,FALLING);
  attachInterrupt(sen_linea1[2],s1p2,FALLING);
  attachInterrupt(sen_linea1[3],s1p3,FALLING);
  attachInterrupt(sen_linea1[4],s1p4,FALLING);
  attachInterrupt(sen_linea1[5],s1p5,FALLING);
  attachInterrupt(sen_linea1[6],s1p6,FALLING);
  attachInterrupt(sen_linea1[7],s1p7,FALLING);
  // activo las interupciones de los de pista s2
  attachInterrupt(sen_linea2[0],s2p0,FALLING);
  attachInterrupt(sen_linea2[1],s2p1,FALLING);
  attachInterrupt(sen_linea2[2],s2p2,FALLING);
  attachInterrupt(sen_linea2[3],s2p3,FALLING);
  attachInterrupt(sen_linea2[4],s2p4,FALLING);
  attachInterrupt(sen_linea2[5],s2p5,FALLING);
  attachInterrupt(sen_linea2[6],s2p6,FALLING);
  attachInterrupt(sen_linea2[7],s2p7,FALLING);
  // apago leds
  digitalWrite(sen_lineae1,0);
  digitalWrite(sen_lineae2,0);
}

void s1p0(){medi_refl_1[0] = micros()-tiem_refl_1[0];if(medi_refl_1[0] < valo_refe_line1[0]){medi_bool_line[0]=HIGH;}else{medi_bool_line[0]=LOW;}}
void s1p1(){medi_refl_1[1] = micros()-tiem_refl_1[1];if(medi_refl_1[1] < valo_refe_line1[1]){medi_bool_line[1]=HIGH;}else{medi_bool_line[1]=LOW;}}
void s1p2(){medi_refl_1[2] = micros()-tiem_refl_1[2];if(medi_refl_1[2] < valo_refe_line1[2]){medi_bool_line[2]=HIGH;}else{medi_bool_line[2]=LOW;}}
void s1p3(){medi_refl_1[3] = micros()-tiem_refl_1[3];if(medi_refl_1[3] < valo_refe_line1[3]){medi_bool_line[3]=HIGH;}else{medi_bool_line[3]=LOW;}}
void s1p4(){medi_refl_1[4] = micros()-tiem_refl_1[4];if(medi_refl_1[4] < valo_refe_line1[4]){medi_bool_line[4]=HIGH;}else{medi_bool_line[4]=LOW;}}
void s1p5(){medi_refl_1[5] = micros()-tiem_refl_1[5];if(medi_refl_1[5] < valo_refe_line1[5]){medi_bool_line[5]=HIGH;}else{medi_bool_line[5]=LOW;}}
void s1p6(){medi_refl_1[6] = micros()-tiem_refl_1[6];if(medi_refl_1[6] < valo_refe_line1[6]){medi_bool_line[6]=HIGH;}else{medi_bool_line[6]=LOW;}}
void s1p7(){medi_refl_1[7] = micros()-tiem_refl_1[7];if(medi_refl_1[7] < valo_refe_line1[7]){medi_bool_line[7]=HIGH;}else{medi_bool_line[7]=LOW;}}

void s2p0(){medi_refl_2[0] = micros()-tiem_refl_2[0];if(medi_refl_2[0] < valo_refe_line2[0]){medi_bool_line[8]=HIGH;}else{medi_bool_line[8]=LOW;}}
void s2p1(){medi_refl_2[1] = micros()-tiem_refl_2[1];if(medi_refl_2[1] < valo_refe_line2[1]){medi_bool_line[9]=HIGH;}else{medi_bool_line[9]=LOW;}}
void s2p2(){medi_refl_2[2] = micros()-tiem_refl_2[2];if(medi_refl_2[2] < valo_refe_line2[2]){medi_bool_line[10]=HIGH;}else{medi_bool_line[10]=LOW;}}
void s2p3(){medi_refl_2[3] = micros()-tiem_refl_2[3];if(medi_refl_2[3] < valo_refe_line2[3]){medi_bool_line[11]=HIGH;}else{medi_bool_line[11]=LOW;}}
void s2p4(){medi_refl_2[4] = micros()-tiem_refl_2[4];if(medi_refl_2[4] < valo_refe_line2[4]){medi_bool_line[12]=HIGH;}else{medi_bool_line[12]=LOW;}}
void s2p5(){medi_refl_2[5] = micros()-tiem_refl_2[5];if(medi_refl_2[5] < valo_refe_line2[5]){medi_bool_line[13]=HIGH;}else{medi_bool_line[13]=LOW;}}
void s2p6(){medi_refl_2[6] = micros()-tiem_refl_2[6];if(medi_refl_2[6] < valo_refe_line2[6]){medi_bool_line[14]=HIGH;}else{medi_bool_line[14]=LOW;}}
void s2p7(){medi_refl_2[7] = micros()-tiem_refl_2[7];if(medi_refl_2[7] < valo_refe_line2[7]){medi_bool_line[15]=HIGH;}else{medi_bool_line[15]=LOW;}}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de vela zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void svela(){
  Serial1.print("R");
  Serial1.print("L");
}

void serialEvent1(){
  // Serial1.read();
  for(int i=0;i<11;i++){
    senvela[i] = Serial1.read();
  }
 while(Serial1.available()>0){Serial1.read();}  
}

void serialEvent2(){
  for(int i=11;i<20;i++){
    senvela[i] = Serial2.read();
  }
 while(Serial2.available()>0){Serial2.read();}  
}

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
  a1=1;a2=1;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras(){
  a1=0;a2=0;
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha(){
  a1=1;a2=0;
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void izquierda(){
  a1=0;a2=1;
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}  

// ---------------- motores ---------------- 
// 1 - adelante
// 0 - atraz
void motor_d(bool dir,byte vel){
  digitalWrite(pin_dire_moto_izqu, dir);      // pin direccion
  analogWrite(pin_velo_moto_izqu, vel);       // pin velodcidad
}
void motor_i(bool dir,byte vel){
  digitalWrite(pin_dire_moto_dere, dir);      // pin direccion
  analogWrite(pin_velo_moto_dere, vel);       // pin velocidad
}
// ---------------- ventilador ----------------
void fan(bool v){
  digitalWrite(fanpin,v); 
}
// ---------------- buzzer ----------------
void buz(bool v){
  digitalWrite(buzpin,v); 
}

