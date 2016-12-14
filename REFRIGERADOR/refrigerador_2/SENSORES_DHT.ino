
////////////////////////////////////////  LIBRERIA SENSORES DHT ////////////////////////////////////// 
#include "DHT.h"

// DEFINO LOS PUERTOS DE CONEXION DE LOS SENSORES
#define DHTPIN1 12    // SENSOR 1
#define DHTPIN2 13    // SENSOR 2

// DEFINO EL TIPO DE SENSOR PARA LA LIBRERIA

#define DHTTYPE1 DHT11     // DHT 11 
#define DHTTYPE2 DHT11     // DHT 11 

// EXPLICACION CONEXION
// pin 1: A LA IZQUIERDA -> A VCC (3V - 5V)
// pin 2: CONECTADO AL DHTPIN ASIGNADO
// pin 4: A LA DERACHA -> A GROUND
// USAMOS UNA RESISTENCIA 5K ENTRE EL PIN 2 (DATOS) Y EL PIN 1 (VCC) DEL SENSOR (PARA AYUDAR CON LOS RUIDOS)

// INICIALIZAMOS EL  DHT sensor for normal 16mhz Arduino
DHT dht1(DHTPIN1, DHTTYPE1,3);
DHT dht2(DHTPIN2, DHTTYPE2,3);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);



void iniciar_dht(){
  dht1.begin();  // SE INICIA EL SENSOR 1
  dht2.begin();  // SE INICIA EL SENSOR 2
  
  leer_cal_dht();
}
  
void medir_dht11(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  // cargo las medicones en las variables globales
  VALOR_DHT1_H = dht1.readHumidity() + float(CAL_DHT1_H-100);
  // Read temperature as Celsius
  VALOR_DHT1_T = dht1.readTemperature() + float(CAL_DHT1_T-100);
  
  VALOR_DHT2_H = dht2.readHumidity() + float(CAL_DHT2_H-100);
  // Read temperature as Celsius
  VALOR_DHT2_T = dht2.readTemperature() + float(CAL_DHT2_T-100);
}

void mostrar_dht(){
  // Check if any reads failed and exit early (to try again).
  if (isnan(VALOR_DHT1_H) || isnan(VALOR_DHT1_T)) {
    Serial.println("Imposible leer sensor 1");
  }else{
    Serial.print("H1: "); 
    Serial.print(VALOR_DHT1_H);
    Serial.print("\t");
    Serial.print("T1: "); 
    Serial.print(VALOR_DHT1_T);
  }
  if (isnan(VALOR_DHT2_H) || isnan(VALOR_DHT2_T)) {
    Serial.println("Imposible leer sensor 2");
  }else{
    Serial.print("\t");
    Serial.print("H1: "); 
    Serial.print(VALOR_DHT2_H);
    Serial.print("\t");
    Serial.print("T1: "); 
    Serial.print(VALOR_DHT2_T);
  }
}

//void calibrar_dht(int val1, int val2, int val3, int val4){
//  CAL_DHT1_T = val1;    // valor de calibracion sensor 1 Temperatura
//  CAL_DHT1_H = val2;    // valor de calibracion senosr 1 Humedad
//  
//  CAL_DHT2_T = val3;    // valor de calibracion sensor 2 Temperatura
//  CAL_DHT2_H = val4;    // valor de calibracion senosr 2 Humedad
//  
//}

// ----------------- leeo los datos almacenados en la eeprom -----------------
void leer_cal_dht(){
  CAL_DHT1_T = (EEPROM.read(0));
  CAL_DHT1_H = (EEPROM.read(1));
  CAL_DHT2_T = (EEPROM.read(2));
  CAL_DHT2_H = (EEPROM.read(3));
}

void escribir_cal_dht(byte t1, byte h1, byte t2, byte h2){
  if (EEPROM.read(0) != t1) EEPROM.write(0,t1);
  if (EEPROM.read(1) != h1) EEPROM.write(1,h1);
  if (EEPROM.read(2) != t2) EEPROM.write(2,t2);
  if (EEPROM.read(3) != h2) EEPROM.write(3,h2);
  
  // al finalizar actulizo los valores
  leer_cal_dht();
}
