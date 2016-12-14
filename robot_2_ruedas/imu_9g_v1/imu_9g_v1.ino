// por: juan diego cardenas
// por: julian galvez
#include <Wire.h>    // libreria serial
// L3G4200D -> GYRO
// LSM303   -> ACC, MAG
// --------------------- variables IMU --------------------- 
// I2C addressses for IMU 
#define GYR_ADDRESS (0xD2 >> 1) // datasheet
#define MAG_ADDRESS (0x3C >> 1) // datasheet
#define ACC_ADDRESS (0x30 >> 1) // datasheet

// Gyro, ACC, Mag enable registers (posicion)
#define L3G4200D_CTRL_REG1 0x20
#define LSM303_CTRL_REG1_A 0x20
#define LSM303_MR_REG_M 0x02

// acc settings 
#define CTRL_REG4_A 0x23

// gyro settings
#define CTRL_REG4 0x23

// Gyro, Acc, Mag output registers 
#define L3G4200D_OUT_X_L 0x28
#define LSM303_OUT_X_L_A 0x28
#define LSM303_OUT_X_H_M 0x03

// ACC Reg data
float ACCx, ACCy, ACCz;
// promedio al origen
float bACCx, bACCy, bACCz;
// Gyro Reg data
float GYROx, GYROy, GYROz;
// Bias
float bGYROx, bGYROy, bGYROz;

// MAG REG data
int MAGx, MAGy, MAGz;

bool ms = 0;

// YAW calc
int xMAGMax, yMAGMax, zMAGMax;
int xMAGMin, yMAGMin, zMAGMin;
float xMAGMap, yMAGMap, zMAGMap;

// calculations
float pitchAccel, pitchGyro;
float rollAccel, rollGyro;

// gyro/acc gain-converts raw values, gyro deg/s, acc to Gs
#define AccGain 3.9      //8g
#define GyroGain 70     //2000dps

// kalman,
float giroVar = 0.1;
float deltaGiroVar = 0.1;
float accelVar = 5;
float Pxx = 0.1; // angle variance
float Pvv = 0.1; // angle change rate variance
float Pxv = 0.1; // angle and angle change rate covariance
float kx, kv;

// final values
float pitchPrediction = 0; // Output of Kalman filter, final pitch value
float rollPrediction = 0;  // Output of Kalman filter, final roll value
float yawRaw = 0;          // final yaw value. yaw doesnt go through kalman! has its own calc.

//----------------------Fin de configuracion IMU-------------------------------------------------------

//----------------------Conficuracion de motores-------------------------------------------------------
//motor izquierdo - m2
const byte dm1 = 4;
const byte vm1 = 5;

// motor derecho - m1
const byte dm2 = 7;  
const byte vm2 = 6;

// velocidades
byte vel1 = 0;
byte vel2 = 0;

//byte vel2 = 60;

char dato;

//----------------Variables controlador------------------------

float SetP = 0;
float Medida = 0;
float Medida1 = 0;

float kp = 0; //  Constante proporcional
float tn = 0; //  Constante integral
float tv = 0; //  Constante derivativa

float controlador = 0;  //  Controlador

float s = 0;
float h = 0;

unsigned long tiempo_s = 0; // Tiempo entre s
unsigned long tiempo=0;
unsigned long tiempo_p = 0; // Tiempo entre pulsos
float del_t = 0;    // delta de tiempo (usado en segundos)

float error = 0;     // Error presente (0 - 255)
float error_p = 0;   // Error pasado   (0 - 255)

unsigned int del_p = 0; //delta de angulo

//time, used in kalman filtering to keep a constant loop time
unsigned long timer = 0;
unsigned long timer1 = 0;
float timeStep = 0.02;          //en s. Need a time step value for integration of gyro angle from angle/sec

unsigned int ajuste_t = 0;

// ---------- busqueda de variable mapero minima y maxima
float valmin = 1000;
float valmax = -1000;

// ----------------------- configuracion ----------------------- 
void setup(){
  Serial.begin(115200); // begin serial comm.
  iniciar_IMU();        // configuro IMU y inicializo
  CalibrateIMU();       // calibrate the IMU for level starting pos.
  iniciar_motores();    // configuro puertos motores y los detengo
  conf_controlador();   // Inicializa el controlador
}

// ----------------------- programa principla ----------------------- 
void loop() {
  timer = micros(); // tiempo de inicio del loop
  // -------------- inicio del programa -------------- 
  //read sensors
  readGyro();readAcc();readMag();
  //calcualte pitch, roll, yaw, kalman etc
  Calculations();
  //print values
  PrintVals();
  // -------------- fin  del programa -------------- 
  // calculo la demora del programa, y ajusto el tiempo de clada ciclo a 20ms
  timer1 = micros(); // tiempo de finalizacion del loop
  ajuste_t = (timer1-timer);
  // si el tiempo en menor lo ajusto
  if (ajuste_t < (timeStep*1000000)){ 
    ajuste_t = (timeStep*1000000) - ajuste_t; // calculo el tiempo muerto restante
    // espero el tiempo necesario para llegar a los 20ms (teniendo en cuanta del desvorde de delayMicroseconds() a 16000)
    if (ajuste_t >= 16000){
      delayMicroseconds(16000);
      delayMicroseconds(ajuste_t - 16000);
    }else{
      delayMicroseconds(ajuste_t);
    }
  }else{
    Serial.println("tiempo de integracion de giroscopo mayor a 20ms");
  }
}
// --------------------------- fin loop ------------------------------

void PrintVals(){
  
//  valmin = min(valmin,rollPrediction);
//  valmax = max(valmax,rollPrediction);
//  //Medida = rollPrediction;
//  Medida1 = map(rollPrediction, -90.0,90.0,2.5,67);
//  // 360/(67-3) = 5.2941
//  //Medida = (rollPrediction-1)*5.2941 - 180;
//  Medida = (rollPrediction-31)*2.5714;
  
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(rollPrediction);
//  Serial.print(", map:");
//  Serial.print(Medida1);
//  Serial.print(", cal:");
//  Serial.print(Medida);
//  Serial.print(", ");
//  Serial.print(error);
//  Serial.print(", max:");
//  Serial.print(valmax);
//  Serial.print(", min:");
//  Serial.println(valmin);
  Serial.println();
}

//-----------------Funciones para tomar datos---------------
//void datoserial(char dato){
////    if(dato == 'a'){
////      adelante();
////    }
//    if(dato == 'p'){
//      vel1=0;
//      parar();
//    }
////    if(dato == 'c'){
////      delay(100);
////      tipo = Serial.read();
////      conf_controlador();
////    }
//    if(dato == 'v'){
//      vel1 = vel1+5; // velocidad de motores
//      adelante();
//    }
//}


