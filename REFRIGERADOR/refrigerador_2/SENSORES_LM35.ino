
// ----------- PUERTOS LM35 -------
// conexion: 1 - +VS, 2 - VOUT, 3 - GND
// PARA EL SENSOR 3
const int PIN_LM35_1 = A1;   // pin sensor
// PARA EL SENSOR 4
const int PIN_LM35_2 = A2;   // pin sensor



void medir_lm35(){
  // ----------- medicion de los lm35 ----------- 
  // -> relacion 10mV/C
  // ADC 1024 -> 5V |-> 5V/1024 -> 0.0048828 -> 0.0048828/0.01 -> 0.48828 
  // ADC 1024 -> 3V |-> 3V/1024 -> 0.0029297 -> 0.0029297/0.01 -> 0.29297 
  // leeo el sensor 3
  VALOR_LM35_1 = analogRead(PIN_LM35_1);
  VALOR_LM35_1 = VALOR_LM35_1*0.29297 + float(CAL_LM35_1-100);
  // leeo el sensor 4
  VALOR_LM35_2 = analogRead(PIN_LM35_2);
  VALOR_LM35_2 = VALOR_LM35_2*0.29297 + float(CAL_LM35_2-100);
}

void mostrar_lm35(){
  Serial.print("\t");
  Serial.print("LM35_1: ");
  Serial.print(VALOR_LM35_1);
  
  Serial.print("\t");
  Serial.print("LM35_2: ");
  Serial.print(VALOR_LM35_2);
}

void calibrar_lm35(int val1, int val2){
  CAL_LM35_1 = val1;  // valor de calibracion sensor 1
  CAL_LM35_2 = val2;  // valor de calibracion sensor 2
}

// ----------------- leeo los datos almacenados en la eeprom -----------------
void leer_cal_lm35(){
  CAL_LM35_1 = (EEPROM.read(4));
  CAL_LM35_2 = (EEPROM.read(5));
}

void escribir_cal_lm35(byte t3, byte t4){
  if (EEPROM.read(4) != t3) EEPROM.write(4,t3);
  if (EEPROM.read(5) != t4) EEPROM.write(5,t4);
  // al finalizar actulizo los valores
  leer_cal_lm35();
}
