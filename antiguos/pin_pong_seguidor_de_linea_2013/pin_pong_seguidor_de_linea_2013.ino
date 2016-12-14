
// por: Julian Galvez Serna - lucas tobon, juan diego cardenas, omar pino
// ------------------------------------------------librerias-------------------------------------------
// ------------------------------------- indicadores pista -------------------------------------
boolean direccion = 0;   // 0 direccion a la caja / 1 direccion meta
boolean scruce = 0;      // 0 primera t / 1 segunda t
boolean caja = 0;        // variable para avilitar el muestreo de los ultrasonidos
// ------------------------------------- servo ------------------------------------- 
//#include <Servo.h> 
// 
//Servo myservo;  // create servo object to control a servo 


// ------------------------------------- motores ------------------------------------- 
// ----------- conexion motores -----------
// motor izquierdo
const int m1d = 4;    // direccion motor izquierdo
const int m1e = 5;    // pwm motor izquierdo
// motor derecho
const int m2d = 7;    // direccion motor derecho
const int m2e = 6;    // pwm motor derecho

// ----------- variables motores -----------
int vel1 = 50;           // velocidad motor 1 - izquierdo
int vel2 = 50;           // velocidad motor 2 - derecho
byte vel[18000];          // vector de velocidades (hasta 3 min - cada 10ms) 60 * 3 = 180 
int i = 0;                // variable usada para recorrer le vector de velocidades
unsigned long tiempo = 0; // lo uso para saber cuanto tiempo ha pasado (en milisegundos)
int tini = 0;             // lo uso para saber en que momento empezo el programa
int velo = 0;
// ------------------------------------- auxiliares ------------------------------------- 
// ----------- conexion auxiliares -----------
const int aux1 = 3;    // aux 1
const int aux2 = 2;    // aux 2

// ------------------------ distribucion pines infrarrojos de pista ------------------------ 
// nota: derecha y izquierdas de el robot 

// ----------- conexion sensores infrarrojos ----------- 
const int einf = 53;   // pin de avilitacion de sensores infrarrojos

const int inf1 = 0;    // infrarrojo izquierdo externo
const int inf2 = 1;    // infrarrojo izquierdo interno
const int inf3 = 2;    // infrarrojo derecho externo
const int inf4 = 3;    // infrarrojo derecho interno

// ----------- indicadores de estado de los infrarrojos ----------- 

const int iinf1 = 49;   // indicador infrarrojo izquierdo externo
const int iinf2 = 47;   // indicador infrarrojo izquierdo interno
const int iinf3 = 45;   // indicador infrarrojo derecho externo
const int iinf4 = 43;   // indicador infrarrojo derecho interno

const int ieinf = 51;   // indicador error en infrarrojos

// ----------------------------- variables infrarrojos de pista ----------------------------- 

// ----------- variables infrarrojos ----------- 

int vinf1 = 0;         // se usa para guardar el valor del sensor 1
int vinf2 = 0;         // se usa para guardar el valor del sensor 2
int vinf3 = 0;         // se usa para guardar el valor del sensor 3
int vinf4 = 0;         // se usa para guardar el valor del sensor 4

// ----------- variables infrarrojos booleanas -----------

boolean sp1 = 0;       // los uso para el programa principal - sensor 1
boolean sp2 = 0;       // los uso para el programa principal - sensor 2  
boolean sp3 = 0;       // los uso para el programa principal - sensor 3  
boolean sp4 = 0;       // los uso para el programa principal - sensor 4  

// ----------- variables de referencia ----------- 
// valor de deteccion de blanco
int rinf1 = 670;       // para el sensor 1
int rinf2 = 670;       // para el sensor 2
int rinf3 = 670;       // para el sensor 3
int rinf4 = 670;       // para el sensor 4

int reinf = 1000;      // para referecnia de error

// -------------------------------------------- ultrasonidos ---------------------------------------- 
// ----------- conexion sensores ultrasonido ----------- 

const int eco = 22;       // pin eco SRF05
const int trigger = 23;   // pin disparo SRF05

// ----------------------------- variables sensores ultrasonido ----------------------------- 
int dist1 = 0;    // distancia en cm sensor izquierdo
int dist2 = 0;    // distancia en cm sensor centro
int dist3 = 0;    // distancia en cm sensor derecho

