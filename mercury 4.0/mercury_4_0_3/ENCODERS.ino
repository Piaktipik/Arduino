
// -------------------------------- encoders -------------------------------- 
// ------------- variables encoders ------------- 

// Encoder 1 - Izquierdo
int paso_1 = 0;
// Encoder 2 - Derecho
int paso_2 = 0;

unsigned long t1 = 0;
unsigned long t2 = 0;

// ---- se configuran las interrupciones de los encoders ---- 
void iniciar_encoders(){
  attachInterrupt(2,enc_1,RISING); // pin 0 is interrupt 2
  attachInterrupt(3,enc_2,RISING); // pin 1 is interrupt 3
}

// ------------- funciones interrupcion ------------- 
void enc_1(){
  paso_1++; 
}

void enc_2(){  
  paso_2++;
}

void calcular_vel(){
  // velocidad motor izquierdo
  t1 = millis()-t1;
  veli = (PI*1000.0*paso_1)/(6.0*t1);
  paso_1 = 0;t1 = millis();
  
  // velocidad motor derecho
  t2 = millis()-t2;
  veld = (PI*1000.0*paso_2)/(6.0*t2);
  paso_2 = 0;t2 = millis();
}

void mostar_encoders(){
  Serial.print(paso_1);
  Serial.print(",");
  Serial.print(paso_2);
  Serial.print(",");
}
