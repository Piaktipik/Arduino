
//-----------------Funciones de motores--------------------
void iniciar_motores(){
  pinMode(dm2,OUTPUT);
  pinMode(vm2,OUTPUT);
  pinMode(dm1,OUTPUT); 
  pinMode(vm1,OUTPUT);
  parar();
}

void parar(){
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, 0);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, 0);
}
void adelante(){
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, vel2);
}
void atras(){
  digitalWrite(dm1, HIGH);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, HIGH);    // motor derecho
  analogWrite(vm2, vel2);
}
//void derecha(){
//  a1=0;a2=1;
//  digitalWrite(dm1, LOW);    // motor izquierdo
//  analogWrite(vm1, vel1);
//  digitalWrite(dm2, HIGH);    // motor derecho
//  analogWrite(vm2, vel2);
//}
//void izquierda(){
//  a1=1;a2=0;
//  digitalWrite(dm1, HIGH);    // motor izquierdo
//  analogWrite(vm1, vel1);
//  digitalWrite(dm2, LOW);    // motor derecho
//  analogWrite(vm2, vel2);
//}
