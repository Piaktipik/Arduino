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

// varialbles que contiene la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_izqu = 70; // izquierdo
byte velo_moto_dere = 75; // derecho

byte velo_esta = 70;      // velocidad entandar de el desplazamiento
byte valo_ajus_velo = 5;  // valor de ajuste -> se le suma al motor derecho



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

byte ruid_vela = 20;                   // variable con el ruido normal de los sensores de vela ------------------------------------------------------- referencia velas --------------------------------------

byte defe_ruid_vela = 20;              // valor normal del ruido de la vela

bool apun_vela = false;                // variable que se activa cuando el robot ve la vela con los sensores laterales y esta rotando hacia la vela

bool apag_vela = false;                // apaganod vela

byte cont_vela = 0;                    // variable que uso para saber el numero de velas detectadas

byte aux_velo_giro = 0;                // variable que uso para ajustar la velocidad de giro hacia la vela

unsigned int tiem_apag = 700;         // tiempo que el robot usa para apagar la vela en ms ---------------------------------------------------------- timepo de apagado vela ---------------------------------

unsigned long ulti_vist_vela = 0;      // variable usada para reiniciar la deteccion de vela en caso de perderla

byte maxi_valo_vela = 0;               // maximo valor de vela detectado - se usa para preorizar en caso de encontrar varias velas

// --------------------------------------- conexion sensores infrarrojos pista  --------------------------------------- 

unsigned int aux_tiem_medi_infr;   // timepo de medicion infrarrojos -> guardo el tiempo desde la ultima medicion

//unsigned int peri_medi_infr = 5; // periodo de medicion infrarrojos ms

const byte nsen = 8;               // numero de sensores de infrarrojo

int rlinea = 7000;                // defino el timepo de espera de los sensores -------------------------------------------------------------------------- referencia linea ------------------------------------
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


byte cont_line_dere = 0;                                // variable conteo sensores de linea derechos, detectando linea
byte cont_line_izqu = 0;                                // variable conteo sensores de linea izquierdo, detectando linea

bool veo_line_fren = false;                             // se pone en alto cuando se detecta linea de frente
bool veo_line_late = false;                             // se pone en alto cuando se detecta linea de lateralmente

// ------------------------------------------------------ ultrasonidos ------------------------------------------------------ 
// ----------- conexion sensores ultrasonido ----------- 

const byte pin_ultr_1 = 30;   // pin ultrasonido 1
const byte pin_ultr_2 = 28;   // pin ultrasonido 2
const byte pin_ultr_3 = 26;   // pin ultrasonido 3 

// ------------- variables sensores ultrasonido --------------- 
unsigned long medi_time_ultr_1,medi_time_ultr_2,medi_time_ultr_3;                      // valor que almacena el tiempo del flanco en us
bool auxi_inte_ultr_1 = false,auxi_inte_ultr_2 = false,auxi_inte_ultr_3 = false;       // se usa para las interrupciones

int medi_dist_ultr_dere, medi_dist_ultr_fron, medi_dist_ultr_izqu;                     // valor que contiene la distancia medida

// parametros de deteccion de objeto

int dist_mini_fron = 15;       // distancia minima frontal
int dist_mini_late = 15;       // distancia minima lateral

byte cont_orde_ultr = 0;                     // la uso para manejar con una interrupcion los 3 sensores
unsigned int aux_tiem_medi_ultr = 0;         // lo uso para activar los ultrasonidos 

bool veo_obst = false;                       // varialbe que muestra cuando hay un objeto en frente de el robot

// ------------------------------------------------------ programa principal ------------------------------------------------------ 
const byte xmax = 25;
const byte ymax = 37;

//byte transitable[xmax][ymax];   // valor con las paredes incluidas
byte explorado[xmax][ymax];     // valor con las zonas ya exploradas
//byte velas[xmax][ymax];         // valor donde se detecto una vela  


// tiempo de serial
unsigned int timeserial = 0;                      // lo uso para tener presente el tiempo  trascurrido

// pruebas - permite seleccionar diferentes fragmentos de codigo sin tener que comentar
byte tipoc = 3;

// 1 - muestreo terminal
// 2 - muestreo processing 
// 3 - muestreo terminal - reactivo

// ----------------- funcion para evitar el atrancamiento ----------------- 

unsigned int aux_tiem_atra = 0; 

int valo_pasa_1 = 0;
int valo_pasa_2 = 0;
int valo_pasa_3 = 0;

// auxiliares deteccion de vela
bool dire_busq_vela = false;         // la uso para guardar el valor de giro, a la derecha o la izquierda
//bool aux_dete_vela = true;           // varialble que uso para saver si acabo de ver una vela por primera vez

//byte valo_masi_sens_dere = 0;
//byte valo_masi_sens_izqu = 0;
// ------------------------------------------------------ variables odometria ------------------------------------------------------ 

//float x_robot = float(dueFlashStorage.read(2));
//float y_robot = float((dueFlashStorage.read(3)+dueFlashStorage.read(4)*255))/100;
//float o_robot = float((dueFlashStorage.read(5)+dueFlashStorage.read(6)*255))/100; // posicion y orientacion de el robot

