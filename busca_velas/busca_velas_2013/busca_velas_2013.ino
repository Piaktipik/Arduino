
// por: Julian Galvez Serna - lucas tobon, juan diego cardenas, omar pino
// ------------------------------------------------librerias-------------------------------------------

// ------------------------------------- motores ------------------------------------- 

// ----------- conexion motores -----------

// motor izquierdo
const int m1d = 4;    // direccion motor izquierdo
const int m1e = 5;    // pwm motor izquierdo
// motor derecho
const int m2d = 7;    // direccion motor derecho
const int m2e = 6;    // pwm motor derecho

// ----------- variables motores -----------

int vel1 = 200;             // velocidad motor 1 - izquierdo
int vel2 = 200;             // velocidad motor 2 - derecho
int i = 0;                // variable usada para recorrer le vector de velocidades
unsigned long tiempo = 0; // lo uso para saber cuanto tiempo ha pasado (en milisegundos)
int tini = 0;             // lo uso para saber en que momento empezo el programa

// ----------- conexion auxiliares -----------

const int aux1 = 3;    // aux 1
const int aux2 = 2;    // aux 2

// ------------------------ distribucion pines infrarrojos de pista ------------------------ 
// nota: la derecha derecha y izquierda son de el robot 

// ----------- conexion sensores infrarrojos pista ----------- 
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
int rinf1 = 700;       // para el sensor 1
int rinf2 = 700;       // para el sensor 2
int rinf3 = 700;       // para el sensor 3
int rinf4 = 700;       // para el sensor 4

int reinf = 1000;      // para referencia de error

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

int dminf = 15;   // distancia minima frontal
int dminl = 25;   // distancia minima lateral
int dvela = 15;   // distancia maxima vela

// ------------------------  infrarrojos busca velas ------------------------ 
byte sv[10] = {0,0,0,0,0,0,0,0,0,70};  // sensores de vela
//byte svs = 50;                        // valor de sobrepaso

// ----------- indicadores de estado de los infrarrojos -----------*

const byte an[10] = {38,36,34,39,37,32,35,33,31,29};  // puertos indicadores sensores de vela

 // ------------------------------------------------------------ configuraciones iniciales -------------------------------------------------------
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
  
  // ----------- infrarrojos de vela -----------*
  
  for (i=0;i<10;i++){
    pinMode(an[i],OUTPUT);  // configuro los puertos de los indicadores de los analogos como salidas
  }

  // ----------- ultasonidos - SRF05 ----------- 
  pinMode(eco,INPUT);       // pin eco SRF05
  pinMode(trigger,OUTPUT);  // pin disparo SRF05
 
  // ----------- auxiliares ----------- 
  pinMode(aux1,OUTPUT);    // aux1
  pinMode(aux2,OUTPUT);    // aux2
  digitalWrite(aux1,0);    // inicializo aux1 
  digitalWrite(aux2,0);    // inicializo aux2
  
  Serial. begin(115200);      // solo proposito de depuracion
  
  Serial1. begin(9600);     // serial para sensores ultasonido SRF02
  
  Serial2. begin(57600);    // serial para bus superior
  
  tini = millis();          // guardo el tiempo al que inicia el programa
  
  adelante();          
}


// ----------------------------------------------------------- programa principal -----------------------------------------------------------
   
void loop() {
    //adelante();
    
    
//    sp();
//    antigolpe();
//    bvela();
  
  // --------------------------------------- muestra de los sensores - (solo pruebas) ---------------------------------------
  parar();
  ctime();            // calculo tiempo que va de el programa
  sultraf();          // reviso sensores de ultrasonido
  sultrai();          // reviso sensores de ultrasonido
  sultrad();          // reviso sensores de ultrasonido
  spista();           // pregunto por los sensores de pista
  sinfra();           // pregunto por los sensores infrarrojos 
  Serial.print("time: ");
  Serial.print(tiempo);
  Serial.print('\t');
  Serial.print(vinf1);
  Serial.print('\t');
  Serial.print(vinf2);
  Serial.print('\t');
  Serial.print(vinf3);
  Serial.print('\t');
  Serial.print(vinf4);
  Serial.print("\t dis1: ");
  Serial.print(dist1);
  Serial.print("\t dis2: ");
  Serial.print(dist2);
  Serial.print("\t dis3: ");
  Serial.print(dist3);
  //imprimo las analogas
  for(i=0;i< 11;i++){
    Serial.print('\t');
    Serial.print(sv[i]);
  }
  Serial.println(' ');
  delay(50);
    
}  // fin void principal

// ----------------------------------------------------------- sub - funciones -----------------------------------------------------------

