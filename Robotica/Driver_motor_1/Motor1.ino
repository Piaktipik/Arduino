
//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ----- encoder 1 -----
int ca1 = 0;  int cb1 = 0;
int pinA1 = 40;
int pinB1 = 42;
int est_enc1 = 0;
long pas_tot1 = 0;

float pasprev1 = 500;
unsigned long ultimotpa1 = 0;
unsigned long ultimotpb1 = 0;
unsigned long ultimopa1 = 0;
unsigned long ultimopb1 = 0;


void activarEncoder1(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  ultimotpa1 = 0;
  ultimotpb1 = 0;
  ultimopa1 = 0;
  ultimopb1 = 0;
  // se verifica el estado 
  est_enc1 = obt_est1(pA,pB);
  // 
  esp_pro_est1(pA, pB, est_enc1);
}

void ENCODERA1() {
  // obtengo el delta de tiempo entre flancos
  ultimopa1 = ultimotpa1 - micros();
  ultimotpa1 = micros();

  pro_est_A1(pinA1,pinB1,est_enc1);
  // aumento el contador de pasos
  ca1++;
}

void ENCODERB1() {
  // obtengo el delta de tiempo entre flancos
  ultimopb1 = ultimotpb1 - micros();
  ultimotpb1 = micros();

  pro_est_B1(pinA1,pinB1,est_enc1);
  // aumento el contador de pasos
  cb1
  ++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est1(int pA,int pB){
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

void esp_pro_est1(int pA,int pB,int est_enc){
  switch(est_enc1){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA1, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB1, FALLING);
      break;
  }
}

int pro_est_A1(int pA,int pB,int estado){
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
  est_enc1 = x;
  esp_pro_est1(pA,pB,x);
}

void pro_est_B1(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tot1--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tot1++;
      break;
  }
  est_enc1 = x;
  esp_pro_est1(pA,pB,x);
}

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ control motores  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void parar1() {
  digitalWrite(pin_dire_moto_1, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_1, 254);
}
void adelante1() {
  digitalWrite(pin_dire_moto_1, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_1, velo_moto_1);
}
void atras1() {
  digitalWrite(pin_dire_moto_1, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_1, velo_moto_1);
}

void mover1g(int grados){
    if(pas_tot1 > grados*pasosgra1){
      atras1();
      while(pas_tot1 > pasosgra1*grados){Serial.println(String(pas_tot1));delay(100);};
      // paramos rapidamente el motor
      adelante1();
      delay(1);
      parar1();
    }else{
      adelante1();
      while(pas_tot1 < pasosgra1*grados){Serial.println(String(pas_tot1));delay(100);};
      // paramos rapidamente el motor
      atras1();
      delay(1);
      parar1();
    }  
  }

  void mantenerq1(){
    //velo_moto_1 = (255-int((float(abs(pas_tot1-q1*pasosgra1))/pasosrev1)*Kp1));
    //if (velo_moto_1 < 0){
    //  velo_moto_1=0;
    //}
    
    if(pas_tot1+10 > q1*pasosgra1){
      atras1();
    }else if(pas_tot1-10 < q1*pasosgra1){
      adelante1();
    }  
  }

void velocidad1(int vel){
  vel = abs(vel);
  if (vel > 255){vel=255;}
  velo_moto_1 = vel;
  velo_moto_1 = vel;
}


