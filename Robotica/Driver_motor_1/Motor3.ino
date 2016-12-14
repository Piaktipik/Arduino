
//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ----- encoder 3 -----
int ca3 = 0;  int cb3 = 0;
int pinA3 = 48;
int pinB3 = 50;
int est_enc3 = 0;
long pas_tot3 = 0;

float pasprev3 = 500;
unsigned long ultimotpa3 = 0;
unsigned long ultimotpb3 = 0;
unsigned long ultimopa3 = 0;
unsigned long ultimopb3 = 0;


void activarEncoder3(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  ultimotpa3 = 0;
  ultimotpb3 = 0;
  ultimopa3 = 0;
  ultimopb3 = 0;
  // se verifica el estado 
  est_enc3 = obt_est3(pA,pB);
  // 
  esp_pro_est3(pA, pB, est_enc3);
}

void ENCODERA3() {
  // obtengo el delta de tiempo entre flancos
  ultimopa3 = ultimotpa3 - micros();
  ultimotpa3 = micros();

  pro_est_A3(pinA3,pinB3,est_enc3);
  // aumento el contador de pasos
  ca3++;
}

void ENCODERB3() {
  // obtengo el delta de tiempo entre flancos
  ultimopb3 = ultimotpb3 - micros();
  ultimotpb3 = micros();

  pro_est_B3(pinA3,pinB3,est_enc3);
  // aumento el contador de pasos
  cb3++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est3(int pA,int pB){
  int x = digitalRead(pA)*2 + digitalRead(pB);
  switch(x){
    case 0:
      x = 1;
      break;
    case 2:
      x = 2;
      break;
    case 3:
      x = 3;
      break;
    case 1:
      x = 4;
      break;
  }
  return x; 
}

void esp_pro_est3(int pA,int pB,int est_enc){
  switch(est_enc3){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA3, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB3, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA3, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB3, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA3, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB3, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA3, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB3, FALLING);
      break;
  }
}

int pro_est_A3(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 2;
      break;
    case 2:
      x = 1;
      break;
    case 3:
      x = 4;
      break;
    case 4:
      x = 3;
      break;
  }
  est_enc3 = x;
  esp_pro_est3(pA,pB,x);
}

void pro_est_B3(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tot3--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tot3++;
      break;
  }
  est_enc3 = x;
  esp_pro_est3(pA,pB,x);
}

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ control motores  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void parar3() {
  digitalWrite(pin_dire_moto_3, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_3, 254);
}
void adelante3() {
  digitalWrite(pin_dire_moto_3, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_3, velo_moto_3);
}
void atras3() {
  digitalWrite(pin_dire_moto_3, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_3, velo_moto_3);
}

void mover3g(int grados){
    //Serial.println("" + grados);
    if(pas_tot3 > grados*pasosgra2){
      atras3();
      while(pas_tot3 > pasosgra2*grados){Serial.println(String(pas_tot3));delay(100);};
      // paramos rapidamente el motor
      adelante3();
      delay(1);
      parar3();
    }else{
      adelante3();
      while(pas_tot3 < pasosgra2*grados){Serial.println(String(pas_tot3));delay(100);};
      // paramos rapidamente el motor
      atras3();
      delay(1);
      parar3();
    }  
  }


void velocidad3(int vel){
  vel = abs(vel);
  if (vel > 255){vel=255;}
  velo_moto_3 = vel;
  velo_moto_3 = vel;
}