// ---------------------------------- funcion para evadir la salida de la pista --------------------------------

void sp(){
  spista();           // reviso los sensores de pista
  if(sp1 == 1){atras();delay(500);derecha();delay(500);}
  if(sp2 == 1){atras();delay(500);derecha();delay(500);}
  
  if(sp3 == 1){atras();delay(500);izquierda();delay(500);}
  if(sp4 == 1){atras();delay(500);izquierda();delay(500);}
}

// ----------------------------- funcion para evitar que el robot se tropiece ----------------------------------

void antigolpe(){
  // cargo las distancias
  sultraf();
  sultrai(); 
  sultrad();
  // frente 
  if(dist2 < dminf){atras();delay(300);if(dist1 > dist3){izquierda();delay(400);}else{derecha();delay(400);}}
  // izquierdo
  if(dist1 < dminl){derecha();c=0;while((dist1 < dminl)&&(c<50)){sultrai();sp();c+=1;}} // ajustar c
  // derecha     
  if(dist3 < dminl){izquierda();c=0;while((dist3 < dminl)&&(c<50)){sultrad();sp();c+=1;}} // ajustar c
  adelante();
}

// -----------------------------  lo uso para ajustar la trayectoria a la vela ----------------------------------

void ajustar(){
// configurar c
sinfra();

if (sv[6]>sv[10]){derecha();c=0;while((sv[6]<sv[10])&&(c<500)){derecha();sinfra();c+=1;delay(1);}} //derecho

if (sv[7]>sv[10]){derecha();c=0;while((sv[7]<sv[10])&&(c<500)){derecha();sinfra();c+=1;delay(1);}}

if (sv[8]>sv[10]){derecha();c=0;while((sv[8]<sv[10])&&(c<500)){derecha();sinfra();c+=1;delay(1);}}


if (sv[2]>sv[10]){izquierda();c=0;while((sv[2]<sv[10])&&(c<500)){izquierda();sinfra();c+=1;delay(1);}} //izquierdo

if (sv[3]>sv[10]){izquierda();c=0;while((sv[3]<sv[10])&&(c<500)){izquierda();sinfra();c+=1;delay(1);}}

if (sv[4]>sv[10]){izquierda();c=0;while((sv[4]<sv[10])&&(c<500)){izquierda();sinfra();c+=1;delay(1);}}
}

// -------------------------------------  lo uso para ajustar buscar la vela -----------------------------------------

void bvela(){

sinfra();

if (sv[6]>sv[10]){derecha();c=0;while((sv[6]<sv[10])&&(c<500)){sinfra();c+=1;delay(1);}}   // derecho

if (sv[7]>sv[10]){derecha();c=0;while((sv[7]<sv[10])&&(c<500)){sinfra();c+=1;delay(1);}}

if (sv[8]>sv[10]){derecha();c=0;while((sv[8]<sv[10])&&(c<500)){sinfra();c+=1;delay(1);}}

if (sv[9]>sv[10]){derecha();c=0;while((sv[9]<sv[10])&&(c<500)){sinfra();c+=1;delay(1);}}


if (sv[5]>sv[10]){adelante();c=0;while((dist2 > dvela && (sp1==0 && sp2==0 && sp3==0 && sp4==0))&&(c<1000)){sultraf();spista();ajustar();adelante();c+=1;delay(1);}parar();fanon();delay(1000);fanoff();} // frente


if (sv[0]>sv[10]){izquierda();c=0;while((sv[0]<sv[10])&&(c<1000)){sinfra();c+=1;delay(1);}}  // izquierdo

if (sv[1]>sv[10]){izquierda();c=0;while((sv[1]<sv[10])&&(c<1000)){sinfra();c+=1;delay(1);}}

if (sv[2]>sv[10]){izquierda();c=0;while((sv[2]<sv[10])&&(c<1000)){sinfra();c+=1;delay(1);}}

if (sv[3]>sv[10]){izquierda();c=0;while((sv[3]<sv[10])&&(c<1000)){sinfra();c+=1;delay(1);}}

if (sv[4]>sv[10]){izquierda();c=0;while((sv[4]<sv[10])&&(c<1000)){sinfra();c+=1;delay(1);}}

//if (sv[4]>sv[5]){izquierda();c=0;while((sv[4]<sv[5])&&(c<50)){izquierda();sinfra();antigolpe();sp();c+=1;}}
}


// --------------------------------------------------------------------------------------------------------------------------

