// tipo de controlador
byte tipo = 0;
// 0 - escalon directo;
// 1 - Control_P();
// 2 - Control_PI();
// 3 - Control_PID();

// ------------------ Variables de los controladores ------------------ 
// variables proceso
float error = 0;     // Error presente (0 - 255)
float error_p = 0;   // Error pasado   (0 - 255)

//float por_dato = 0;    // Porcentaje de PWM requerido
////float por_PWM = 0;   // Porcentaje del PWM
//float por_vel = 0;     // Porcentaje de velocidad

// variables controlador

//const float ave_time = 1.16; // Tiempo promedio de estabilizacion
//const float tu = 0.02;
//const float tg = 1.13;

float kp = 2; //  Constante proporcional
float tn = 0.1; //  Constante integral
float tv = 0.4; //  Constante derivativa

float controlador = 0;  //  Controlador

float s = 0;
float h = 0;

unsigned long tiempo_s = 0; // Tiempo entre s

// ----------- conexion motores ----------- 
//motor izquierdo - m2
const byte pin_dir_mot_izq = 4;   // verde
const byte pin_vel_mot_izq = 5;   // amarillo

// motor derecho - m1
const byte pin_dir_mot_der = 7;   // rojo
const byte pin_vel_mot_der = 6;   // naranja

// velocidades motores (valor de pwm)
byte vel_mot_izq = 0;
byte vel_mot_der = 0;

// velocidades deseadas (valor de pwm)
byte vel_mot_izq_des = 0;
byte vel_mot_der_des = 0;

// vel maxima pwm (la uso para hacer proporcional la velocidad)
float vel_mot_izq_max = 36.5;
float vel_mot_der_max = 0;

// velocidades medidas ()
float vel_mot_izq_med = 0;   // en rad/seg
float vel_mot_der_med = 0;   // en rad/seg

// velocidad escalada para el error
float vel_mot_izq_med_esc = 0;

// tiempos (ms) entre calculos de velocidad de los motores
unsigned long tiempo_p = 0; // Tiempo entre pulsos
float del_t = 0;    // delta de tiempo (usado en segundos) 

// ------------- variables encoders ------------- 

unsigned int enc_izq = 0;      // contador de pulsos del ecnoder
unsigned int enc_izq_p = 0;    // Numero de vueltas pasadas

unsigned int del_p = 0;         // delta de pulsos

const int pul_por_vue = 482;   // pulsos por vuelta marcado por los encoders


// --------------- variable tiempo de muestreo --------------- 
unsigned long tiempo = 0;   // Tiempo entre muestreos

//float wr = 0;            // Velocidad angular

//int tiempo_eje=0;
//int tiempo_eje_p=0;
//int tiempo_ejecucion=0;

// --------------- sintonizacion --------------- 
float kcr = 1;
int tcr = 0; // periodo oscilacion
unsigned long tiempo_tcr = 0;
bool signo_error = false; 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

