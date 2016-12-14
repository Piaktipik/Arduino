

// ------------- variables encoders ------------- 
int enco1 = 0;
int enco2 = 0;

bool a1 = false,a2 = false;

// ----------------------- variables fisicas ----------------------- 
const byte fan = 53;

// ----- motores ----- 
// motor izquierdo - m2
const byte dm1 = 4;
const byte vm1 = 5;

// motor derecho - m1
const byte dm2 = 7;  
const byte vm2 = 6;

// velocidades
byte vel1 = 80;
byte vel2 = 80;

char dato;
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// ----------------------------------------- configuracion --------------------------------------------- 

void setup() {

  // ------ motores ------
  pinMode(fan,OUTPUT);
  // se declaran como salidas
  pinMode(dm2,OUTPUT);
  pinMode(vm2,OUTPUT);
  pinMode(dm1,OUTPUT); 
  pinMode(vm1,OUTPUT);
  
  // ------------ lo uso para detectar los ecnoders -----------------
  // attachInterrupt(pin, ISR, mode); -> solo en due
  attachInterrupt(22, in_enco1a, CHANGE);
  attachInterrupt(23, in_enco1b, CHANGE);
  attachInterrupt(24, in_enco2a, CHANGE);
  attachInterrupt(25, in_enco2b, CHANGE);

  // ------- inicializo serial ------- 
  Serial.begin(9600);
  pinMode(13,0);
  digitalWrite(fan,0);
  parar();
} // fin setup

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void loop() {
  if(Serial.available() > 0){
    dato = char(Serial.read());
    // dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
    if(dato == 'a'){
      adelante();
    }
     if(dato == 'i'){
      izquierda();
    } 
    if(dato == 'd'){
      derecha();
    }
    if(dato == 'p'){
      parar();

    }
    if(dato == 'h'){
      atras();
    }
    if(dato == 'f'){
      digitalWrite(fan,1);
    }
    if(dato == 'o'){
      digitalWrite(fan,0);
    }
  } 
  
  Serial.print("encoder m1= ");
  Serial.print(enco1);
  Serial.print("\t encoder m2= ");
  Serial.println(enco2);
  
  delay(10);
} // fin loop 

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// subfunciones

// incremento los respectivos contadores de los encoders
void in_enco1a(){
  if(a1){enco1++;a1 = 0;}
  else{a1 = 1;}
}
void in_enco1b(){
  if(a1){enco1--;a1 = 0;}
  else{a1 = 1;}
}
void in_enco2a(){
  if(a2){enco2++;a2 = 0;}
  else{a2 = 1;}
}
void in_enco2b(){
  if(a2){enco2--;a2 = 0;}
  else{a2 = 1;}
}

// ------------------------------------ motores  ------------------------------------ 

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

