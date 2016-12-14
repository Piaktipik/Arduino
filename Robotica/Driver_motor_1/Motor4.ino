
//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ----- encoder 4 -----
int ca4 = 0;  int cb4 = 0;
int pinA4 = 52;
int pinB4 = 53;
int est_enc4 = 0;
long pas_tot4 = 0;

float pasprev4 = 500;
unsigned long ultimotpa4 = 0;
unsigned long ultimotpb4 = 0;
unsigned long ultimopa4 = 0;
unsigned long ultimopb4 = 0;


void activarEncoder4(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  ultimotpa4 = 0;
  ultimotpb4 = 0;
  ultimopa4 = 0;
  ultimopb4 = 0;
  // se verifica el estado 
  est_enc4 = obt_est4(pA,pB);
  // 
  esp_pro_est4(pA, pB, est_enc4);
}

void ENCODERA4() {
  // obtengo el delta de tiempo entre flancos
  ultimopa4 = ultimotpa4 - micros();
  ultimotpa4 = micros();

  pro_est_A4(pinA4,pinB4,est_enc4);
  // aumento el contador de pasos
  ca4++;
}

void ENCODERB4() {
  // obtengo el delta de tiempo entre flancos
  ultimopb4 = ultimotpb4 - micros();
  ultimotpb4 = micros();

  pro_est_B4(pinA4,pinB4,est_enc4);
  // aumento el contador de pasos
  cb4++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est4(int pA,int pB){
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

void esp_pro_est4(int pA,int pB,int est_enc){
  switch(est_enc4){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA4, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB4, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA4, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB4, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA4, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB4, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA4, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB4, FALLING);
      break;
  }
}

int pro_est_A4(int pA,int pB,int estado){
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
  est_enc4 = x;
  esp_pro_est4(pA,pB,x);
}

void pro_est_B4(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tot4--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tot4++;
      break;
  }
  est_enc4 = x;
  esp_pro_est4(pA,pB,x);
}

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ control motores  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void parar4() {
  digitalWrite(pin_dire_moto_4, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_4, 254);
}
void adelante4() {
  digitalWrite(pin_dire_moto_4, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_4, velo_moto_4);
}
void atras4() {
  digitalWrite(pin_dire_moto_4, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_4, velo_moto_4);
}

void mover4g(int grados){
    //Serial.println("" + grados);
    if(pas_tot4 > grados*pasosgra3){
      atras4();
      while(pas_tot4 > pasosgra3*grados){Serial.println(String(pas_tot4));delay(100);};
      // paramos rapidamente el motor
      adelante4();
      delay(1);
      parar4();
    }else{
      adelante4();
      while(pas_tot4 < pasosgra3*grados){Serial.println(String(pas_tot4));delay(100);};
      // paramos rapidamente el motor
      atras4();
      delay(1);
      parar4();
    }  
  }


void velocidad4(int vel){
  vel = abs(vel);
  if (vel > 255){vel=255;}
  velo_moto_4 = vel;
  velo_moto_4 = vel;
}


