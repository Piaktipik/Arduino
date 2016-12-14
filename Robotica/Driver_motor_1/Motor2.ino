
//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ encoders  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ----- encoder 2 -----
int ca2 = 0;  int cb2 = 0;
int pinA2 = 44;
int pinB2 = 46;
int est_enc2 = 0;
long pas_tot2 = 0;

float pasprev2 = 500;
unsigned long ultimotpa2 = 0;
unsigned long ultimotpb2 = 0;
unsigned long ultimopa2 = 0;
unsigned long ultimopb2 = 0;


void activarEncoder2(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  ultimotpa2 = 0;
  ultimotpb2 = 0;
  ultimopa2 = 0;
  ultimopb2 = 0;
  // se verifica el estado 
  est_enc2 = obt_est2(pA,pB);
  // 
  esp_pro_est2(pA, pB, est_enc2);
}

void ENCODERA2() {
  // obtengo el delta de tiempo entre flancos
  ultimopa2 = ultimotpa2 - micros();
  ultimotpa2 = micros();

  pro_est_A2(pinA2,pinB2,est_enc2);
  // aumento el contador de pasos
  ca2++;
}

void ENCODERB2() {
  // obtengo el delta de tiempo entre flancos
  ultimopb2 = ultimotpb2 - micros();
  ultimotpb2 = micros();

  pro_est_B2(pinA2,pinB2,est_enc2);
  // aumento el contador de pasos
  cb2++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est2(int pA,int pB){
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

void esp_pro_est2(int pA,int pB,int est_enc){
  switch(est_enc2){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA2, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB2, FALLING);
      break;
  }
}

int pro_est_A2(int pA,int pB,int estado){
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
  est_enc2 = x;
  esp_pro_est2(pA,pB,x);
}

void pro_est_B2(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tot2--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tot2++;
      break;
  }
  est_enc2 = x;
  esp_pro_est2(pA,pB,x);
}

//  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ control motores  ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void parar2() {
  digitalWrite(pin_dire_moto_2, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_2, 254);
}
void adelante2() {
  digitalWrite(pin_dire_moto_2, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_2, velo_moto_2);
}
void atras2() {
  digitalWrite(pin_dire_moto_2, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_2, velo_moto_2);
}


void mover2g(int grados){
    //Serial.println("" + grados);
    if(pas_tot2 > grados*pasosgra1){
      atras2();
      while(pas_tot2 > pasosgra1*grados){Serial.println(String(pas_tot2));delay(100);};
      // paramos rapidamente el motor
      adelante2();
      delay(1);
      parar2();
    }else{
      adelante2();
      while(pas_tot2 < pasosgra1*grados){Serial.println(String(pas_tot2));delay(100);};
      // paramos rapidamente el motor
      atras2();
      delay(1);
      parar2();
    }  
  }

 void mantenerq2(){
    if(pas_tot2+10 > q2*pasosgra2){
      atras2();
    }else if(pas_tot2-10 < q2*pasosgra2){
      adelante2();
    }  
  }
  
void velocidad2(int vel){
  vel = abs(vel);
  if (vel > 255){vel=255;}
  velo_moto_2 = vel;
  velo_moto_2 = vel;
}


