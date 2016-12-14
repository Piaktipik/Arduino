
//---------------------Funciones de IMU------------------------------------
void iniciar_IMU(){
  // ---- Inicializacion de IMU ----
  Wire.begin(); //start Wire for IMU
  
  writeGyroReg(L3G4200D_CTRL_REG1, 0b10101111); //enable gyro, 0b10101111, 1010- 400hz ODR/50hz cutoff, 1111- default(enable all axis/normal mode)
  writeAccReg(LSM303_CTRL_REG1_A, 0b00110111);  //enable acc, 0b00110111, 001- normal mode, 10- 400hz ODR, 111- default(enable all axis)
  writeMagReg(LSM303_MR_REG_M, 0x00); //enable mag
  
  //gyro settings
  //writeGyroReg(CTRL_REG4, 0b00110000); //0-continous update, 0- little endian, 11 2000dps, 0- blank,  00- self test disabled, 0 spi 4 wire(if used)

  //acc settings
  writeAccReg(CTRL_REG4_A, 0b00110000); //0- continuous update, 0- little endian, 11- 8g scale, 00 default, 0- blank, 0- self test disabled
}

//get level value bias of IMU sensors
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
    
    readGyro();
    readAcc();
    
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

// get x, y, z values from gyro
void readGyro(){
  // 0 -> write, 1 -> read
  
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(L3G4200D_OUT_X_L | (1 << 7));
  Wire.endTransmission();
  
  Wire.requestFrom(GYR_ADDRESS, 6);

  while (Wire.available() < 6);    // espero los datos del sensor

  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  GYROy = xha << 8 | xla;  // pego parte  y parte baja de los datos que llegan
  GYROx = yha << 8 | yla;
  GYROz = zha << 8 | zla;
  
  if (ms){
    Serial.print("GYRO: ");
    Serial.print(GYROx);
    Serial.print(", ");
    Serial.print(GYROy);
    Serial.print(", ");
    Serial.print(GYROz);
    Serial.print(", ");
  }
}

//write stuff to the snsor registers
void writeGyroReg(byte reg, byte value){
  
  Wire.beginTransmission(GYR_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// --------------------------------- ACC -----------------------------------

// get x, y, z values from accelerometer
void readAcc(){
  
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(LSM303_OUT_X_L_A | (1 << 7));
  Wire.endTransmission();
  Wire.requestFrom(ACC_ADDRESS, 6);

  while (Wire.available() < 6); // espero los datos del sensor

  byte xla = Wire.read();
  byte xha = Wire.read();
  byte yla = Wire.read();
  byte yha = Wire.read();
  byte zla = Wire.read();
  byte zha = Wire.read();

  ACCy = (xha << 8 | xla); //reversed y axis
  ACCx = (yha << 8 | yla); //reversed x
  ACCz = (zha << 8 | zla);

//  ACCy = -((xha << 8 | xla) >> 4); //reversed y axis
//  ACCx = -((yha << 8 | yla) >> 4); //reversed x
//  ACCz = (zha << 8 | zla) >> 4;
  
  if (ms){
    Serial.print("ACC: ");
    Serial.print(ACCx);
    Serial.print(", ");
    Serial.print(ACCy);
    Serial.print(", ");
    Serial.print(ACCz);
    Serial.print(", ");
  }
}

//write stuff to the snsor registers
void writeAccReg(byte reg, byte value){
  
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// --------------------------------- MAG -----------------------------------
//get mag x, y, z values
void readMag(){
  
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(LSM303_OUT_X_H_M);
  Wire.endTransmission();
  Wire.requestFrom(MAG_ADDRESS, 6);

  while (Wire.available() < 6);    // espero los datos del sensor

  byte xhm = Wire.read();
  byte xlm = Wire.read();
  byte zhm = Wire.read();
  byte zlm = Wire.read();
  byte yhm = Wire.read();
  byte ylm = Wire.read();

  MAGx = (xhm << 8 | xlm);
  MAGy = (yhm << 8 | ylm);
  MAGz = (zhm << 8 | zlm);
  
  if (ms){
    Serial.print("MAG: ");
    Serial.print(MAGx);
    Serial.print(", ");
    Serial.print(MAGy);
    Serial.print(", ");
    Serial.print(MAGz);
    Serial.print(", ");
  }
}

//write stuff to the snsor registers
void writeMagReg(byte reg, byte value){
  
  Wire.beginTransmission(MAG_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
