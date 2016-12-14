const byte u1 = 30;   // pin ultrasonido 1
const byte u2 = 28;   // pin ultrasonido 2
const byte u3 = 26;  // pin ultrasonido 3                                              

unsigned long d1,d2,d3;
int di1,di2,di3;
bool mu1 = false,mu2 = false,mu3 = false;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  // apago ventilador
  pinMode(53,OUTPUT);
  digitalWrite(53,0);
}

void loop(){
  srf05_1();
  srf05_2();
  srf05_3();
  delay(50);
  Serial.print("di1 = ");
  Serial.print(di1);
  Serial.print("\t di2 = ");
  Serial.print(di2);
  Serial.print("\t di3 = ");
  Serial.print(di3);
  Serial.println();
  
}

// funcion para medir los ultrasonidos
void srf05_1(){
  detachInterrupt(u1);
  d1 = 0;di1 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u1, OUTPUT);digitalWrite(u1, HIGH);
  delayMicroseconds(10);
  digitalWrite(u1, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u1, INPUT);
  mu1 = false;attachInterrupt(u1, fu1, RISING);mu1 = true;
}
void srf05_2(){
  detachInterrupt(u2);
  d2 = 0;di2 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u2, OUTPUT);digitalWrite(u2, HIGH);
  delayMicroseconds(10);
  digitalWrite(u2, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u2, INPUT);
  mu2 = false;attachInterrupt(u2, fu2, RISING);mu2 = true;
}
void srf05_3(){  
  detachInterrupt(u3);
  d3 = 0;di3 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u3, OUTPUT);digitalWrite(u3, HIGH);
  delayMicroseconds(10);
  digitalWrite(u3, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u3, INPUT);
  mu3 = false;attachInterrupt(u3, fu3, RISING);mu3 = true;
}


void fu1(){
  if (mu1){
    if (d1 == 0){d1 = micros();attachInterrupt(u1, fu1, FALLING);} 
    else{d1 = micros()-d1; di1 = d1/58.30;} 
  }
}

void fu2(){
  if (mu2){
    if (d2 == 0){d2 = micros();attachInterrupt(u2, fu2, FALLING);}
    else{d2 = micros()-d2; di2 = d2/58.30;}
  }
}
void fu3(){
  if (mu3){
  if (d3 == 0){d3 = micros();attachInterrupt(u3, fu3, FALLING);}
  else{d3 = micros()-d3; di3 = d3/58.30;}
  }
}

