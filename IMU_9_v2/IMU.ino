
//---------------------Funciones de IMU------------------------------------
void iniciar_IMU(){
  // ---- Inicializacion de IMU ----
  Wire.begin();
  write_Reg(L3GD20_ADDRESS,L3G_CTRL_REG1, 0b10101111); 
  write_Reg(ACC_ADDRESS,LSM303_CTRL_REG1_A, 0x37);
  write_Reg(ACC_ADDRESS,LSM303_CTRL_REG4_A, 0x08); // DLHC: enable high resolution mode
  write_Reg(MAG_ADDRESS,LSM303_MR_REG_M, 0x00);
}

// se escribe los datos al giroscopo
void write_Reg(byte dir,byte reg, byte value){
  Wire.beginTransmission(dir);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// get level value bias of IMU sensors
void CalibrateIMU(){
  //temporary total holders
  int tGYROx;
  int tGYROy;
  int tGYROz;
  
  int tACCx;
  int tACCy;
  int tACCz;

  delay(10); //wait for stable values
  
  // --------------- tomamos 100 valores y los promedio --------------- 
  for(int i = 0; i<50; i++){
    
    Gyro_read();
    Acc_read();
    
    tGYROx += GYROx; // total gyrox value += current reading
    tGYROy += GYROy;
    tGYROz += GYROz;
    tACCx += ACCx;
    tACCy += ACCy;
    tACCz += ACCz;
    
    delay(20);    // leo cada 20ms
  }

  bGYROx = float(tGYROx) / 50; // bias in gyro x = total gyro x/50
  bGYROy = float(tGYROy) / 50;
  bGYROz = float(tGYROz) / 50;

  bACCx = float(tACCx) / 50;
  bACCy = float(tACCy) / 50;
  bACCz = float(tACCz) / 50; // ? Don't compensate gravity away! We would all (float)!
  
}

// --------------------------------- GYRO -----------------------------------

// se obtienen x, y, z del giroscopo
void Gyro_read(){
  Wire.beginTransmission(L3GD20_ADDRESS);
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  Wire.write(L3G_OUT_X_L | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(L3GD20_ADDRESS, 6);

  unsigned int io_timeout = 10; // demora maxima us
  unsigned int millis_start = micros(); // tiempo de inicio
  while (Wire.available() < 6) {
    if ((micros() - millis_start) > io_timeout) {
      return; // si demora mucho finalizo la funcion
    }
  }
  int td = micros() - millis_start;
  
  
  uint8_t xlg = Wire.read();
  uint8_t xhg = Wire.read();
  uint8_t ylg = Wire.read();
  uint8_t yhg = Wire.read();
  uint8_t zlg = Wire.read();
  uint8_t zhg = Wire.read();

  // combine high and low bytes
  GYROx = (int16_t)(xhg << 8 | xlg);
  GYROy = (int16_t)(yhg << 8 | ylg);
  GYROz = (int16_t)(zhg << 8 | zlg);
  
  if (ms){
    Serial.print(" GYRO: ");
    Serial.print(GYROx);
    Serial.print(", ");
    Serial.print(GYROy);
    Serial.print(", ");
    Serial.print(GYROz);
    Serial.print(", t: ");
    Serial.print(td);
  }
  
}

// --------------------------------- ACC -----------------------------------

// se obtienen x, y, z del acelerometro
void Acc_read(void){
  Wire.beginTransmission(ACC_ADDRESS);
  // assert the MSB of the address to get the accelerometer
  // to do slave-transmit subaddress updating.
  Wire.write(LSM303_OUT_X_L_A | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(ACC_ADDRESS,6);

  unsigned int io_timeout = 10; // demora maxima us
  unsigned int millis_start = micros(); // tiempo de inicio
  while (Wire.available() < 6) {
    if ((micros() - millis_start) > io_timeout) {
      return; // si demora mucho finalizo la funcion
    }
  }
  int td = micros() - millis_start;
  
  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  // combine high and low bytes, then shift right to discard lowest 4 bits (which are meaningless)
  // GCC performs an arithmetic right shift for signed negative numbers, but this code will not work
  // if you port it to a compiler that does a logical right shift instead.
  ACCx = ((int16_t)(xha << 8 | xla)) >> 4;
  ACCy = ((int16_t)(yha << 8 | yla)) >> 4;
  ACCz = ((int16_t)(zha << 8 | zla)) >> 4;
  
  if (ms){
    Serial.print(" ACC: ");
    Serial.print(ACCx);
    Serial.print(", ");
    Serial.print(ACCy);
    Serial.print(", ");
    Serial.print(ACCz);
    Serial.print(", t: ");
    Serial.print(td);
  }
  
}

// --------------------------------- MAG -----------------------------------

// se obtienen x, y, z del magnetometro
void Mag_read(void){
  
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(LSM303_OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom(MAG_ADDRESS, 6);
  
  // espero los datos 
  unsigned int io_timeout = 10; // demora maxima us
  unsigned int millis_start = micros(); // tiempo de inicio
  while (Wire.available() < 6) {
    if ((micros() - millis_start) > io_timeout) {
      return; // si demora mucho finalizo la funcion
    }
  }
  int td = micros() - millis_start;
  
  byte xhm,xlm,yhm, ylm, zhm, zlm;
  xhm = Wire.read();
  xlm = Wire.read();
  zhm = Wire.read();
  zlm = Wire.read();
  yhm = Wire.read();
  ylm = Wire.read();
  // combine high and low bytes
  MAGx = (int16_t)(xhm << 8 | xlm);
  MAGy = (int16_t)(yhm << 8 | ylm);
  MAGz = (int16_t)(zhm << 8 | zlm);
  
   if (ms){
    Serial.print(" MAG: ");
    Serial.print(MAGx);
    Serial.print(", ");
    Serial.print(MAGy);
    Serial.print(", ");
    Serial.print(MAGz);
    Serial.print(", ");
    Serial.print(", t: ");
    Serial.print(td);
  }
  
}
