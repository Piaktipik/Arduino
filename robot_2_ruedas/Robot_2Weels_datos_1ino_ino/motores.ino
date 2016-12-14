
//-----------------Funciones de motores--------------------
void iniciar_motores(){
  pinMode(dm2,OUTPUT);
  pinMode(vm2,OUTPUT);
  pinMode(dm1,OUTPUT); 
  pinMode(vm1,OUTPUT);
  parar();
}


void motores(float vel_mot_izq, float vel_mot_der){
  bool dir_mi = false;
  bool dir_md = false;
  byte vel_izq = 0;
  byte vel_der = 0;
  
  // escalo los valores a -1 - 1
  if(abs(vel_mot_izq)>1){if(vel_mot_izq < 0){vel_mot_izq =- 1;}else{vel_mot_izq = 1;}}
  if(abs(vel_mot_der)>1){if(vel_mot_der < 0){vel_mot_der =- 1;}else{vel_mot_der = 1;}}
  
  // obtengo la direcion
  if(vel_mot_izq < 0){dir_mi = true;vel_mot_izq = -vel_mot_izq;} // si es negativo cambiamos la direccion hacia atraz
  if(vel_mot_der < 0){dir_md = true;vel_mot_der = -vel_mot_der;} // si es negativo cambiamos la direccion hacia atraz
  
  // escalo la salida
  vel_izq = vel_mot_izq*255;
  vel_der = vel_mot_der*255;
  
  digitalWrite(dm1, dir_mi);    // motor izquierdo 0 -> ADELANTE
  analogWrite(vm1, vel_izq);
  digitalWrite(dm2, dir_md);    // motor derecho
  analogWrite(vm2, vel_der);
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
void derecha(){
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, HIGH);    // motor derecho
  analogWrite(vm2, vel2);
}
void izquierda(){
  digitalWrite(dm1, HIGH);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, vel2);
}
