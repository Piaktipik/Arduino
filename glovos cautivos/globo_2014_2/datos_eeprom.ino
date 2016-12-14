  
// ---------------- almaceno los parametros en la eeprom ---------------
void eeprom_tiempo(byte tie){
  if (EEPROM.read(0) != tie) EEPROM.write(0,tie);
}

void eeprom_velserial(byte vel){
  if (EEPROM.read(1) != vel) EEPROM.write(1,vel);
}
// ----------------- leeo los datos almacenados en la eeprom -----------------

byte leer_tie(){
  return EEPROM.read(0);
}

byte leer_vel(){
  return EEPROM.read(1);
}

// -------- muestro datos eeprom -----------
void mostrar_eeprom(){
  Serial.print("<ee/>");
  Serial.print("t");
  Serial.print(leer_tie());
  Serial.print("v");
  Serial.print(leer_vel());
  Serial.print("</ee>");
}
