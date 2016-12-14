// por: Julian Galvez Serna
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

#include <EEPROM.h>      // se usara para guardar valores de configuracion
  
//-------------------------tiempo----------------------------------
#include <Time.h>
// hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ------------------------------------- vector de salida de datos ---------------------------------
const byte bufsalida = 30;             // tamaño del buffer de salida
char vector_salida[bufsalida][27];    // vector usado para guardar los datos generados mientras se envian
  
// los uso para moverme a travez del vector de salida de datos
byte vec_i = 0;                    // puntero posicion inicial -> de donde se toma el dato a enviar
byte vec_f = 0;                    // puntero posicion final -> en donde se guardan los nuevos datos

// ------------------------------- variables direccionamiento de datos ------------------------------
String ID = "";                    // identificacion del globo -> serial del xbee
String nom = "";                   // la uso para guardar la direccion de destino 

// ----------------------------------- variables tiempo de muestreo ---------------------------------
byte tmuestreo = 1;                // (tiempo entre medidas) - variable configurable de la base en seg
unsigned long tiempomedicion = 0;  // guardo el momento en que se cargo la ultima medicion

// -------------------------------------- sensores --------------------------------------------- 

// ------------ variables y parametros del sensor de luminosidad ------------ 
const int lumpin = A0;          // puerto analogo señal de luminosidad
int luminosidad = 0;             // variable sensor intensidad luminosa
const float klum = 0.0048;           // constante para la intensidad luminica (0.0048: 0-1024 -> voltios)
String s_luminosidad = "0000";   // intensidad e luz en string

// ------------ variables y parametros del sensor de humedad y temperatura (chipcap) ------------
const int tempin = A4;  // puerto analogo señal de temperatura
const int humpin = A5;  // puerto analogo señal de humedad

int tem = 0;            // valor digital de temperatura
int hum = 0;            // valor digital de humedad
float ftem = 0;         // valor en unidades de temperatura
float fhum = 0;         // valor en unidades de humedad
String s_hum,s_tem;     // temp y hum en string

