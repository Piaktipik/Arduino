  
// ---------------- almaceno los parametros en la eeprom ---------------
void parametros(byte tie1){
  if (EEPROM.read(0) != tie1) EEPROM.write(0,tie1);
  //if (EEPROM.read(1) != tie2) EEPROM.write(1,tie2);
}
  
// ----------------- leeo los datos almacenados en la eeprom -----------------
byte leer_tie(){
  byte x = (EEPROM.read(0));
  //int x = (EEPROM.read(0) << 8 | EEPROM.read(1));
  return x;
}

// conversion de 10bits to 2 bytes
byte bytesh(int val){
  return val >> 8;
}

byte bytesl(int val){
  return val;
}
