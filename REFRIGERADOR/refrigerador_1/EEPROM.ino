

void leer_cal_temp(){
  T_MAX_CRIT = (EEPROM.read(9));
  T_MAX_ESTA = (EEPROM.read(10));
  T_MIN_ESTA = (EEPROM.read(11));
  T_MIN_CRIT = (EEPROM.read(12));
}

void escribir_cal_temp(byte t1, byte t2, byte t3, byte t4){
  if (EEPROM.read(9) != t1) EEPROM.write(9,t1);
  if (EEPROM.read(10) != t2) EEPROM.write(10,t2);
  if (EEPROM.read(11) != t3) EEPROM.write(11,t3);
  if (EEPROM.read(12) != t4) EEPROM.write(12,t4);
  
  // al finalizar actulizo los valores
  leer_cal_temp();
}

void mostrar_cal_temp(){
  Serial.print("\t");
  Serial.print("TMAC: "); 
  Serial.print(T_MAX_CRIT);
  Serial.print("\t");
  Serial.print("TMAE: "); 
  Serial.print(T_MAX_ESTA);
  Serial.print("\t");
  Serial.print("TMIE: "); 
  Serial.print(T_MIN_ESTA);
  Serial.print("\t");
  Serial.print("TMIC: "); 
  Serial.print(T_MIN_CRIT);
}


void leer_temp_lim(){
  T_MAX = (EEPROM.read(13));
  T_MIN = (EEPROM.read(14));
}

void escribir_temp_lim(byte t1, byte t2){
  if (EEPROM.read(13) != t1) EEPROM.write(13,t1);
  if (EEPROM.read(14) != t2) EEPROM.write(14,t2);
  // al finalizar actulizo los valores
  leer_temp_lim();
}

void mostrar_temp_lim(){
  Serial.print("\t");
  Serial.print("TMAX: "); 
  Serial.print(T_MAX);
  Serial.print("\t");
  Serial.print("TMIN: "); 
  Serial.print(T_MIN);
}


// conversion de 10bits to 2 bytes
byte bytesh(int val){
  return val >> 8;
}

byte bytesl(int val){
  return val;
}