// ------------------------------------ infrarrojos - vela  ------------------------------------
void sinfra(){
  Serial2.print('L');                  // envio caracter para que el micro envie datos
  c = 0;
  while(Serial2.available() < 11){     // espero que llegen las 11 analogas
    if(c > 100){c=0;break;}else{delay(1);c+=1;}      // si demora mas de 100 seg cancelo
  }
  for (i=0;i<10;i++){
    if(c != 0){
      sv[i] = Serial2.read();
      // muestro indicadores
      
      sv[10] = 70; // valor de offset
      
      if(sv[i]>sv[10]){digitalWrite(an[i],1);
      }else{digitalWrite(an[i],0);}
      // fin if valores mayor ref
    }else{sv[i] = 0;digitalWrite(an[i],0);}
    // fin if datos validos
  } // fin for recorrido de vector
  while(Serial2.available() > 0){Serial2.read();} // limpio puerto
}


// ------------------------------------ ventilador  ------------------------------------
void fanon(){digitalWrite(aux1,1);}

void fanoff(){digitalWrite(aux1,0);}

// ------------------------------------ tiempo  ------------------------------------
void ctime(){
   tiempo = millis() - tini; // calculo el tiempo que ba de el programa
}

// ------------------------------------ motores  ------------------------------------ 

void parar(){
  digitalWrite(m1d, 1);    // motor izquierdo
  analogWrite(m1e, 0);
  digitalWrite(m2d, 0);    // motor derecho
  analogWrite(m2e, 0);
}
void adelante(){
  digitalWrite(m1d, 1);    // motor izquierdo
  analogWrite(m1e, vel1);
  digitalWrite(m2d, 0);    // motor derecho
  analogWrite(m2e, vel2);
}
void atras(){
  digitalWrite(m1d, 0);    // motor izquierdo
  analogWrite(m1e, vel1);
  digitalWrite(m2d, 1);    // motor derecho
  analogWrite(m2e, vel2);
}
void derecha(){
  digitalWrite(m1d, 1);    // motor izquierdo
  analogWrite(m1e, vel1);
  digitalWrite(m2d, 1);    // motor derecho
  analogWrite(m2e, vel2);
}
void izquierda(){
  digitalWrite(m1d, 0);    // motor izquierdo
  analogWrite(m1e, vel1);
  digitalWrite(m2d, 0);    // motor derecho
  analogWrite(m2e, vel2);
}

// ------------------------------------ toma de datos - sensores de pista ------------------------------------ 
void spista(){
  
  digitalWrite(einf,1);     // prendo sensores 
  delayMicroseconds(50);    // espero un tiempo para que los sensores se inicien
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
   if(vinf1 > reinf || vinf2 > reinf || vinf3 > reinf || vinf4 > reinf){digitalWrite(ieinf,HIGH);}else{digitalWrite(ieinf,LOW);}  
}

// ------------------------------------ toma de datos ultrasonidos ------------------------------------ 

void sultrai(){
  // ---------- sensor 1 ---------- 
  // envio la orden al SRF02 de la izquierda para que mande la distancia en cm
  Serial1.write(1);
  Serial1.write(84);
  // espero que llegen los datos durante 80ms de lo contrario pongo distancia maxima
  c = 0;
  while(Serial1.available() < 2){                                      // espero a que llegen los datos
    if(c > 80){c = 0; dist1 = 1000;break;}else{delay(1);c = c + 1;}    // aborto si demora mas de 80ms
  }
  // si c > 0 fue porque llegaron los 2 datos
  if (c > 0) {
    inByte1 = Serial1.read();
    inByte2 = Serial1.read();
    dist1 = inByte1*256 + inByte2;
  }
  if (dist1==0){dist1 = 1000;}
}

void sultraf(){
  // ---------- sensor 2 ----------
  digitalWrite(trigger, LOW);      // mando el pulso de para iniciar medicion
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
}

void sultrad(){
  // ---------- sensor 3 ---------- 
  // envio la orden al SRF02 de la izquierda para que mande la distancia en cm
  Serial1.print(char(0));
  Serial1.write(84);
  // espero que llegen los datos durante 80ms de lo contrario pongo distancia maxima
  c = 0;
  while(Serial1.available() < 2){                                      // espero a que llegen los datos
    if(c > 80){c = 0; dist3 = 1000;break;}else{delay(1);c = c + 1;}    // aborto si demora mas de 80ms
  }
  // si c > 0 fue porque llegaron los 2 datos
  if (c > 0) {
    inByte1 = Serial1.read();
    inByte2 = Serial1.read();
    dist3 = inByte1*256 + inByte2;
  }
  if (dist3==0){dist3 = 1000;}
}



// ------------------------------------ escombros ----------------------------------- 

