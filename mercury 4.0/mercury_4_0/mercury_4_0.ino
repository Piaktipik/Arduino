
// MERCURY 4.0 - Velocista por GEM

double Input, Output, Setpoint; // parametros de  entrada, salida y deseados del controlador
float veli,veld;                // velocidad de los motores calculada con los encoders

bool ult_i = 0,ent_i = 0;                 // variable para detectar si salio por la izquierda
bool ult_d = 0,ent_d = 0;                 // variable para detectar si salio por la derecha

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup(){
  Serial.begin(57600); //This pipes to the serial monitor
  iniciar_motores();
  //iniciar_encoders();
  
  // configuracion del controlador -> 1) 1,10
  //SetTunings(0.5,0.3,0.1);   // kp, ki, kd
  //SetTunings(2,0.1,5);         // configuramos constantes del controlador kp, ki, kd
  SetTunings(1,0.0,0.0);//kp=2, ki=0.03, kd=2
  SetSampleTime(20);           // indicamos el periodo de muestreo
  SetOutputLimits(-1,1);       // delimitamos los valores min, max
  Setpoint = 0;                // iniciamos el setpoint en 0
  SetMode(1);                  // ponemos el PID en modo automatico
  //Initialize();
  
  // calibramos el blanco del sensor de linea antes de empezar
  delay(1000);
  calibrar_linea();
  delay(1000);
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void loop(){ 
  float linea;  // posicion de la linea
  medirlinea();                // mido los sensores de linea
  linea = calcular_linea();    // calculo la posicion de la linea por pesos
  Input = linea;               // pongo como entrada del controlador el valor medido por peso
  Compute();                   // se calcula el pid
  //mostar_encoders();
  Serial.print(linea);
  Serial.print('\t');
  Serial.print(Output);
  Serial.print('\t');
  Serial.print(ult_i);
  Serial.print('\t');
  Serial.print(ult_d);
//  Serial.print('\t');
//  Serial.print(ent_i);
//  Serial.print('\t');
//  Serial.print(ent_d);
//  Serial.print('\t');
  mostar_linea();              // muestro el valor medido de todos los sensores

  Serial.println();

  seguir_1(linea);
  //seguir_1(Output);
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub funcion ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

// ------------ seguir linea con funicion de retorno ------------ 
void seguir_1(float LINEA){
  float vd = 0.8;    // velocidad base derecha
  float vi = 0.8;    // velocidad base izquierda
  float va = 0.0;   // rango de no accion
  
  if (abs(LINEA) < 0.2 && (ult_d || ult_i)){
    if(ult_d){
      motor(0,vd*2);     // motor derecho
      motor(1,vi*-0.1);   // motor izquierdo
    }else if(ult_i){
      motor(0,vd*-0.1);   // motor derecho
      motor(1,vi*2);     // motor izquierdo
    }  
  }
  // evito que oscile mucho para valores pequeños de perturbacion
  else if(abs(LINEA) > va){
    motor(0,vd+LINEA);   // motor derecho
    motor(1,vi-LINEA);   // motor izquierdo
  }
  
//  if(abs(LINEA) > 0.4){
//    ult_i = false;
//    ult_d = false;
//  }
  
//  if (LINEA < -va){
//    motor(0,vd+LINEA);   // motor derecho
//    motor(1,vi-LINEA);   // motor izquierdo
//  }else if(LINEA > va){
//    motor(0,vd+LINEA);   // motor derecho
//    motor(1,vi-LINEA);   // motor izquierdo
//  }
  
  // ----------- si sale de la linea ----------- 
//  else{  
//  }
//  }
  
}

// ----------------------- seguir linea simple ----------------------- 
void seguir_2(float ACCION){
  float vd = 0.4;
  float vi = 0.4;
  
  motor(0,vd-ACCION);   // motor derecho
  motor(1,vi+ACCION);   // motor izquierdo
  
}
