// por: julian galvez
#include <Wire.h>    // libreria serial
// L3G4200D -> GYRO
// LSM303   -> ACC, MAG
// --------------------- variables IMU --------------------- 
// I2C addressses for IMU 
//#define GYR_ADDRESS (0xD2 >> 1) // datasheet
#define MAG_ADDRESS (0x3C >> 1) // datasheet
#define ACC_ADDRESS (0x30 >> 1) // datasheet

// Gyro, ACC, Mag enable registers (posicion)
//#define L3G4200D_CTRL_REG1 0x20
#define LSM303_CTRL_REG1_A 0x20
#define LSM303_MR_REG_M 0x02

// acc settings 
#define CTRL_REG4_A 0x23

// gyro settings
#define CTRL_REG4 0x23

// Gyro, Acc, Mag output registers 
//#define L3G4200D_OUT_X_L 0x28
#define LSM303_OUT_X_L_A 0x28
#define LSM303_OUT_X_H_M 0x03

// ACC Reg data
float ACCx, ACCy, ACCz;
// promedio al origen
float bACCx, bACCy, bACCz;
//// Gyro Reg data
//float GYROx, GYROy, GYROz;
//// Bias
//float bGYROx, bGYROy, bGYROz;

// MAG REG data
int MAGx, MAGy, MAGz;

bool ms = 1;

// YAW calc
int xMAGMax, yMAGMax, zMAGMax;
int xMAGMin, yMAGMin, zMAGMin;
float xMAGMap, yMAGMap, zMAGMap;

// calculations
//float pitchAccel, pitchGyro;
//float rollAccel, rollGyro;

// gyro/acc gain-converts raw values, gyro deg/s, acc to Gs
#define AccGain 3.9      //8g
//#define GyroGain 70     //2000dps

// kalman,
//float giroVar = 0.1;
//float deltaGiroVar = 0.1;
//float accelVar = 5;
//float Pxx = 0.1; // angle variance
//float Pvv = 0.1; // angle change rate variance
//float Pxv = 0.1; // angle and angle change rate covariance
//float kx, kv;
//
//// final values
//float pitchPrediction = 0; // Output of Kalman filter, final pitch value
//float yawRaw = 0;          // final yaw value. yaw doesnt go through kalman! has its own calc.
//
//float rollPrediction = 0;  // Output of Kalman filter, final roll value
//
//float rollmax = 0;
//float rollmin = 0;
//float rollajustado = 0;  // Output of Kalman filter, final roll value

//----------------------Fin de configuracion IMU-------------------------------------------------------

void setup() {
  Serial.begin(9600);
  iniciar_IMU();        // configuro IMU y inicializo
  //CalibrateIMU();       // calibrate the IMU for level starting pos.
  
}

void loop() {
  //readGyro();
  readAcc();
  readMag();
  Serial.println("");
  delay(10);
}
