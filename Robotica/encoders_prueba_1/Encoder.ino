float pasprev = 500;
unsigned long ultimotpa = 0;
unsigned long ultimotpb = 0;
unsigned long ultimopa = 0;
unsigned long ultimopb = 0;


void activarEncoder(int pA, int pB){
  // se ponen como entradas los puertos del encoder
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  // se incializan las bases de tiempo
  ultimotpa = 0;
  ultimotpb = 0;
  ultimopa = 0;
  ultimopb = 0;
  // se verifica el estado 
  est_enc = obt_est(pA,pB);
  // 
  esp_pro_est(pA, pB, est_enc);
}

void ENCODERA() {
  // obtengo el delta de tiempo entre flancos
  ultimopa = ultimotpa - micros();
  ultimotpa = micros();

  pro_est_A(pinA,pinB,est_enc);
  // aumento el contador de pasos
  ca++;
}

void ENCODERB() {
  // obtengo el delta de tiempo entre flancos
  ultimopb = ultimotpb - micros();
  ultimotpb = micros();

  pro_est_B(pinA,pinB,est_enc);
  // aumento el contador de pasos
  cb++;
}

// obtenemos el estado inicial donde se encuentra el encoder
int obt_est(int pA,int pB){
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

void esp_pro_est(int pA,int pB,int est_enc){
  switch(est_enc){
    case 1:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB, RISING);
      break;
    case 2:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB, RISING);
      break;
    case 3:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA, FALLING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB, FALLING);
      break;
    case 4:
      attachInterrupt(digitalPinToInterrupt(pA), ENCODERA, RISING);
      attachInterrupt(digitalPinToInterrupt(pB), ENCODERB, FALLING);
      break;
  }
}

int pro_est_A(int pA,int pB,int estado){
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
  est_enc = x;
  esp_pro_est(pA,pB,x);
}

void pro_est_B(int pA,int pB,int estado){
  int x = 0;
  switch(estado){
    case 1:
      x = 4;
      pas_tot--;
      break;
    case 2:
      x = 3;
      break;
    case 3:
      x = 2;
      break;
    case 4:
      x = 1;
      pas_tot++;
      break;
  }
  est_enc = x;
  esp_pro_est(pA,pB,x);
}
