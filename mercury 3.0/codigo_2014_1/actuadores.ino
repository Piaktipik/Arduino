
// ------------------------------------------------------ motores y actuadores ------------------------------------------------------ 
// ----------- conexion motores ----------- 
// ----- motores ----- 
// motor izquierdo - m2
const byte pin_dire_moto_izqu = 4;
const byte pin_velo_moto_izqu = 5;

// motor derecho - m1
const byte pin_dire_moto_dere = 7;  
const byte pin_velo_moto_dere = 6;

// variables que contiene la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_izqu = 70; // izquierdo
byte velo_moto_dere = 85; // derecho

byte velo_esta = 70;      // velocidad entandar de el desplazamiento
byte valo_ajus_velo = 5;  // valor de ajuste -> se le suma al motor derecho

// ------------- variables encoders ------------- 
int enco1 = 0;
int enco2 = 0;
unsigned long t_enc1 = 0;
unsigned long t_enc2 = 0;

bool a1 = true,a2 = true;

float vel_med1,vel_med2;
float dist1=0,dist2=0;

// ----------- conexion ventilador ----------- 
const byte fanpin = 53;    // pin ventilador

// ----------- conexion buzzer ----------- 
const byte buzpin = 51;    // pin buzzer

void inicio_actuadores(){
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);
  
  parar(); // inicializo el robot quieto
  
  // ------------ lo uso para detectar los encoders -----------------
  // attachInterrupt(pin, ISR, mode); -> solo en due
  attachInterrupt(22, in_enco1a, RISING);
  //attachInterrupt(23, in_enco1b, CHANGE);
  attachInterrupt(24, in_enco2a, RISING);
  //attachInterrupt(25, in_enco2b, CHANGE);
  
  // ----------------- vetilador ----------------- 
  pinMode(fanpin,OUTPUT);
  digitalWrite(fanpin,LOW);    // inicializo ventilador apagado
  
  // ----------------- buzzer ----------------- 
  pinMode(buzpin,OUTPUT);
  digitalWrite(buzpin,LOW);    // inicializo buzzer apagado
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void motores(char movi){
  if (movi != 'p'){
    analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
    analogWrite(pin_velo_moto_dere, velo_moto_dere);
  }
  switch (movi) {
    case 'a':
      digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
      digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
      break;
    case 'p':
      analogWrite(pin_velo_moto_izqu, 0);       // motor izquierdo
      analogWrite(pin_velo_moto_dere, 0);       // motor derecho
      break;
    case 'h':
      digitalWrite(pin_dire_moto_izqu, HIGH);   // motor izquierdo
      digitalWrite(pin_dire_moto_dere, HIGH);   // motor derecho
      break;
    case 'i':
      digitalWrite(pin_dire_moto_izqu, HIGH);   // motor izquierdo
      digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
      break;
    case 'd':
      digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
      digitalWrite(pin_dire_moto_dere, HIGH);   // motor derecho
      break;
    default: 
      analogWrite(pin_velo_moto_izqu, 0);       // motor izquierdo
      analogWrite(pin_velo_moto_dere, 0);       // motor derecho
  }
}


void parar(){
  analogWrite(pin_velo_moto_izqu, 0);
  analogWrite(pin_velo_moto_dere, 0);
}

void adelante(){
  a1=1;a2=1;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}

void atras(){
  a1=0;a2=0;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}

void derecha(){
  a1=1;a2=0;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}

void izquierda(){
  a1=0;a2=1;  // variables donde guardo la direccion de el robot
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}  

// ---------------- ventilador ----------------
void fan(bool v){
  digitalWrite(fanpin,v); 
}

void probar_fan(){
  fan(1);delay(500);fan(0);
}

// ---------------- buzzer ----------------
void buz(bool v){
  digitalWrite(buzpin,v); 
}
