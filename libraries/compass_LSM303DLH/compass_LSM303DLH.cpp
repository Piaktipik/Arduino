/* LSM303DLH Example Code
   by: Jim Lindblom
   SparkFun Electronics
   date: 9/6/11
   license: Creative commons share-alike v3.0
   
   Summary:
   Show how to calculate level and tilt-compensated heading using
   the snazzy LSM303DLH 3-axis magnetometer/3-axis accelerometer.
   
   Firmware:
   You can set the accelerometer's full-scale range by setting
   the SCALE constant to either 2, 4, or 8. This value is used
   in the initLSM303() function. For the most part, all other
   registers in the LSM303 will be at their default value.
   
   Use the LSM303_write() and LSM303_read() functions to write
   to and read from the LSM303's internal registers.
   
   Use getLSM303_accel() and getLSM303_mag() to get the acceleration
   and magneto values from the LSM303. You'll need to pass each of
   those functions an array, where the data will be stored upon
   return from the void.
   
   getHeading() calculates a heading assuming the sensor is level.
   A float between 0 and 360 is returned. You need to pass it a
   array with magneto values. 
   
   getTiltHeading() calculates a tilt-compensated heading.
   A float between 0 and 360 degrees is returned. You need
   to pass this function both a magneto and acceleration array.
   
   Headings are calculated as specified in AN3192:
   http://www.sparkfun.com/datasheets/Sensors/Magneto/Tilt%20Compensated%20Compass.pdf
   
   Hardware:
   I'm using SparkFun's LSM303 breakout. Only power and the two
   I2C lines are connected:
   LSM303 Breakout ---------- Arduino
         Vin                   5V
         GND                   GND
         SDA                   A4
         SCL                   A5
*/

#include <Wire.h>
#include <math.h>
#include "compass_LSM303DLH.h"

LSM303DLHClass compass;

//LSM303DLHClass compass;

// Constructors ////////////////////////////////////////////////////////////////

LSM303DLHClass::LSM303DLHClass()
{

}

// Public Methods //////////////////////////////////////////////////////////////


void LSM303DLHClass::begin(int fs)
{
  write(0x27, CTRL_REG1_A);  // 0x27 = normal power mode, all accel axes on
  if ((fs==8)||(fs==4))
    write((0x00 | (fs-fs/2-1)<<4), CTRL_REG4_A);  // set full-scale
  else
    write(0x00, CTRL_REG4_A);
  write(0x14, CRA_REG_M);  // 0x14 = mag 30Hz output rate
  write(0x00, MR_REG_M);  // 0x00 = continouous conversion mode
}


float LSM303DLHClass::getHeading(int * magValue)
{
  // see section 1.2 in app note AN3192
  float heading = 180*atan2(magValue[Y], magValue[X])/PI;  // assume pitch, roll are 0
  
  if (heading <0)
    heading += 360;
  
  return heading;
}

float LSM303DLHClass::getTiltHeading(int * magValue, float * accelValue)
{
  // see appendix A in app note AN3192 
  float pitch = asin(-accelValue[X]);
  float roll = asin(accelValue[Y]/cos(pitch));
  
  float xh = magValue[X] * cos(pitch) + magValue[Z] * sin(pitch);
  float yh = magValue[X] * sin(roll) * sin(pitch) + magValue[Y] * cos(roll) - magValue[Z] * sin(roll) * cos(pitch);
  float zh = -magValue[X] * cos(roll) * sin(pitch) + magValue[Y] * sin(roll) + magValue[Z] * cos(roll) * cos(pitch);

  float heading = 180 * atan2(yh, xh)/PI;
  if (yh >= 0)
    return heading;
  else
    return (360 + heading);
}

void LSM303DLHClass::get_mag(int * rawValues)
{
  Wire.beginTransmission(LSM303_MAG);
  Wire.write(OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom(LSM303_MAG, 6);
  for (int i=0; i<3; i++)
    rawValues[i] = (Wire.read() << 8) | Wire.read();
}

void LSM303DLHClass::get_accel(int * rawValues)
{
  rawValues[Z] = ((int)read(OUT_X_L_A) << 8) | (read(OUT_X_H_A));
  rawValues[X] = ((int)read(OUT_Y_L_A) << 8) | (read(OUT_Y_H_A));
  rawValues[Y] = ((int)read(OUT_Z_L_A) << 8) | (read(OUT_Z_H_A));  
  // had to swap those to right the data with the proper axis
}

unsigned char LSM303DLHClass::read(unsigned char address)
{
  unsigned char temp;
  
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
    
  Wire.write(address);
  
  if (address >= 0x20)
    Wire.requestFrom(LSM303_ACC, 1);
  else
    Wire.requestFrom(LSM303_MAG, 1);
  while(!Wire.available())
    ;
  temp = Wire.read();
  Wire.endTransmission();
  
  return temp;
}

void LSM303DLHClass::write(unsigned char data, unsigned char address)
{
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
    
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}