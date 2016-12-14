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

//-------------------------------------vector de salida de datos---------------------------------
  const byte bufsalida = 40;             // tamaño del buffer de salida
  char vector_salida[bufsalida][27];    // vector usado para guardar los datos generados mientras se envian
  
  // los uso para moverme a travez del vector de salida de datos
  byte vec_i = 0;                    // puntero posicion inicial -> de donde se toma el dato a enviar
  byte vec_f = 0;                    // puntero posicion final -> en donde se guardan los nuevos datos
  //bool fvr = false;

//--------------------------------------otras variables---------------------------------------------  
String ID = "";                    // identificacion del globo -> serial del xbee
byte tmuestreo = 1;                // (tiempo entre medidas) - variable configurable de la base en seg
unsigned long tiempomedicion = 0;  // guardo el momento en que se cargo la ultima medicion
String nom = "";                   // la uso para guardar la direccion de destino

//int dato[1];             // variable para lectura de datos por serial
//word para[7];            // variable para guardar los parametros del globo
// -------------------------------------- sensores --------------------------------------------- 

// ------------ variables analogas del entorno ------------ 
int luminosidad = 0;             // variable sensor intensidad luminosa
const float klum = 12;           // constante para la intensidad luminica
String s_luminosidad = "0000";   // intensidad e luz en string

// ------------ variables para la lectrura del chipcap ------------
const int tempin = A4;  // puerto analogo señal de temperatura
const int humpin = A5;  // puerto analogo señal de humedad

int tem = 0;            // valor digital de temperatura
int hum = 0;            // valor digital de humedad
float ftem = 0;         // valor en unidades de temperatura
float fhum = 0;         // valor en unidades de humedad
String s_hum,s_tem;     // temp y hum en string
// ------------ variables para la lectrura del anemometro ------------
unsigned long tanemometro = 0;    // variable anemometro (tiempo entre pulsos)
float anemometro = 0;             // velocidad anemometro en m/s
unsigned long tantes = 0;         // tiempo a el pulso anterior
String s_anemometro;              // velocidad en string
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ setup ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup(){
  //parametros(10);                            // configuro tiempo de muestreo a 1 seg - para precargar la eeprom
  tmuestreo = leer_tie();                   // leeo los parametros de configuracion almacenados en la eeprom
  if (tmuestreo == 0){tmuestreo = 1;}       // si el tiempo de muestreo es 0 lo hago de un segundo
  Serial.begin(19200);                       // velocidad serial
  ID = leer_dir_xbee();                     // leo el serial del xbee y lo uso como identificador del globo
  
  // --------- configuracion anemometro ---------  
  pinMode(2, INPUT);                              // configuro como entrada el pin del anemometro
  attachInterrupt(0, medir_tanemometro, RISING);  // abilito interrupcion en pin 2 - acciono la funcion:  medir_tanemometro
  // -------------- configuro el tiempo ---------------
  // si el tiempo no ha sido configurado pido la hora a la base
  //setTime(00,00,00,8,9,14);                          // hora, minutos, segundos, dia, mes, añó.
  
  // --------- antes de inciar ---------  
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  lserial();                                           // limpio el serial antes de empezar
} // fin configuracion
  
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ loop ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void loop(){
  // ------------- leeo sensores ------------- 
  medir_chipcap();       // -> actualiza: ftem, fhum
  medir_anemometro();    // -> actualiza: anemometro
  
  // -------------- lleno el vector de salida cada t muestreo -------------- 
  if(millis() > (tiempomedicion+(1000*tmuestreo))){
    // ------------- lleno el vector de salida ------------- 
     capturar_dato();
     tiempomedicion = millis();   // actualizo el tiempo      
  }
  
  ini:
  // ----------------------- pregunto por datos entrantes --------------------
  if (Serial.available() > 0) {    // si llegan datos seriales 
    accion_serial();
  }
  
  // si no se ha configurado la hora se pide la hora
  if(timeStatus() == timeNotSet){Serial.println("GI" + ID + "F");delay(200);goto ini; }
  // si el tiempo requiere ser sincronizado envio el mensaje para sincronizarlo
  if(timeStatus() == timeNeedsSync){Serial.println("GI" + ID + "F");}
  
  // ----------------------- envio datos por serial --------------------
  enviar_datos();
  
  delay(300);    // actualizo medidas
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
  // ----------------- salto de linea ----------------- 
}

