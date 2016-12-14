


// ----------- conexion motores ----------- 
// ----- motores ----- 
// motor izquierdo - m2
const byte pin_dire_moto_izqu = 4;
const byte pin_velo_moto_izqu = 5;

// motor derecho - m1
const byte pin_dire_moto_dere = 7;  
const byte pin_velo_moto_dere = 6;

void iniciar_motores(){
  // se declaran como salidas
  pinMode(pin_dire_moto_dere,OUTPUT);
  pinMode(pin_velo_moto_dere,OUTPUT);
  pinMode(pin_dire_moto_izqu,OUTPUT); 
  pinMode(pin_velo_moto_izqu,OUTPUT);
  parar(); // inicializo el robot quieto
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, 0);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, 0);
}
void adelante(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void atras(){
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void izquierda(){
  digitalWrite(pin_dire_moto_izqu, LOW);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, HIGH);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
void derecha(){
  digitalWrite(pin_dire_moto_izqu, HIGH);    // motor izquierdo
  analogWrite(pin_velo_moto_izqu, velo_moto_izqu);
  digitalWrite(pin_dire_moto_dere, LOW);    // motor derecho
  analogWrite(pin_velo_moto_dere, velo_moto_dere);
}