long dur = 0;     // lo uso para el SRF05
int c = 0;        // contador de espera
int inByte1 = 0;  // primer bit SRF02
int inByte2 = 0;  // segundo bit SRF02
 //-------------------------------------configuraciones iniciales-------------------------------------
void setup() {
  // ----------------- motores ----------------- 
  // todos los pines del motor son salidas
  pinMode(m1d,OUTPUT);   
  pinMode(m1e,OUTPUT);    
  pinMode(m2d,OUTPUT);    
  pinMode(m2e,OUTPUT);   
  
  // ----------- infrarrojos de pista ----------- 
  
  pinMode(einf,OUTPUT);    // pin de avilitacion de sensores como salida
  
  pinMode(iinf1,OUTPUT);    // pin idicador de sensor 1 en blanco  
  pinMode(iinf2,OUTPUT);    // pin idicador de sensor 2 en blanco 
  pinMode(iinf3,OUTPUT);    // pin idicador de sensor 3 en blanco 
  pinMode(iinf4,OUTPUT);    // pin idicador de sensor 4 en blanco 
  
  pinMode(ieinf,OUTPUT);    // pin idicador de error en sensores infrarrojos de pista
 
  // ----------- ultasonidos - SRF05 ----------- 
  pinMode(eco,INPUT);       // pin eco SRF05
  pinMode(trigger,OUTPUT);  // pin disparo SRF05
 
  // ----------- auxiliares ----------- 
  pinMode(aux1,OUTPUT);    // aux1
  pinMode(aux2,OUTPUT);    // aux2
  digitalWrite(aux1,0);    // aux1
  digitalWrite(aux2,0);    // aux2
  
  Serial. begin(9600);      // solo proposito de depuracion
  
  Serial1. begin(9600);     // serial para sensores ultasonido SRF02
  //serial2. begin(9600);     // serial para bus superior
  
  // ------------- lleno el vector de velocidades ---------------------
  // vector para cada 10 ms
  for (i=0; i <= 200; i++){
    vel[i] = 80;        
  }
  for (i=200; i <= 1500; i++){
    vel[i] = 80;        
  }
  for (i = 1500; i <= 18000; i++){
    vel[i] = 80;        
  }
  
  velo = 80;
  
  tini = millis();  // guardo el tiempo al que inicia el programa
}


// ----------------------------------------------------------- programa principal -----------------------------------------------------------

