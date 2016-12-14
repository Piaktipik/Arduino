#include <Wire.h>

// giroscopo
#define L3GD20_ADDRESS (0xD6 >> 1)
#define L3G_OUT_X_L       0x28
#define L3G_CTRL_REG1     0x20

// compas
#define MAG_ADDRESS  (0x3C >> 1)
#define ACC_ADDRESS  (0x32 >> 1)
#define LSM303_CTRL_REG1_A       0x20
#define LSM303_CTRL_REG4_A       0x23
#define LSM303_OUT_X_L_A         0x28
#define LSM303_MR_REG_M          0x02
#define LSM303_OUT_X_H_M         0x03

int GYROx, GYROy, GYROz;
int ACCx, ACCy, ACCz;
int MAGx, MAGy, MAGz;

// Bias
float bGYROx, bGYROy, bGYROz;
float bACCx, bACCy, bACCz;

bool ms = 0; //lo uso para mostrar los datos de la imu 1-> datos, 0-> no datos

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
float yawRaw = 0;          // final yaw value. yaw doesnt go through kalman! has its own calc.
float rollPrediction = 0;  // Output of Kalman filter, final roll value

// ---------------- Variables temporales kalman ------------------------

//time, used in kalman filtering to keep a constant loop time
unsigned long timer = 0;
unsigned long timer1 = 0;
float timeStep = 0.02;          //en s. Need a time step value for integration of gyro angle from angle/sec

unsigned int ajuste_t = 0;

void setup() {
  Serial.begin(115200);
  iniciar_IMU();
  CalibrateIMU();
}

void loop() {
  timer = micros(); // tiempo de inicio del loop
  Gyro_read();
  Acc_read();  
  Mag_read();
  // calculo roll, pitch, yaw
  Calculations();
  // muestro valores
  PrintVals();
  // -------------- fin  del programa -------------- 
  // calculo la demora del programa, y ajusto el tiempo de clada ciclo a 20ms
  timer1 = micros(); // tiempo de finalizacion del loop
  ajuste_t = (timer1 - timer);
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
  Serial.print(millis());
  Serial.print(", R: ");
  Serial.print(rollPrediction);
  Serial.print(", P: ");
  Serial.print(pitchPrediction);
  Serial.print(", Y: ");
  Serial.print(yawRaw);
  Serial.println();
}

void PrintVals1(){
  Serial.print(rollPrediction);
  Serial.print(",");
  Serial.print(pitchPrediction);
  Serial.print(",");
  Serial.print(yawRaw);
  Serial.println();
}
  
