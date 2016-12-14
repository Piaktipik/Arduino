// enviador de lecturas analogas

// --------------- defino los puertos de lectura ------------------

const int pin_corriente_1 = 1;  // pin de lectura de el sensor de corriente
const int pin_voltaje_1 = 0;  // pin de lectura de el sensor de corriente

const int pin_corriente_2 = 3;  // pin de lectura de el sensor de corriente
const int pin_voltaje_2 = 2;  // pin de lectura de el sensor de corriente

const int pin_corriente_3 = 5;  // pin de lectura de el sensor de corriente
const int pin_voltaje_3 = 4;  // pin de lectura de el sensor de corriente

const int pin_corriente_4 = 7;  // pin de lectura de el sensor de corriente
const int pin_voltaje_4 = 6;  // pin de lectura de el sensor de corriente

// ----------------- defino i = 0 ---------------------

const int x = 512 ;          // valor 0 de el sensor de corriente

// --------- creo las variables de los sensores -------------------

float corriente_1 = 0;       // valor de la lectura de corriente
float voltaje_1 = 0 ;        // valor de la lectura de voltaje

float corriente_2 = 0;       // valor de la lectura de corriente
float voltaje_2 = 0 ;        // valor de la lectura de voltaje

float corriente_3 = 0;       // valor de la lectura de corriente
float voltaje_3 = 0 ;        // valor de la lectura de voltaje

float corriente_4 = 0;       // valor de la lectura de corriente
float voltaje_4 = 0 ;        // valor de la lectura de voltaje

// -------------- configuracion --------------------- 

void setup() {
  Serial.begin(9600); // inicializo el serial
}

// ---------------- programa principal -----------------

void loop() {
  // ----------------- leo valores analogos -------------------
  
  corriente_1 = analogRead(pin_corriente_1);  // leo sensor de corrinete
  voltaje_1 = analogRead(pin_voltaje_1);      // leo voltaje de divisor
  
  corriente_2 = analogRead(pin_corriente_2);  // leo sensor de corrinete
  voltaje_2 = analogRead(pin_voltaje_2);      // leo voltaje de divisor
  
  corriente_3 = analogRead(pin_corriente_3);  // leo sensor de corrinete
  voltaje_3 = analogRead(pin_voltaje_3);      // leo voltaje de divisor
  
  corriente_4 = analogRead(pin_corriente_4);  // leo sensor de corrinete
  voltaje_4 = analogRead(pin_voltaje_4);      // leo voltaje de divisor
  
  // ------- mapeo los valores de 0-1024 -> 0-3.3 ------
  
  voltaje_1 = ((voltaje_1/1023)*19.3);
  voltaje_2 = ((voltaje_2/1023)*19.5);
  voltaje_3 = ((voltaje_3/1023)*19.3);
  voltaje_4 = ((voltaje_4/1023)*19);
  
  // ---------- linealizo los valores del sensor de corriente ------------
  
  // --- sensor 1
  
  if (corriente_1 > x)
  corriente_1 = ((corriente_1-x)/(1024-x))*(12+3);
  
  else
  corriente_1 = ((corriente_1-x)/(x))*(12+3.8);
  
  // --- sensor 2

  if (corriente_2 > x)
  corriente_2 = ((corriente_2-x)/(1024-x))*(12+3);
  
  else
  corriente_2 = ((corriente_2-x)/(x))*(12+3.8);
  
  // --- sensor 3
  
  if (corriente_3 > x)
  corriente_3 = ((corriente_3-x)/(1024-x))*(12+3);
  
  else
  corriente_3 = ((corriente_3-x)/(x))*(12+3.8);
  
  // --- sensor 4

  if (corriente_4 > x)
  corriente_4 = ((corriente_4-x)/(1024-x))*(12+3);
  
  else
  corriente_4 = ((corriente_4-x)/(x))*(12+3.8);
  
  //--------------------------------------------------------------
  
  // ------- envio los datos por serial(ususario) --------
  Serial.print("I1 = " );                       
  Serial.print(corriente_1);
  Serial.print(", V1 = " );
  Serial.print(voltaje_1);
  
  Serial.print(" | I2 = " );                       
  Serial.print(corriente_2);
  Serial.print(", V2 = " );
  Serial.print(voltaje_2);
  
  Serial.print(" | I3 = " );                       
  Serial.print(corriente_3);
  Serial.print(", V3 = " );
  Serial.print(voltaje_3);
  
  Serial.print(" | I4 = " );                       
  Serial.print(corriente_4);
  Serial.print(", V4 = " );
  Serial.println(voltaje_4);
 
//// -------- envio los datos por serial (maquina) --------                    
//  Serial.print(corriente_1);
//  Serial.print(",");
//  Serial.print(voltaje_1);
//  Serial.print(",");  
//  Serial.print(corriente_2);
//  Serial.print(","); 
//  Serial.print(voltaje_2);
//  Serial.print(",");   
//  Serial.print(corriente_3);
//  Serial.print(","); 
//  Serial.print(voltaje_3);
//  Serial.print(",");   
//  Serial.print(corriente_4);
//  Serial.print(","); 
//  Serial.println(voltaje_4);
  
  // espero un tiempo para evitar colapso del buffer
  delay(300);  
}
