
// MERCURY 4.0 - Velocista por GEM

double Input, Output, Setpoint; // parametros de  entrada, salida y deseados del controlador
float veli,veld;                // velocidad de los motores calculada con los encoders

bool recta = 0;                                         // detecto la salida de la linea
bool izquierda = 0;                                     // indico si esta avanzando a la izquierda
unsigned long ejecucion = 0;
bool correr = 1;                  // 0, cuando muestro por serial, 1 no serial 
bool afuera = 0;                  // la uso para saber si sali de la linea
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup(){
  ejecucion = millis();
  if(!correr)
  Serial.begin(57600); // se inicia la comunicacion serial
  
  iniciar_motores();   // se inician los motores
  
  // configuracion del controlador
  
  //SetTunings(0.48,0.25,0.0625);          // 
  //SetTunings(0.63,0.01,0.078);          // bien -> v = 0.4
  
  // otros parametros del controlador
  
//  SetSampleTime(25);           // indicamos el periodo de muestreo
//  SetOutputLimits(-1,1);       // delimitamos los valores min, max
//  Setpoint = 0.00;             // iniciamos el setpoint en 0
//  SetMode(1);                  // ponemos el PID en modo automatico
//  SetControllerDirection(0);   // se pone el controlador de accion directa
//  Initialize();
  
  calibrar_sensores();         // se calibran los QTR, tomando el valor minimo, fondo blanco
  // ------------------------ fin configuraciones ------------------------ 
  while(ejecucion + 5000 > millis()){} // ajustamos el tiempo requerido al inicio
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void loop(){ 
  ejecucion = millis();  // tomamos como base de la ejecucion el tiempo de incio del ciclo
  
  float linea;  // posicion de la linea
  
  leer_prom(6);                 // leeo y promedio las 5 ultimas mediciones
  linea = calcular_linea();     // calculo la posicion de la linea por pesos
  
  // si sale de la linea
  if (!hay_linea()){            // pongo como entrada del controlador el valor medido por peso
    if (izquierda){linea = -1;}
    else{linea = 1;}
    afuera = 1;
  }
  // si estoy dentro de la linea
  else{
    //detengo con suabidad
    if(afuera == 1){
      if(abs(linea)>0.5)linea = -linea;
      else afuera=0;
    }
  }
  
  //Input = -linea; 
  
  
  //while(ejecucion + 25 > millis()){} // ajustamos el tiempo del controlador
  //Compute();                         // se calcula el pid

  if(!correr){
    Serial.print(linea);
    Serial.print('\t');
    Serial.print(Output);
    Serial.print('\t');
    Serial.print(izquierda);
    Serial.print('\t');
  
    mostar_linea();              // muestro el valor medido de todos los sensores
    Serial.println();
  }
  
  seguir_1(linea);
  //seguir_1(Output);            // aplico la accion calculada por el 
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub funcion ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

// ------------ seguir linea con funicion de retorno ------------ 
void seguir_1(float LINEA){
  float vd = 0.4;    // velocidad base derecha
  float vi = 0.4;    // velocidad base izquierda

  motor(0,vd+LINEA);   // motor derecho
  motor(1,vi-LINEA);   // motor izquierdo
  
}