void setup() {
  
  // configuro pines de los motores como salida
  pinMode(pin_dir_mot_der,OUTPUT);
  pinMode(pin_vel_mot_der,OUTPUT);
  pinMode(pin_dir_mot_izq,OUTPUT); 
  pinMode(pin_vel_mot_izq,OUTPUT);
  
  attachInterrupt(0, in_enco1a, RISING); // activo interrupcion pin 2 (arduino mega)

  Serial.begin(115200);      // configuro puerto serial
  
  // configuro valores del controlador elegido
  conf_controlador();
  pinMode(53,OUTPUT);
  digitalWrite(53,LOW);
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void loop() {
  
  // ejecuto periodicamente el control
  if (millis() > tiempo + 50){
    
    velocidad_izq();      // calculo velocidad motores -> vel_mot_izq_med
    calcular_error();     // calculo error (diferencia de velocidades) -> error , calculo s -> s
    
    // ----------
    
    if (tipo == 1) {controlador = Control_P();}
    if (tipo == 2) {controlador = Control_PI();}
    if (tipo == 3) {controlador = Control_PID();}
    
    // sintonizados
    if (tipo == 4) {controlador = Control_P();}
    if (tipo == 5) {controlador = Control_PI();}
    if (tipo == 6) {controlador = Control_PID();}
    
//    if((controlador + vel_mot_izq) > 255){vel_mot_izq = 255;}
//    else if ((controlador + vel_mot_izq) < 0){vel_mot_izq = 0;}
//    else {vel_mot_izq += controlador;}
  
    if(controlador > 255){controlador = 255;}
    else if (controlador < 0){controlador = 0;}
    
    vel_mot_izq = controlador;
    
    if (tipo == 0)  {vel_mot_izq = vel_mot_izq_des;}
    adelante();
    
    imprimir();
    
    tiempo = millis(); // actualizo el tiempo
  }
  
  // reviso el puerto en busca de datos de el usuario
  if(Serial.available() > 0){
    datoserial(Serial.read());
  }
  
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void conf_controlador(){
  // P
  if (tipo == 1) {  
    kp = 1;
  }
  // PI
  if (tipo == 2) {  
    kp = 5;
    tn = 0.11;
  }
  if (tipo == 3) {  
    kp = 0;
    tn = 0.0055;
    tv = 0;
  }
  
  if (tipo == 4) {  
    kp = 0.5*kcr;
  }
  if (tipo == 5) {  
    kp = 0.45*kcr;
    tn = tcr/1.2;
  }
  if (tipo == 6) {  
    kp = 0.5*kcr;
    tn = 0.5*tcr;
    tv = 0.125*tcr;
  }
  
}


// funcion que calcula la velocidad del motor
void velocidad_izq(){
  // calculo -> cambio de tiempo y cambio de angulo
  del_t = (millis() - tiempo_p);     // delta de tiempo 
  del_p = enc_izq - enc_izq_p;       // delta de pulsos
  del_t = (del_t/1000.0);            // pongo el delta de tiempo en segundos

  // almaceno el valor actual como anterior
  tiempo_p = millis();
  enc_izq_p = enc_izq; 
  
  // calculo la velocidad
  vel_mot_izq_med = float(del_p)/del_t;                        // calculo la velocidad de los pulsos
//  Serial.print(" \t enco: ");
//  Serial.print(enc_izq);
//  
//  Serial.print(" \t velmed1: ");
//  Serial.print(vel_mot_izq_med);
  vel_mot_izq_med = vel_mot_izq_med*(2*PI/pul_por_vue);   // convierto de pulsos a radianes
//  Serial.print(" \t velmed2: ");
//  Serial.print(vel_mot_izq_med);
//  Serial.print(" \t del_t: ");
//  Serial.print(del_t);
//  Serial.print(" \t del_p: ");
//  Serial.print(del_p);
}


// Funcion para calcular el error del sistema
void calcular_error(){
  // calculo -> cambio de tiempo entre calculo de errores
//  del_t = millis() - tiempo_s;
//  tiempo_s = millis();           // actualizo tiempo
//  del_t = (del_t/1000.0);        // pongo el delta de tiempo en segundos
  
  vel_mot_izq_med_esc = (vel_mot_izq_med/vel_mot_izq_max)*255; // escalo la velocidad medida a 0 a 255
  if(vel_mot_izq_med > vel_mot_izq_max){vel_mot_izq_med_esc = 255;}      // evito desbordes
  
  // calculo el error (deseado - real)
  error = vel_mot_izq_des - vel_mot_izq_med_esc;
  
  // calculo parametros integral y derivativo
  
  s = (error-error_p)/del_t;  // parametro difencial
  
  h += error * del_t; // parametro integral
  
  
  error_p = error; // actualizo el error_p
} 

//Controladores
// ---------------------- control p ---------------------- 

float Control_P(){
//  if (tipo == 4) {
//    // sintonizo controlador
//    //pregunto si el error es mayor al 2%
//    if((vel_mot_izq_des*1.02 > error) || (vel_mot_izq_des*0.98 > error)){
//      
//    }
//    else{
//     if(signo_error == (error > 0))
//     
//    }
//    kcr = kp;
//    tiempo_tcr = (millis()/1000);
//  }
  return kp*error;
}

// ---------------------- control pi ---------------------- 

float Control_PI(){
  
  //return kp*(1 + h/(tn));
  return kp*error + tn*h;
}

// ---------------------- control pid ---------------------- 

float Control_PID(){
  
  //return kp*(tv*s + 1/(tn*h) + 1);
  return kp*error + tn*h + tv*s;
}

// ---------------------- Funcion para incremetar encoder ---------------------- 

void in_enco1a(){
  enc_izq++;
}

// ---------------------- Funcion para imprimir datos ---------------------- 

void imprimir(){
  
  // datos requeridos por el graficador
  //Serial.print("\t tie: ");
  Serial.print(millis());               // tiempo en ms
  
  //Serial.print("\t vel: ");
  //Serial.print(byte(vel_mot_izq));    // envio la velocidad del motor izquierdo deseada en pwm(0-255) 1 byte - interfaz
  //Serial.print(vel_mot_izq);            // envio la velocidad del motor izquierdo deseada en pwm(0-255) 1-3 bytes - visualizacion
  Serial.print(',');
  //Serial.print("\t velm: ");
  Serial.print(vel_mot_izq_med);    // envio la velocidad del motor izquierdo deseada en pwm(0-255) 1 byte - interfaz
  
  Serial.print(',');
  Serial.print(vel_mot_izq_des);    // envio la velocidad que se esta tratando de alcanzar
  
  //Serial.print("\t veld: ");
  //Serial.print(vel_mot_izq_des);    // envio la velocidad del motor izquierdo deseada en pwm(0-255) 1 byte - interfaz
  
  Serial.print(',');
  Serial.print(error);      // envio la velocidad del motor izquierdo leida rad/seg
  Serial.print(',');
  Serial.println(controlador);
  
  //Serial.print( "\t vele: ");
  ////Serial.println(vel_mot_izq_med);      // envio la velocidad del motor izquierdo leida rad/seg
  //Serial.println(vel_mot_izq_med_esc);      // envio la velocidad del motor izquierdo con valor de (0-255)
  
 
}

// ---------------------- Manejo del motor ---------------------- 
void adelante(){
  digitalWrite(pin_dir_mot_izq, LOW);    // motor izquierdo
  analogWrite(pin_vel_mot_izq, vel_mot_izq);
  digitalWrite(pin_dir_mot_der, LOW);    // motor derecho
  analogWrite(pin_vel_mot_der, vel_mot_der);
}

void parar(){
  digitalWrite(pin_dir_mot_izq, LOW);    // motor izquierdo
  analogWrite(pin_vel_mot_izq, 0);
  digitalWrite(pin_dir_mot_der, LOW);    // motor derecho
  analogWrite(pin_vel_mot_der, 0);
}

// ---------------- funcion que procesa los datos recibidos por serial ---------------- 
void datoserial(char dato){
    if(dato == 'a'){
      adelante();
    }
    if(dato == 'p'){
      parar();
    }
    if(dato == 'c'){
      delay(100);
      tipo = Serial.read();
      conf_controlador();
    }
    if(dato == 'v'){
      delay(100);
      
      vel_mot_izq_des = Serial.read(); // vel izquierda
      //vel_mot_der = Serial.read(); // vel derecha
      adelante();
    }
}
