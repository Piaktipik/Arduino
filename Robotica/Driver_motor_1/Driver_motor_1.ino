// por: Julian Galvez Serna - Lucas Tobon Vanegas - Francis Osorio

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
#include <Servo.h> 
Servo pinza; 

//#include <PID_v1.h>

//Define Variables we'll be connecting to
//double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Transformada Homogenea ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
float q4T1[] = {0.0003,0.0013,0.0028,0.0050,0.0079,0.0113,0.0154,0.0201,0.0254,0.0314,0.0380,0.0452,0.0531,0.0616,0.0707,0.0804,0.0908,0.1018,0.1134,0.1257,0.1385,0.1521,0.1662,0.1810,0.1963,0.2124,0.2290,0.2463,0.2642,0.2827,0.3019,0.3217,0.3421,0.3632,0.3848,0.4072,0.4301,0.4536,0.4778,0.5027,0.5281,0.5542,0.5809,0.6082,0.6362,0.6648,0.6940,0.7238,0.7543,0.7854,0.8165,0.8470,0.8768,0.9060,0.9346,0.9626,0.9899,1.0166,1.0427,1.0681,1.0930,1.1172,1.1407,1.1636,1.1860,1.2076,1.2287,1.2491,1.2689,1.2881,1.3066,1.3245,1.3418,1.3584,1.3744,1.3898,1.4046,1.4187,1.4323,1.4451,1.4574,1.4690,1.4800,1.4904,1.5001,1.5092,1.5177,1.5256,1.5328,1.5394,1.5453,1.5507,1.5554,1.5595,1.5629,1.5658,1.5680,1.5695,1.5705,1.5708};


// ------------------------------------------------------ motores y actuadores ------------------------------------------------------
// ----------- conexion motores -----------
// ----- motor 1 -----
const byte pin_dire_moto_1 = 24;
const byte pin_velo_moto_1 = 9;
byte velo_moto_1 = 100;
// ----- motor 2 -----
const byte pin_dire_moto_2 = 32;
const byte pin_velo_moto_2 = 11;
byte velo_moto_2 = 100;
// ----- motor 3 -----
const byte pin_dire_moto_3 = 30;
const byte pin_velo_moto_3 = 10;
byte velo_moto_3 = 10;
// ----- motor 4 -----
const byte pin_dire_moto_4 = 22;
const byte pin_velo_moto_4 = 8;
byte velo_moto_4 = 10;

// los pines de los encoders pinA(n) y PinB(n) deven soportar interrupciones fisicas -> Arduino Due

// ------------ datos control ------------ 
int pasosrev1 = 1640;
int pasosrev2 = 1200;
int pasosrev3 = 512;
float pasosgra1 = float(pasosrev1)/360;
float pasosgra2 = float(pasosrev2)/360;
float pasosgra3 = float(pasosrev3)/360;
// prueba implementacion control
int set_point = 0;
int offset = 0;
int q1 = 45;
int q2 = -45;
int Kp1 = 2000;
int i = 0;
unsigned long tiempop = 0;
const float pi = 3.1416;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void setup() {
  Setup();
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {
  Principal();
  
}