void loop() {
  
  spista();           // reviso los sensores de pista
  
  // ------------------------------------------------------------------ controlo velocidades ------------------------------------------------------------------ 
  if (sp1 == 0){
      vel1 -= 50; // le bajo velocidad al motor izquierdo giro a la izquierda
     if(vel1 < 0){vel1 = 0;} 
  }
  
  if (sp2 == 0){
      vel1 -= 1; // le bajo velocidad al motor izquierdo giro a la izquierda
      if(vel1 < 0){vel1 = 0;}
  }
  
  if (sp3 == 0){
      vel2 -= 1; // le bajo velocidad al motor derecho giro a la derecha
      if(vel2 < 0){vel2 = 0;}
  }
  
  if (sp4 == 0){
      vel2 -= 50; // le bajo velocidad al motor derecho giro a la derecha
      if(vel2 < 0){vel2 = 0;}
  }
  
  ctime();  // cargo el tiempo que ba de el programa en tiempo 
  if (vel1 < vel[int(tiempo/10)] && vel2 < vel[int(tiempo/10)] ){
    vel1 += 2;
    vel2 += 2;
    if(vel1 > vel[int(tiempo/10)]){vel1 = vel[int(tiempo/10)];}
    if(vel2 > vel[int(tiempo/10)]){vel2 = vel[int(tiempo/10)];}
  }

  
  // ------------------------------------------------------------------ controlo intercessiones ------------------------------------------------------------------ 
  
  // --------------- primer t de ida ------------------------
  
  if (direccion == 0 && scruce == 0 && sp1 == 0 && sp2 == 0 && sp3 == 0 && sp4 == 0 ){
    vel1 = velo;
    vel2 = velo;
    izquierda();
    delay(500);
    c = 0;
    spista();
    while(sp3){                                                      // si no he recivido almenos 2 valores -> espero (OK)
      if(c > 3000){c = 0;break;}else{spista();delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
    }
    scruce = 1; // pase la primera t
  }
  
  // ---------------- segunda t de ida --------------------------
  
  if (direccion == 0 && scruce == 1 && sp1 == 0 && sp2 == 0){
    vel1 = velo;
    vel2 = velo;
    derecha();
    delay(200);
    c = 0;
    spista();
    while(sp2){                                                      // si no he recivido almenos 2 valores -> espero (OK)
      if(c > 3000){c = 0;break;}else{spista();delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
    }
    caja = 1; // busco la caja en el ultimo tramo de la pista
  }
     
  // --------------- pregunto por ultrasonido -----------------------
  
  if (caja){
    sultra();         // reviso ultrasonido
    if (dist2 < 13 && dist2 > 5){
      parar();        // detengo el robot
      openc();        // abro la compuerta
      delay(1000);
      closec();       // cierro la compuerta
      delay(5000);    // espero que se vacee
      vel1 = velo;
      vel2 = velo;
      atras();
      delay(1500);
      iizquierda();     // giro sobre el eje
      delay(500);
      spista();
      while(sp2){                                                      // gire hasta que vuelva a la pista
        if(c > 5000){c = 0;break;}else{spista();delay(1);c = c + 1;}      
      }
      caja = 0;
      direccion = 1; // ya puedo devolverme
    }
  }
      
  // --------------- primer t de regreso ------------------------
//  if (direccion == 1 && scruce == 1 && sp1 == 0 && sp2 == 0){
//    vel1 = velo;
//    vel2 = velo;
//    izquierda();
//    delay(400);
//    c = 0;
//    spista();
//    while(sp3){                                                      // si no he recivido almenos 2 valores -> espero (OK)
//      if(c > 3000){c = 0;break;}else{spista();delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
//    }
//    scruce = 0; // paso a la segunda t
//  }
//  // --------------- segunda t de regreso -----------------------
//  if (direccion == 1 && scruce == 0 && sp4 == 0 && sp3 == 0 ){
//    vel1 = velo;
//    vel2 = velo;
//    derecha();
//    delay(4000);
//    c = 0;
//    spista();
//    while(sp2){                                                      // si no he recivido almenos 2 valores -> espero (OK)
//      if(c > 3000){c = 0;break;}else{spista();delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
//    }
//  }
  
  // artualizo el movimiento
  adelante();        // actualizo las velocidades
  
// muestra de los sensores - (solo pruebas)
//  Serial.print(vinf1);
//  Serial.print(',');
//  Serial.print(vinf2);
//  Serial.print(',');
//  Serial.print(vinf3);
//  Serial.print(',');
//  Serial.print(vinf4);
//  Serial.print(", distancia1: ");
//  Serial.print(dist1);
//  Serial.print(", distancia2: ");
//  Serial.print(dist2);
//  Serial.print(", distancia3: ");
//  Serial.println(dist3);
  
}

// ----------------------------------------------------------- sub-funciones -----------------------------------------------------------
// ------------------------------------ servo  ------------------------------------
void openc(){
  digitalWrite(aux1,1);
  // myservo.write(90); 
}

void closec(){
  digitalWrite(aux1,0);
  // myservo.write(0); 
}

// ------------------------------------ tiempo  ------------------------------------
void ctime(){
   tiempo = millis() - tini; // calculo el tiempo que ba de el programa
}

// ------------------------------------ motores  ------------------------------------ 

void parar(){
  // motor izquierdo
  digitalWrite(m1d, 1);
  analogWrite(m1e, 0);
  // motor derecho
  digitalWrite(m2d, 0);
  analogWrite(m2e, 0);
}

void adelante(){
  // motor izquierdo
  digitalWrite(m1d, 1);
  analogWrite(m1e, vel1);
  // motor derecho
  digitalWrite(m2d, 0);
  analogWrite(m2e, vel2);
}

void atras(){
  // motor izquierdo
  digitalWrite(m1d, 0);
  analogWrite(m1e, vel1);
  // motor derecho
  digitalWrite(m2d, 1);
  analogWrite(m2e, vel2);
}

void dderecha(){
  // motor izquierdo
  digitalWrite(m1d, 1);
  analogWrite(m1e, vel1);
  // motor derecho
  digitalWrite(m2d, 1);
  analogWrite(m2e, vel2);
}

void iizquierda(){
  // motor izquierdo
  digitalWrite(m1d, 0);
  analogWrite(m1e, vel1);
  // motor derecho
  digitalWrite(m2d, 0);
  analogWrite(m2e, vel2);
}

void derecha(){
  // motor izquierdo
  digitalWrite(m1d, 1);
  analogWrite(m1e, vel1);
  // motor derecho
  digitalWrite(m2d, 1);
  analogWrite(m2e, 0);
}

void izquierda(){
  // motor izquierdo
  digitalWrite(m1d, 0);
  analogWrite(m1e, 0);
  // motor derecho
  digitalWrite(m2d, 0);
  analogWrite(m2e, vel2);
}

// ------------------------------------ toma de datos sensores de pista ------------------------------------ 
void spista(){
  
  digitalWrite(einf,1);     // prendo sensores 
  delayMicroseconds(50);   // espero un tiempo para que los sensores se inicien
  vinf1 = analogRead(inf1); // leo sensor 1
  vinf2 = analogRead(inf2); // leo sensor 2
  vinf3 = analogRead(inf3); // leo sensor 3
  vinf4 = analogRead(inf4); // leo sensor 4
  digitalWrite(einf,0);     // apago sensores
 
   // muestro el indicador del sensor 1
   if(vinf1 < rinf1){digitalWrite(iinf1,HIGH);sp1 = 1;}else{digitalWrite(iinf1,LOW);sp1 = 0;}
   
   // muestro el indicador del sensor 2
   if(vinf2 < rinf2){digitalWrite(iinf2,HIGH);sp2 = 1;}else{digitalWrite(iinf2,LOW);sp2 = 0;}   
   
   // muestro el indicador del sensor 3
   if(vinf3 < rinf3){digitalWrite(iinf3,HIGH);sp3 = 1;}else{digitalWrite(iinf3,LOW);sp3 = 0;}
   
   // muestro el indicador del sensor 4
   if(vinf4 < rinf4){digitalWrite(iinf4,HIGH);sp4 = 1;}else{digitalWrite(iinf4,LOW);sp4 = 0;}
  
   // muestro el indicador de errores
   if( vinf1 > reinf || vinf2 > reinf || vinf3 > reinf || vinf4 > reinf){digitalWrite(ieinf,HIGH);}else{digitalWrite(ieinf,LOW);} 
   
}

// ------------------------------------ toma de datos ultrasonidos ------------------------------------ 
void sultra(){
  // ---------- sensor 1 ---------- 
  // envio la orden al SRF02 de la izquierda para que mande la distancia en cm
//  Serial1.write(1);
//  Serial1.write(84);
//  
//  // espero que llegen los datos durante 100ms de lo contrario pongo distancia maxima
//  c = 0;
//  while(Serial1.available() < 2){                             // si no he recivido almenos 2 valores -> espero (OK)
//    if(c > 100){c = 0; dist1 = 1000;break;}else{delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
//  }
//  
//  // si c > 0 fue porque llegaron los 2 datos
//  if (c > 0) {
//    inByte1 = Serial1.read();
//    inByte2 = Serial1.read();
//    dist1 = inByte1*256 + inByte2;
//  }
  
  // ---------- sensor 2 ----------
  
  // mando el pulso de para iniciar medicion
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  dist2 = 600;
  dur = 0;
  dur = pulseIn(eco, HIGH, 50000); // cuento el tiempo que demora el pulso en alto, el proceso no puede demorar mas de 50ms
  if (dur != 0){
    dist2 = dur/58.30;
  }
  // ---------- sensor 3 ---------- 
  // envio la orden al SRF02 de la izquierda para que mande la distancia en cm
//  Serial1.print(char(0));
//  Serial1.write(84);
//  
//  // espero que llegen los datos durante 100ms de lo contrario pongo distancia maxima
//  c = 0;
//  while(Serial1.available() < 2){                             // si no he recivido almenos 2 valores -> espero (OK)
//    if(c > 100){c = 0; dist3 = 1000;break;}else{delay(1);c = c + 1;}      // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
//  }
//  
//  // si c > 0 fue porque llegaron los 2 datos
//  if (c > 0) {
//    inByte1 = Serial1.read();
//    inByte2 = Serial1.read();
//    dist3 = inByte1*256 + inByte2;
//  }
  
}



// ------------------------------------ escombros ----------------------------------- 