// ------------ variables del anemometro ------------
unsigned long tanemometro = 0;    // variable anemometro (tiempo entre pulsos)
float anemometro = 0;             // velocidad anemometro en m/s
unsigned long tantes = 0;         // tiempo a el pulso anterior
String s_anemometro;              // velocidad en string
  
  
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ setup ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup(){
  // inicio -> 1 parpadeo
  pinMode(13, OUTPUT);
  parpadear(500,1); 
  
  // ------ se leee el timepo de muestreo de la eeprom
  //eeprom_tiempo(10);                                           // configuro tiempo de muestreo a 1 seg - para precargar la eeprom
  tmuestreo = leer_tie();                                       // leeo los parametros de configuracion almacenados en la eeprom
  if (tmuestreo <= 0 || tmuestreo >= 255){tmuestreo = 1;}       // si el tiempo de muestreo es 0  o 255 lo hago de un segundo
  
  // se configura la velocidad del xbee
  //eeprom_velserial(4);
  Serial.begin(cod2vel(leer_vel()));        // configuro la velocidad del puerto serial tomando el codigo xbee de eeprom y decodificandolo a la velocidad en baudios
  
  
  parpadear(300,2);                         // trato de obtener la direccion del xbee -> 2 parpadeos
  ID = leer_dir_xbee();                     // leo el serial del xbee y lo uso como identificador del globo
  
  parpadear(500,3);                         // ultimas configuraciones para el incio del programa -> 3 parpadeos
  
  // --------- configuracion anemometro ---------  
  pinMode(2, INPUT);                              // configuro como entrada el pin del anemometro
  attachInterrupt(0, medir_tanemometro, RISING);  // abilito interrupcion en pin 2 - acciono la funcion:  medir_tanemometro
  
  // -------------- configuro el tiempo ---------------
  // si el tiempo no ha sido configurado pido la hora a la base
  // setTime(00,00,00,8,9,14);                          // hora, minutos, segundos, dia, mes, añó.
  // parametros(2);
  
  // --------- antes de inciar ---------  
  lserial();                                           // limpio el serial antes de empezar
  
} // fin configuracion
  
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ loop ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void loop(){
  //parpadear(10,20); 
  
  ini:
  // ----------------------- pregunto por datos entrantes --------------------
  if (Serial.available() > 0) {    // si llegan datos seriales 
    accion_serial();
  }
  
  // si no se ha configurado la hora se pide la hora
  if(timeStatus() == timeNotSet){Serial.println("GI" + ID + "F");delay(200);goto ini; }
  // si el tiempo requiere ser sincronizado envio el mensaje para sincronizarlo
  if(timeStatus() == timeNeedsSync){Serial.println("GI" + ID + "F");}
  
  
  // ------------- leeo sensores ------------- 
  medir_chipcap();       // -> actualiza: ftem, fhum
  medir_anemometro();    // -> actualiza: anemometro
  medir_luz();           // -> actualiza: luz
  
  // -------------- lleno el vector de salida cada t muestreo -------------- 
  if(millis() > (tiempomedicion+(1000*tmuestreo))){
    // ------------- lleno el vector de salida ------------- 
     capturar_dato();
     tiempomedicion = millis();   // actualizo el tiempo      
  }
  
  // ----------------------- envio datos por serial --------------------
  enviar_datos();
  
  delayy(200);    // actualizo medidas
  
  // reviso las mediciones
  //mostrar_anemometro();
  //mostrar_chipcap();
  //mostrar_eeprom();
  //mostrar_lum();
  //Serial.println();
  
}// fin loop
  
  
void delayy(int demora){
  unsigned long contador = millis();
  while (contador+demora > millis()){
    if (Serial.available() > 0) {    // si llegan datos seriales 
      accion_serial();
      delay(10);
    }
  }
} 
  
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// almaceno en el vector de salida el ultimo dato y ahumento el vector vec_f
void capturar_dato(){
  // aumento el indice de llenado (siempre que no llene el campo del vector de salida)
  //byte aux_vecf = (vec_f + 1);
  //if (aux_vecf >= (bufsalida-1)){aux_vecf = 0;}
  // verifico que el valor que sigue no sea el que esta saliendo de ser paro
  //if (aux_vecf != vec_i){
  if ((vec_f + 1)%(bufsalida-1) != vec_i){
    String aux_tiempo = obt_tiempo();
    
    // guardo el tiempo
    for(int i=0;i<12;i++){             
      vector_salida[vec_f][i] = aux_tiempo[i] ; 
    }
    // guardo los datos
    for(int i=12;i<16;i++){             
      vector_salida[vec_f][i] = s_tem.charAt(i-12);            // lleno temperatura
      vector_salida[vec_f][i+4] = s_hum.charAt(i-12);          // lleno humedad
      vector_salida[vec_f][i+8] = s_anemometro.charAt(i-12);   // lleno anemometro
      vector_salida[vec_f][i+12] = s_luminosidad.charAt(i-12); // lleno luminosidad
    }
  
    //vec_f = aux_vecf;                  // aumento el indice de valor final del buffer
    vec_f = (vec_f + 1)%(bufsalida-1);
//    Serial.print("vec_f: ");
//    Serial.println(vec_f);
  }
  
}

// envio los datos diponibles en el vector se salida por serial a la mayor velocidad
void enviar_datos(){
  if(vec_i == vec_f){return;}      // si no hay datos no envio nada
  
  Serial.print("GD");              // envio datos por serial
  Serial.print(ID);
  for(int i = 0;i<28;i++){
    Serial.print(vector_salida[vec_i][i]);
  }
  Serial.print("F");
  // ----------------- salto de linea ----------------- 
  Serial.println();
}

void parpadear(int tie,int num){ 
  for(int i=0;i<num;i++){
    digitalWrite(13,HIGH);
    delay(tie);
    digitalWrite(13,LOW);
    delay(tie);
  }
}