float x_robot;  // posicion x  de el robot
float y_robot;  // posicion y  de el robot
float o_robot;  // orientacion de el robot

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
  
  if (tipoc == 3){
  
  // -objetivos: -buscar velas - evadir obstaculos -> (paredes, linea)
  // -variables de entrada 
  // ultrasonidos -> (medi_dist_ultr_dere,medi_dist_ultr_fron,medi_dist_ultr_izqu) der - fre -izq
  // infrarrojos
  //   vela ->     senvela[0 <-> 20] atraz - izq - dereha
  //   pista ->    medi_bool_line[0 <-> 15] derecho - izquierdo
  // - codigo: 
  
  //definir_dir(0); // funcion que uso para definir la direccion de movimiento
  
  
  // ------------------------------------ detecto lineas ------------------------------------  
  
  veo_line_fren = false;    // supongo que no estoy viendo linea de frente
  veo_line_late = false;    // supongo que no estoy viendo linea de lado
  
  cont_line_dere = 0;     // variable conteo sensores de linea derechos detectando linea
  cont_line_izqu = 0;     // variable conteo sensores de linea izquierdos detectando linea
  
  // cuento las detecciones de el lado derecho
  for(int i=0;i<8;i++){
    if(medi_bool_line[i] == HIGH){cont_line_dere++;}
  }
  // cuento las detecciones de el lado izquierdo
  for(int i=8;i<15;i++){
    if(medi_bool_line[i] == HIGH){cont_line_izqu++;}
  }
  
  if(cont_line_dere > 0 && cont_line_izqu > 0){veo_line_fren = true;}        // si hay linea en frente
  if(cont_line_dere > 1 || cont_line_izqu > 1){veo_line_late = true;}        // si veo con dos sensores linea de lado
  
  // ------------------------------------ apago la vela ------------------------------------  
  
  // si veo vela la apago, y la veo si: estoy a 20cm de un obstaculo y veo una linea de frente y estoy en frente de algo caliente
  if((medi_dist_ultr_fron < 20 && veo_line_fren) && apag_vela){
    atras();delayy(10);                    // freno rapidamente el motor
    parar();fan(1);delayy(tiem_apag);      // me detengo prendo el ventilador y espero el timepo de apagado
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo; // configuro velocidades
    fan(0);atras();delayy(200);            // apago el ventilador marcho atraz por 400 ms
    // voy al lugar de mayor distancia
    
    if(!(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela)){escapar();delayy(200);} // giro solo si la vela esta apagada
  }
  
  // ------------------------------------ evado obstaculos ------------------------------------  
  
  veo_obst = false;    // supongo que no estoy viendo obstaculos
  
  if(apun_vela){dist_mini_late = 5;}else{dist_mini_late = 15;}      // ajusto la distancia de evacion
  // derecho
  if(medi_dist_ultr_dere < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{izquierda();}veo_obst = true;}
  // izquierdo
  if(medi_dist_ultr_izqu < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{derecha();}veo_obst = true;}
    // frontal
  if(medi_dist_ultr_fron < dist_mini_fron) {atras();delayy(200);escapar();delayy(200);veo_obst = true;}
  
  if (veo_obst && !apag_vela && !veo_line_late){goto evadiendoo;}  // si hay obstaculos y no estoy apagando una vela y no estoy evadiendo linea -> no reviso la vela y evado obstaculo
  
   // ----------------------------------------------- evito salirme de la pista o entrar a la vela apagada ------------------------------------------- 
  // retrocedo y giro hacia donde alla detectado menos lineas
  if (!apag_vela){
    velo_moto_izqu = velo_esta;velo_moto_dere = velo_esta + valo_ajus_velo;  // configuro velocidad de motores
    atras();delayy(500);          // retrocedo ya que la linea esta de frente
    if(cont_line_dere > cont_line_izqu){izquierda();}else{derecha();}
    delayy(150);      // escapo para el lado con al cual tenga menos sensores en alto 
  }
  
  // giro levemente
  else if(!apag_vela && !apun_vela){
    velo_moto_izqu = velo_esta;velo_moto_dere = velo_esta + valo_ajus_velo;  // configuro velocidad de motores
    atras();delay(500);          // retrocedo ya que la linea esta de frente
    if(cont_line_dere > cont_line_izqu){izquierda();}else{derecha();}
    delayy(150);      // escapo para el lado con al cual tenga menos sensores en alto 
  }
  
  // ------------------------------------------------------------------ busco velas ------------------------------------------------------------------ 
  // deteccion de vela
  
  // aumentamos la velocidad de el robot, ya que cuando esta apuntando la reduce - cuidado con que no vea ni la linea o la vela
  if(!apag_vela && !apun_vela){
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;
  }
  // me desplazo hacia adelante en caso de que no este pasando nada
  if(!apag_vela && !apun_vela && !veo_obst){
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;
    adelante();
  }
  
  // pregunto si estoy apuntando a vela, para en cazo de ver mas de una vela a la vez no quedar oscilando
  
  // si pierdo de vista la vela durante mas de 500 ms la doy por perdida
  if (ulti_vist_vela + 500 < millis()){
    apun_vela = false;          // suponemos que no estamos girando hacia la vela
    apag_vela = false;          // no hay vela en frente
  }
  
  maxi_valo_vela = 0; // reinicio el maximo valor de vela detectado
  cont_vela = 0;      // reinicio el contador de velas 
  
  // si he visto vela y he girado a la derecha sigo girando a la derecha
  if(apun_vela && dire_busq_vela){
    goto giro_derecha;
  }
  // analizo sensores de la izquierda
  for(int i=0;i<10;i++){
    if(senvela[i] > ruid_vela){
      cont_vela++;
      aux_velo_giro = (velo_esta-((velo_esta/10)*i)); // lo uso para girar despacion
      // si el valor de vela detectado es el mayor de todos giro en esa direccion
      if (senvela[i] > maxi_valo_vela){
        maxi_valo_vela = senvela[i];     // guardo el maximo valor
        dire_busq_vela = false;          //izquierda();
        // ajusto la velocidad
        velo_moto_izqu = aux_velo_giro;
        velo_moto_dere = aux_velo_giro + valo_ajus_velo;
        ulti_vist_vela = millis();  // actualizo la ultima vista de la vela
      }
    } // fin si veo vela
    
  }
  
  // si he visto vela y he girado a la izquierda sigo girando a la izquierda
  if (apun_vela && !dire_busq_vela){
    goto giro_izquierda;
  }
  giro_derecha:          // salto giro a la izquierda si estaba girando a la derecha
  
  // analizo sensores de la derecha
  for(int i=19;i>10;i--){
    if(senvela[i]>ruid_vela){  // revisar error de velocidad
      cont_vela++;
      aux_velo_giro = (velo_esta-((velo_esta/10)*(19-i))); // lo uso para girar despacion
      // si el valor de vela detectado es el mayor de todos giro en esa direccion
      if (senvela[i] > maxi_valo_vela){
        maxi_valo_vela = senvela[i];     // guardo el maximo valor
        dire_busq_vela = true;//derecha();
        // ajusto la velocidad
        velo_moto_izqu = aux_velo_giro;
        velo_moto_dere = aux_velo_giro + valo_ajus_velo;
        ulti_vist_vela = millis();  // actualizo la ultima vista de la vela
      } 
    }// fin si veo vela 
  } 
  giro_izquierda:      // salto  giro a la derecha si estaba girando a la izquierda
  
  if (cont_vela > 0){
    apun_vela = true;
    // configuro la posicion
    if(dire_busq_vela){derecha();}else{izquierda();} 
  } // si veo la vela con un sensor pongo apuntar vela en true

  //}
  
  
  //dist_mini_fron = 30;dist_mini_late = 30; ruid_vela=10;} 
  
  // ----------- porbar ajuste dinamico de sensivilidad de la vela -------------
  // disminullo la sensibilidad cuando encuentro velas
//  if (cont_vela > 0){
//    if(maxi_valo_vela - defe_ruid_vela > ruid_vela){   // osea si obtengo un valor de ruido mayor - menos sensibilidad ante el ruido
//      ruid_vela = maxi_valo_vela - defe_ruid_vela;     // uso el nuevo ruido
//    }
//  }
//  else{
//    ruid_vela = defe_ruid_vela;
//  }   

  //if(senvela[10] > ruid_vela){buz(1);}else{buz(0);}
  
  // ------------------------------------------ me acerco a la vela ------------------------------------------ 
  // si la vela enta en frente me acerco a ella
  
  if(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela){apag_vela = true;apun_vela = false;}
   
  // si la vela esta en todo el frente
  if(senvela[10] > ruid_vela){
    // ajusto la velocidades al maximo
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;  
    // me acerco al robot
    adelante();                          
  } 
  
  evadiendoo:    // si detecto obejeto con los ultrasonidos salto aca
  
  
//  if(!apun_vela && !veo_obst && !apag_vela && !veo_line){
//    velo_moto_izqu =  velo_esta;
//    velo_moto_dere = velo_esta + valo_ajus_velo;adelante();
//  }
  
  
  
  // ------------------------------------ funcion que revisa si el robot se ha boqueada ------------------------------------ 
  
  // --------- evito que el robot se atranque ---------> minimo cada 60ms (tiempo entre medidas ultrasonidos) 
  if((aux_tiem_atra + 100) < millis()){
    if(medi_dist_ultr_dere == valo_pasa_1 && medi_dist_ultr_fron == valo_pasa_2 && medi_dist_ultr_izqu == valo_pasa_3){
      // configuro las velocidades
      velo_moto_izqu = velo_esta;
      velo_moto_dere = velo_esta + valo_ajus_velo;
      atras();buz(1);
      delayy(500);
      buz(0);
      if(dire_busq_vela){derecha();}else{izquierda();}delayy(200);  // tomo decicion de nuevo rumbo en la direccion contraria al giro que el robot hizo hacia la vela
      adelante();
      delayy(400);
    }
    valo_pasa_1 = medi_dist_ultr_dere;
    valo_pasa_2 = medi_dist_ultr_fron;
    valo_pasa_3 = medi_dist_ultr_izqu;
    
    aux_tiem_atra = millis(); // actualizo el contador
  }
  
  } // fin tipoc==3
  //delay(10);
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- fin programacion reactiva -----------------------------------------------------  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // lo uso para mostrar los datos de los sensores
  if(tipoc == 1 || tipoc == 3){
    mostrar_serial();
  }

  // lo uso para mostrar los datos en processing
  if(tipoc == 2){
    mostrar_processing();
  }
  
   // --------- mido los sensores de ultrasonido ---------> minimo cada 60ms 
  if((aux_tiem_medi_ultr + 20) < millis()){
    // prueba de los ultrasonidos
    if(cont_orde_ultr==0){srf05_1();}
    if(cont_orde_ultr==1){srf05_2();}
    if(cont_orde_ultr==2){srf05_3();}
    cont_orde_ultr=cont_orde_ultr+1;
    if(cont_orde_ultr>=3){cont_orde_ultr=0;}
    aux_tiem_medi_ultr = millis(); // actualizo el contador
  }
  // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
  if((aux_tiem_medi_infr + 20) < millis()){
    //medirlinea();
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
      if(contadort % 20 == 1){
      // prueba de los ultrasonidos
      if(cont_orde_ultr==0){srf05_1();}
      if(cont_orde_ultr==1){srf05_2();}
      if(cont_orde_ultr==2){srf05_3();}
      cont_orde_ultr = cont_orde_ultr+1;
      if(cont_orde_ultr>=3){cont_orde_ultr=0;}    // reinicio el contador
      }
    // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
      if(contadort % 10 == 1){
        medirlinea();
        svela();
      }
      
      delay(10);
      contadort += 10;
      // muestro datos mietras espero 
      if(tipoc == 2){
        mostrar_processing();
      }
      if(tipoc == 1){
        mostrar_serial();
      }
    }
  }
  
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
      fan(1);  // prendo el ventilador
    }
    if(dato == 'b'){
      buz(1);  // prendo el buzzer
    }
    if(dato == 'o'){
      fan(0);  // apago el ventilador
      buz(0);  // apago el buzzer
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
      rlinea = int(dueFlashStorage.read(1))*100;
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
      dueFlashStorage.write(2,Serial.read());  // x baja
      
      dueFlashStorage.write(3,Serial.read());  // y baja
      dueFlashStorage.write(4,Serial.read());  // y alta
      
      dueFlashStorage.write(5,Serial.read());  // o baja
      dueFlashStorage.write(6,Serial.read());  // o alta
      
      x_robot = float(dueFlashStorage.read(2));
      y_robot = float(int(dueFlashStorage.read(4))*255 + dueFlashStorage.read(3))/100.0;
      o_robot = float(int(dueFlashStorage.read(6))*255 + dueFlashStorage.read(5))*(PI/180.0)/100.0;
      
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

void s1p0(){medi_refl_1[0] = micros()-tiem_refl_1[0];if(medi_refl_1[0] < valo_refe_line1[0]){medi_bool_line[0]=LOW;}else{medi_bool_line[0]=LOW;}}  // desactivado
void s1p1(){medi_refl_1[1] = micros()-tiem_refl_1[1];if(medi_refl_1[1] < valo_refe_line1[1]){medi_bool_line[1]=LOW;}else{medi_bool_line[1]=LOW;}}  // desactivado
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
void s2p6(){medi_refl_2[6] = micros()-tiem_refl_2[6];if(medi_refl_2[6] < valo_refe_line2[6]){medi_bool_line[14]=LOW;}else{medi_bool_line[14]=LOW;}}  // desactivado
void s2p7(){medi_refl_2[7] = micros()-tiem_refl_2[7];if(medi_refl_2[7] < valo_refe_line2[7]){medi_bool_line[15]=LOW;}else{medi_bool_line[15]=LOW;}}  // desactivado

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

// ---------------- ventilador ----------------
void fan(bool v){
  digitalWrite(fanpin,v); 
}
// ---------------- buzzer ----------------
void buz(bool v){
  digitalWrite(buzpin,v); 
}

