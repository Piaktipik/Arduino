const byte nsen = 8;

// derecho
const byte sen_lineae1 = 32;
const byte sen_linea1[] = {34,36,38,40,42,44,46,48};
unsigned long vsen_linea1[8];
unsigned int valsenl1[nsen];
// izquierda
const byte sen_lineae2 = 33;
const byte sen_linea2[] = {35,37,39,41,43,45,47,49};
unsigned long vsen_linea2[8];
unsigned int valsenl2[nsen];

void setup() {
  // declaro puertos de control de los led como salida
  pinMode(sen_lineae1,OUTPUT);
  pinMode(sen_lineae2,OUTPUT);
  
  pinMode(13, OUTPUT);
  
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  Serial.begin(115200);
  
  // apago ventilador
  pinMode(53,OUTPUT);
  digitalWrite(53,0);
  
}

void loop() {
  medirlinea();
  
  delay(17);
  
  for(int i=0;i<nsen;i++){
    if(valsenl1[i]!=0)Serial.print(valsenl1[i]);
    Serial.print("\t");
  }
  for(int i=0;i<nsen;i++){
    if(valsenl2[i]!=0)Serial.print(valsenl2[i]);
    Serial.print("\t");
  }
  Serial.println();
}

void medirlinea(){
  // enciendo leds
  digitalWrite(sen_lineae1,1);
  digitalWrite(sen_lineae2,1);
  // cargo los condesandores poniendo en alto los pines
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],OUTPUT);digitalWrite(sen_linea1[i],HIGH);
    pinMode(sen_linea2[i],OUTPUT);digitalWrite(sen_linea2[i],HIGH);
  }
  // espero 10us a que cargue el capacitor
  delayMicroseconds(10);
  // pongo los pines como entrada y espero a la interrupcion
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],INPUT);
    vsen_linea1[i] = micros();valsenl1[i]=0;
    pinMode(sen_linea2[i],INPUT);
    vsen_linea2[i] = micros();valsenl2[i]=0;
  }
  // activo las interupciones de los de pista
  attachInterrupt(sen_linea1[0],s1p0,FALLING);
  attachInterrupt(sen_linea1[1],s1p1,FALLING);
  attachInterrupt(sen_linea1[2],s1p2,FALLING);
  attachInterrupt(sen_linea1[3],s1p3,FALLING);
  attachInterrupt(sen_linea1[4],s1p4,FALLING);
  attachInterrupt(sen_linea1[5],s1p5,FALLING);
  attachInterrupt(sen_linea1[6],s1p6,FALLING);
  attachInterrupt(sen_linea1[7],s1p7,FALLING);
  
  attachInterrupt(sen_linea2[0],s2p0,FALLING);
  attachInterrupt(sen_linea2[1],s2p1,FALLING);
  attachInterrupt(sen_linea2[2],s2p2,FALLING);
  attachInterrupt(sen_linea2[3],s2p3,FALLING);
  attachInterrupt(sen_linea2[4],s2p4,FALLING);
  attachInterrupt(sen_linea2[5],s2p5,FALLING);
  attachInterrupt(sen_linea2[6],s2p6,FALLING);
  attachInterrupt(sen_linea2[7],s2p7,FALLING);
  // apago leds
  digitalWrite(sen_lineae1,0);
  digitalWrite(sen_lineae2,0);
}

void s1p0(){valsenl1[0] = micros()-vsen_linea1[0];}
void s1p1(){valsenl1[1] = micros()-vsen_linea1[1];}
void s1p2(){valsenl1[2] = micros()-vsen_linea1[2];}
void s1p3(){valsenl1[3] = micros()-vsen_linea1[3];}
void s1p4(){valsenl1[4] = micros()-vsen_linea1[4];}
void s1p5(){valsenl1[5] = micros()-vsen_linea1[5];}
void s1p6(){valsenl1[6] = micros()-vsen_linea1[6];}
void s1p7(){valsenl1[7] = micros()-vsen_linea1[7];}

void s2p0(){valsenl2[0] = micros()-vsen_linea2[0];}
void s2p1(){valsenl2[1] = micros()-vsen_linea2[1];}
void s2p2(){valsenl2[2] = micros()-vsen_linea2[2];}
void s2p3(){valsenl2[3] = micros()-vsen_linea2[3];}
void s2p4(){valsenl2[4] = micros()-vsen_linea2[4];}
void s2p5(){valsenl2[5] = micros()-vsen_linea2[5];}
void s2p6(){valsenl2[6] = micros()-vsen_linea2[6];}
void s2p7(){valsenl2[7] = micros()-vsen_linea2[7];}




