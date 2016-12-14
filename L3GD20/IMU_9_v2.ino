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

int gx, gy, gz;
int ax, ay, az;
int mx, my, mz;

int m_maxx = +540, m_maxy = +500, m_maxz = 180;
int m_minx = -520, m_miny = -570, m_minz = -770;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();

  writeReg(L3G_CTRL_REG1, 0b10101111); 
  
  writeAccReg(LSM303_CTRL_REG1_A, 0x37);
  writeAccReg(LSM303_CTRL_REG4_A, 0x08); // DLHC: enable high resolution mode
  
  writeMagReg(LSM303_MR_REG_M, 0x00);
  
}

void loop() {
  L3G_read();
  readAcc();  
  readMag();
  
  Serial.print("G: ");
  Serial.print("X: ");
  Serial.print(gx);
  Serial.print(" Y: ");
  Serial.print(gy);
  Serial.print(" Z: ");
  Serial.print(gz);
  
  Serial.print(" A: ");
  Serial.print("X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.print(ay);
  Serial.print(" Z: ");
  Serial.print(az);
  
  Serial.print(" M: ");
  Serial.print("X: ");
  Serial.print(mx);
  Serial.print(" Y: ");
  Serial.print(my);
  Serial.print(" Z: ");
  Serial.print(mz);
  
  Serial.println("");
  
  delay(10);
  
}

void L3G_read(){
  Wire.beginTransmission(L3GD20_ADDRESS);
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  Wire.write(L3G_OUT_X_L | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(L3GD20_ADDRESS, 6);

  while (Wire.available() < 6);

  uint8_t xlg = Wire.read();
  uint8_t xhg = Wire.read();
  uint8_t ylg = Wire.read();
  uint8_t yhg = Wire.read();
  uint8_t zlg = Wire.read();
  uint8_t zhg = Wire.read();

  // combine high and low bytes
  gx = (int16_t)(xhg << 8 | xlg);
  gy = (int16_t)(yhg << 8 | ylg);
  gz = (int16_t)(zhg << 8 | zlg);
}


void writeReg(byte reg, byte value){
  Wire.beginTransmission(L3GD20_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void writeAccReg(byte reg, byte value){
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(reg);
  Wire.write(value);Wire.endTransmission();
}

void writeMagReg(byte reg, byte value){
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();

}

// Reads the 3 accelerometer channels and stores them in vector a
void readAcc(void){
  Wire.beginTransmission(ACC_ADDRESS);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  Wire.write(LSM303_OUT_X_L_A | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(ACC_ADDRESS,6);

  unsigned int millis_start = millis();
  bool did_timeout = false;
  unsigned int io_timeout = 200;
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout) {
      did_timeout = true;
      return;
    }
  }
  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  // combine high and low bytes, then shift right to discard lowest 4 bits (which are meaningless)
  // GCC performs an arithmetic right shift for signed negative numbers, but this code will not work
  // if you port it to a compiler that does a logical right shift instead.
  ax = ((int16_t)(xha << 8 | xla)) >> 4;
  ay = ((int16_t)(yha << 8 | yla)) >> 4;
  az = ((int16_t)(zha << 8 | zla)) >> 4;
}



// Reads the 3 magnetometer channels and stores them in vector m

void readMag(void){
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(LSM303_OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom(MAG_ADDRESS, 6);
  unsigned int millis_start = millis();
  bool did_timeout = false;
  unsigned int io_timeout = 200;
  while (Wire.available() < 6) {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout) {
      did_timeout = true;
      return;
    }
  }
byte xhm,xlm,yhm, ylm, zhm, zlm;
  xhm = Wire.read();
  xlm = Wire.read();
  zhm = Wire.read();
  zlm = Wire.read();
  yhm = Wire.read();
  ylm = Wire.read();
  // combine high and low bytes
  mx = (int16_t)(xhm << 8 | xlm);
  my = (int16_t)(yhm << 8 | ylm);
  mz = (int16_t)(zhm << 8 | zlm);
}
