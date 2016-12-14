// por: Julian Galvez Serna

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

// ------------------------------------------------------ motores y actuadores ------------------------------------------------------
// ----------- conexion motores -----------
// ----- motores -----
// motor izquierdo - m2
const byte pin_dire_moto_izqu = 4;
const byte pin_velo_moto_izqu = 5;

// motor derecho - m1
const byte pin_dire_moto_dere = 7;
const byte pin_velo_moto_dere = 6;

// varialbles que contiene la velocidad de los motores cargadas al llamar las funciones de movimiento
byte velo_moto_izqu = 250; // izquierdo
byte velo_moto_dere = 250; // derecho

bool obstaculo = false;

int pinA = 2;
int pinB = 3;
int ca = 0;
int cb = 0;
int est_enc = 0;
long pas_tot = 0;

int set_point = 0;
int offset = 0;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void setup() {

  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas
  
  activarEncoder(pinA,pinB);
  
  // ----------------- motores -----------------
  // se declaran como salidas
  pinMode(pin_dire_moto_dere, OUTPUT);
  pinMode(pin_velo_moto_dere, OUTPUT);
  pinMode(pin_dire_moto_izqu, OUTPUT);
  pinMode(pin_velo_moto_izqu, OUTPUT);

  parar(); // inicializo el robot quieto

}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void loop() {
  
  //delay(100);
  //int est_enc = obt_est(pinA,pinB);
  
  Serial.print("Ca = " + String(ca));
  Serial.print(" , Cb = " + String(cb));
  Serial.print(" , Est_enc = " + String(est_enc));
  Serial.println(" , pas_tot = " + String(pas_tot));
  
  // recivo comandos por el xbee serial
  if (Serial.available() > 0) {
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
  if(pas_tot > set_point+offset){velocidad((pas_tot-set_point)*40);atras();}
  else if(pas_tot < set_point-offset){velocidad((pas_tot-set_point)*40);adelante();}
  else{parar();
  }
}  // fin loop

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

void datoserial(char dato) {
  if (dato == 'a') {
    adelante();
    Serial.println('a');
  }
  if (dato == 'i') {
    izquierda();
    Serial.println('i');
  }
  if (dato == 'd') {
    derecha();
    Serial.println('d');
  }
  if (dato == 'p') {
    parar();
    Serial.println('p');
  }
  if (dato == 'h') {
    atras();
    Serial.println('h');
  }

  if (dato == 'v') {
    delay(100);
    velo_moto_izqu = Serial.read(); // vel izquierda
    velo_moto_dere = Serial.read(); // vel derecha
  }

  if (dato == 's') {
    delay(100);
    set_point = int(Serial.read()-48)*1000;
    set_point += int(Serial.read()-48)*100;
    set_point += int(Serial.read()-48)*10;
    set_point += int(Serial.read()-48);
  }
  
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar() {
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void adelante() {
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras() {
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void izquierda() {
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha() {
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}

void velocidad(int vel){
  vel = abs(vel);
  if (vel > 255){vel=255;}
  velo_moto_izqu = vel;
  velo_moto_dere = vel;
}

