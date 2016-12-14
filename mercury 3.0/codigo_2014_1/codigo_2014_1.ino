// por: Julian Galvez Serna - lucas tobon, juan diego cardenas, omar pino
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// para guardar datos en la memoria sinq ue se borren al reiniciarlos
// #include <DueFlashStorage.h>
// DueFlashStorage dueFlashStorage;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
//------------------------------------------------------ variables infrarrojos  ------------------------------------------------------ 
 
// --------------------------------------- variables pines infrarrojos de vela ---------------------------------------
byte senvela[19];

byte ruid_vela = 20;                   // variable con el ruido normal de los sensores de vela

byte defe_ruid_vela = 20;              // valor normal del ruido de la vela

bool apun_vela = false;                // variable que se activa cuando el robot ve la vela con los sensores laterales y esta rotando hacia la vela

bool apag_vela = false;                // apaganod vela

byte cont_vela = 0;                    // variable que uso para saber el numero de velas detectadas

byte aux_velo_giro = 0;                // variable que uso para ajustar la velocidad de giro hacia la vela

unsigned int tiem_apag = 700;          // tiempo que el robot usa para apagar la vela en ms

unsigned long ulti_vist_vela = 0;      // variable usada para reiniciar la deteccion de vela en caso de perderla

byte maxi_valo_vela = 0;               // maximo valor de vela detectado - se usa para preorizar en caso de encontrar varias velas

// --------------------------------------- conexion sensores infrarrojos pista  --------------------------------------- 

unsigned int aux_tiem_medi_infr;   // timepo de medicion infrarrojos -> guardo el tiempo desde la ultima medicion

//unsigned int peri_medi_infr = 5; // periodo de medicion infrarrojos ms

const byte nsen = 8;               // numero de sensores de infrarrojo

int rlinea = 1000;                 // defino el tiempo de espera de los sensores 
// ---------------------------------------------- referencia linea ------------------------------------
//  variables usadas para comparar cada sensor de linea individualmente
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
bool evadiendoo = false;

// ------------------------------------------------------ programa principal ------------------------------------------------------ 
const byte xmax = 25;
const byte ymax = 37;

//byte transitable[xmax][ymax];   // valor con las paredes incluidas
byte explorado[xmax][ymax];     // valor con las zonas ya exploradas
//byte velas[xmax][ymax];         // valor donde se detecto una vela  

// tiempo de serial
unsigned int timeserial = 0;                      // lo uso para tener presente el tiempo  trascurrido

// ----------------- funcion para evitar el atrancamiento ----------------- 

unsigned int aux_tiem_atra = 0; 

int valo_pasa_1 = 0;
int valo_pasa_2 = 0;
int valo_pasa_3 = 0;

// auxiliares deteccion de vela
bool dire_busq_vela = false;         // la uso para guardar el valor de giro, a la derecha o la izquierda

// ------------------------------------------------------ variables odometria ------------------------------------------------------ 
float x_robot;  // posicion x  de el robot
float y_robot;  // posicion y  de el robot
float o_robot;  // orientacion de el robot

// pruebas - permite seleccionar diferentes fragmentos de codigo sin tener que comentar
byte tipoc = 1;

// 0 - competencia
// 1 - competencia - processing 
// 2 - competencia - muestreo terminal
// 3 - muestreo processing
// 4 - muestreo terminal 


// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {
  inicio_posicion_rob();
  inicio_comunicaciones();
  inicio_actuadores();
  inicio_sensores();
  probar_fan();
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- programacion reactiva ----------------------------------------------------- 
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  if (tipoc < 3){
    // ------------------------------------ detecto lineas ----------------------------------- 
    detecto_lineas();
    // ------------------------------------ apago la vela ------------------------------------ 
    apago_vela();
    // ------------------------------------ evado obstaculos ---------------------------------  
    if(evado_obstaculo()){
      // ---------------- evito salirme de la pista o entrar a la vela apagada -----------------
      pista_vela();
      // ------------------------------------- busco velas ------------------------------------- 
      busco_vela();
      
      apago_vela();
      // --------------------------------- me acerco a la vela ---------------------------------
      acercar_vela();
    }    // si detecto obejeto con los ultrasonidos salto aca
    // --------------------- funcion que revisa si el robot se ha boqueada -------------------
    evito_bloqueo();
  }
  
  //calcular_desplazamiento();
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // ----------------------------------------------------- fin programacion reactiva -----------------------------------------------------  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  // lo uso para mostrar los datos de los sensores
  if(tipoc == 2 || tipoc == 4){mostrar_serial();}
  // lo uso para mostrar los datos en processing
  if(tipoc == 1 || tipoc == 3){mostrar_processing();}
  // mido sensores
  medir_ultrasonidos();
  medir_pista_vela();
  medirlinea();
  //delay(5);              // delay a tener en cuenta para la estabilidad de las medidas
  // recivo comandos por serial
  if(Serial.available() > 0){
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ




