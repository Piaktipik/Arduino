// por: Julian Galvez Serna - lucas tobon, juan diego cardenas, omar pino
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
#include <DueTimer.h>

//#include <EEPROM.h>
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ----------- conexion motores ----------- 
// ----- motores ----- 
// motor izquierdo - m2
const byte dm1 = 4;
const byte vm1 = 5;

// motor derecho - m1
const byte dm2 = 7;  
const byte vm2 = 6;

// velocidades
byte vel1 = 60;
byte vel2 = 60;

// ------------- variables encoders ------------- 
int enco1 = 0;
int enco2 = 0;

bool a1 = true,a2 = true;

// ----------- conexion ventilador ----------- 
const byte fanpin = 53;    // pin ventilador

// ------------------------ variables pines infrarrojos de vela ------------------------ 
byte senvela[20];
byte rvela = dueFlashStorage.read(0);  // leo el valor de ajuste del ruido natural de la vela
// ----------------------------- variables infrarrojos de pista ----------------------------- 
// ----------- conexion sensores infrarrojos pista ----------- 
unsigned int timei = 0;

const byte nsen = 8;
int rlinea = 100*int(dueFlashStorage.read(1)); // leo el valor de ajuste del ruido natural de la vela

int calsen_linea1[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};
int calsen_linea2[] = {rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea,rlinea};

//int calsen_linea1[] = {10000,10000,10000,10000,10000,10000,10000,10000};
//int calsen_linea2[] = {10000,10000,10000,10000,10000,10000,10000,10000};

bool val_sen_lin[15];

// derecho
const byte sen_lineae1 = 32;                            // pin de abilitacion mosfet
const byte sen_linea1[] = {34,36,38,40,42,44,46,48};    // pines digitales del sensor
unsigned long vsen_linea1[nsen];
unsigned int valsenl1[nsen];
// izquierda
const byte sen_lineae2 = 33;                            // pin de abilitacion mosfet
const byte sen_linea2[] = {35,37,39,41,43,45,47,49};    // pines digitales del sensor
unsigned long vsen_linea2[nsen];
unsigned int valsenl2[nsen];


//// ----------- variables infrarrojos ----------- 
//
//int vinf1 = 0;         // se usa para guardar el valor del sensor 1
//int vinf2 = 0;         // se usa para guardar el valor del sensor 2
//int vinf3 = 0;         // se usa para guardar el valor del sensor 3
//int vinf4 = 0;         // se usa para guardar el valor del sensor 4
//
//// ----------- variables infrarrojos booleanas -----------
//
//boolean sp1 = 0;       // los uso para el programa principal - sensor 1
//boolean sp2 = 0;       // los uso para el programa principal - sensor 2  
//boolean sp3 = 0;       // los uso para el programa principal - sensor 3  
//boolean sp4 = 0;       // los uso para el programa principal - sensor 4  
//
//// ----------- variables de referencia ----------- 
//// valor de deteccion de blanco
//int rinf1 = 700;       // para el sensor 1
//int rinf2 = 700;       // para el sensor 2
//int rinf3 = 700;       // para el sensor 3
//int rinf4 = 700;       // para el sensor 4
//
//int reinf = 1000;      // para referencia de error

// -------------------------------------------- ultrasonidos ---------------------------------------- 
// ----------- conexion sensores ultrasonido ----------- 

const byte u1 = 30;   // pin ultrasonido 1
const byte u2 = 28;   // pin ultrasonido 2
const byte u3 = 26;   // pin ultrasonido 3 

// ----------------------------- variables sensores ultrasonido ----------------------------- 
unsigned long d1,d2,d3;                      // valor que almacena el tiempo del falco en us
int di1,di2,di3;                             // valor que contiene la distancia medida
bool mu1 = false,mu2 = false,mu3 = false;    // se usa para las interrupciones
byte cu = 0;                                 // la uso para manejar con una interrupcion los 3 sensores
unsigned int timeu = 0;                      // lo uso para activar los ultrasonidos 

//int dminf = 15;   // distancia minima frontal
//int dminl = 25;   // distancia minima lateral
//int dvela = 15;   // distancia maxima vela

// reactivo
bool evadiendo = false;
bool apagando = false;
bool buscando = false;
int cvel=0;


// timers para la medicion de los sensores


// -------------------------------------------- programa prinicipal ---------------------------------------- 
float x_robot = float(dueFlashStorage.read(2));
float y_robot = float((dueFlashStorage.read(3)+dueFlashStorage.read(4)*255))/100;
float o_robot = float((dueFlashStorage.read(5)+dueFlashStorage.read(6)*255))/100; // posicion y orientacion de el robot

//x_robot = dueFlashStorage.read(2);
//y_robot = float((dueFlashStorage.read(3)+dueFlashStorage.read(4)*255));
//o_robot = float((dueFlashStorage.read(5)+dueFlashStorage.read(6)*255));

const byte xmax=50;
const byte ymax=74;

byte transitable[xmax][ymax];   // valor con las paredes incluidas
byte explorado[xmax][ymax];     // valor con las zonas ya exploradas
byte velas[xmax][ymax];         // valor donde se detecto una vela  

// tiempo de serial
unsigned int timeserial = 0;                      // lo uso para enviar datos por serial

// pruebas
byte tipoc = 4;

// 1 - odometria francis
// 2 - muestreo de sensores
// 3 - prueba codigo sin posicion robot
// 4 - prueba codigo con posicion robot
// 5 - 

// ----------------------------------- variables posicionamiento de el robot -------------------------------- 

float Wd=0,Wi=0,dT=0,dX=0,dY=0;
int Na1=0,N1=0,Na2=0,N2=0;
unsigned int timee = 0;
const float r = 3.5; // radio en cm de las ruedas
float lrobot;
float vellin;

const float pi = 3.14;
float encoder1 = 0; //Numero de vueltas
float encoder1_p = 0; //Numero de vueltas pasadas
float encoder2 = 0; //Numero de vueltas
float encoder2_p = 0; //Numero de vueltas pasadas
float sl = 0; //Velocidades lineales l=izquierda, r=derecha
float sr = 0;
float s = 0; //velocidad lineal del robot
float wl=0; //velocidades angulares
float wr=0;
float w=0; //Velocidad angular promedio del robot
float x;
float y;
float phi;
float tiempo=0;
float tiempo_p=0;
float timer=0;

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {
x_robot = 0;
y_robot = 0;
o_robot = 0;
  // ----------------- timer medicion de sensores ----------------- 
//  Timer3.attachInterrupt(srf05);
//  Timer3.start(50000); // Calls every 30ms

  Timer3.attachInterrupt(localizacion);
  Timer3.start(30000); // Calls every 30ms
  
  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas
  // seriales sensores de vela
  Serial1.begin(57600);       // lado derecho
  Serial2.begin(57600);       // lado izquierda
  
  // ----------------- motores ----------------- 
  // se declaran como salidas
  pinMode(dm2,OUTPUT);
  pinMode(vm2,OUTPUT);
  pinMode(dm1,OUTPUT); 
  pinMode(vm1,OUTPUT);
  
  parar(); // inicializo el robot quieto
  
  // ------------ lo uso para detectar los ecnoders -----------------
  // attachInterrupt(pin, ISR, mode); -> solo en due
  attachInterrupt(22, in_enco1a, CHANGE);
  //attachInterrupt(23, in_enco1b, CHANGE);
  attachInterrupt(24, in_enco2a, CHANGE);
  //attachInterrupt(25, in_enco2b, CHANGE);
  
  // ----------------- vetilador ----------------- 
  pinMode(fanpin,OUTPUT);
  digitalWrite(fanpin,LOW);    // inicializo ventilador apagado
  
  // ----------- infrarrojos de pista ----------- 
  // declaro puertos de control de los led como salida
  pinMode(sen_lineae1,OUTPUT);
  pinMode(sen_lineae2,OUTPUT);
  
}

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
void loop() {  
  if(tipoc==1){
    
  if((timee+20) < millis()){
    timee = millis()-timee;
    Na1 = enco1;
    Na2 = enco2;
    N1=Na1-N1;//numero de pulsos en el periodo
    Wd=((PI/480)*N1)/(timee);//velocidad angular derecha
    N2=Na2-N2;
    Wi=((PI/480)*N2)/(timee);//velocidad angular izquierda
    dT=(Wd-Wi)*r*timee/(20);// cambion de angulo del robot
    dX=(Wd+Wi)*r*timee*(sin (dT))/2;//cambio de x del robot
    dY=(Wd+Wi)*r*timee*(cos (dT))/2;//cambio de Y del robot
    o_robot=o_robot+dT;//angulo del robot
    x_robot=x_robot-dX;//posicion x del robot
    y_robot=y_robot+dY;//posicion y del robot
//    x_robot=x_robot+(dY*sin(o_robot));
//    y_robot=y_robot+(dY*cos(o_robot));
//    vellin=(Wd+Wi)*r/2;
//    dX=dX-vellin * (sin (dT))*timee;
//    dY=dY+vellin * (cos (dT))* timee; 
    N1=Na1;
    N2=Na2;
    timee = millis();
  }
  Serial.print("dtX: ");
  Serial.print(dX);
  Serial.print("\t dtY: ");
  Serial.print(dY);
  Serial.print("\t Wd: ");
  Serial.print(Wd);
  Serial.print("\t Wi: ");
  Serial.print(Wi);
  Serial.print("\t x_robot: ");
  Serial.print(x_robot);
  Serial.print("\t y_robot: ");
  Serial.print(y_robot);
  Serial.print("\t o_robot: ");
  Serial.println(o_robot*180.0/(PI));
//    Serial.print("Na1 ");
//    Serial.println(Na2);
//    delay(20);
  }
  
  
  
  
  // ----------------------------------------------------- programacion reactiva ----------------------------------------------------- 
  
  if(di2 < 20 && apagando){parar();fan(1);delay(1000);fan(0);}
  
  // evasion de obstaculos
  evadiendo = false;
  
  if(di2 < 15) {while(di2 < 20){atras();srf05_2();delay(50);}if(di1 < di3){izquierda();delay(500);}else{derecha();delay(500);} evadiendo = true;}
  if(di1 < 15) {while(di1 < 20){izquierda();srf05_1();delay(50);}evadiendo = true;}
  if(di3 < 15) {while(di3 < 20){derecha();srf05_3();delay(50);}evadiendo = true;}
  
  
  // deteccion de vela
  apagando = false;
  buscando = false;
  
  // analizo sensores de la izquierda
  for(int i=0;i<10;i++){
    if(senvela[i]>20){
      cvel = (80-(7*i)); // lo uso para girar despacion
      izquierda();
      buscando = true;
      vel1=cvel;vel2=cvel;
    }
  }
  
  // analizo sensores e la derecha
  for(int i=11;i<20;i++){
    if(senvela[i]>20){
      cvel = (80-(7*(i-11))); // lo uso para girar despacion
      derecha();
      buscando = true;
      vel1=cvel;vel2=cvel;
    }
  }
  
  if(senvela[9]>20 || senvela[10]>20 || senvela[11]>20){vel1=50;vel2=50;adelante();}
  if((senvela[9]>20 || senvela[10]>20 || senvela[11]>20) && di2 < 30){vel1=50;vel2=50;apagando = true;}
  
  for(int i=0;i<17;i++){
    if(val_sen_lin[i] == 1 && !apagando){
      atras();delay(500);
      if(di1 < di3){izquierda();delay(500);}else{derecha();delay(500);}
    }
  }
  
  no_vela:
  
  if(!buscando && !apagando && !evadiendo){adelante();}
  
  
  // fin programacion reactiva
  
  
  
  
  
  
  // lo uso para manejar el robot
  if(Serial.available() > 0){
    char dato = char(Serial.read());
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
      fan(1); // prendo el ventilador
    }
    if(dato == 'o'){
      fan(0); // apago el ventilador
    }
    
    if(dato == 'v'){
      delay(100);
      vel1 = Serial.read(); // vel izquierda
      vel2 = Serial.read(); // vel derecha
      fan(1);
      delay(200);
      fan(0);
    }
    if(dato == 'c'){
      delay(100);
      dueFlashStorage.write(0,Serial.read());
      dueFlashStorage.write(1,Serial.read());
      rvela = dueFlashStorage.read(0);
      rlinea = 100*int(dueFlashStorage.read(1));
//      Serial.print("rlinea: ");
//      Serial.print(rlinea);
      for(int i=0;i<nsen;i++){
        calsen_linea1[i] = rlinea;
        calsen_linea2[i] = rlinea;
      }
      
      fan(1);
      delay(200);
      fan(0);
    } 
    if(dato == 's'){
      delay(100);
      dueFlashStorage.write(2,Serial.read());
      
      dueFlashStorage.write(3,Serial.read());  // parte baja
      dueFlashStorage.write(4,Serial.read());  // parte alta  
      
      dueFlashStorage.write(5,Serial.read());
      dueFlashStorage.write(6,Serial.read());
      
      x_robot = float(dueFlashStorage.read(2));
      y_robot = float(dueFlashStorage.read(3)+dueFlashStorage.read(4)*255)/100;
      o_robot = float(dueFlashStorage.read(5)+dueFlashStorage.read(6)*255)/100;
      
      fan(1);
      delay(200);
      fan(0);
    }
  }
  
    if((timeu+20) < millis()){
      // prueba de los ultrasonidos
      if(cu==0){srf05_1();}
      if(cu==1){srf05_2();}
      if(cu==2){srf05_3();}
      cu=cu+1;
      if(cu==3){cu=0;}
      timeu = millis();
    }
    //srf05_1(); // derecho
    //srf05_2(); // frontal
    //srf05_3(); // izquierdo
    if((timei+50) < millis()){
      medirlinea();
      svela();
      timei = millis();
    }
    // actualizo el contador


if(tipoc == 2){
  
//delay(20);            // espero que los ultrasonidos estabilicen
  
  Serial.print(di1);
  Serial.print(" ");
  Serial.print(di2);
  Serial.print(" ");
  Serial.print(di3);
  Serial.print(" | ");
  
  // prueba infrarrojos de pista
  for(int i=0;i<nsen;i++){
    Serial.print(valsenl1[i]);
    Serial.print(" ");
  }
  for(int i=0;i<nsen;i++){
    Serial.print(valsenl2[i]);
    Serial.print(" ");
  }
  Serial.print(" | ");
  
  for(int i=0;i<16;i++){
    Serial.print(val_sen_lin[i]);
    Serial.print(" ");
  }
  
  Serial.print(" | ");
  // prueba infrarrojos de vela
 for(int i=0;i<20;i++){
   Serial.print(senvela[i]);
   Serial.print(" ");
 } 
 
 Serial.print(" | ");
 
 Serial.print("em1= ");
  Serial.print(enco1);
  Serial.print("em2= ");
  Serial.print(enco2);
  
//  Serial.print(" | ");
// 
//  Serial.print("rvela= ");
//  Serial.print(rvela);
//  Serial.print("rlinea= ");
//  Serial.print(rlinea);
//  
//  Serial.print(" | ");
// 
//  Serial.print("x_r= ");
//  Serial.print(x_robot);
//  Serial.print("y_r= ");
//  Serial.print(y_robot);
//  Serial.print("O_r= ");
//  Serial.print(o_robot);

 Serial.println();
 
}

 delay(5);              // delay a tener en cuenta para la estabilidad de las medidas

if(tipoc==3){
  
  if((timeserial+50) < millis()){
    
    Serial.write(bytesl(di1));
    Serial.write(bytesh(di1));
    Serial.write(bytesl(di2));
    Serial.write(bytesh(di2));
    Serial.write(bytesl(di3));
    Serial.write(bytesh(di3));
    
    // prueba infrarrojos de pista
    for(int i=0;i<16;i++){
      Serial.write(val_sen_lin[i]);
    }
    
    // prueba infrarrojos de vela
    for(int i=0;i<20;i++){
      Serial.write(senvela[i]);
   } 
   
    Serial.write(bytesl(enco1));
    Serial.write(bytesh(enco1));
    Serial.write(bytesl(enco2));
    Serial.write(bytesh(enco2));

    // actualizo el contador
    Serial.print('F');
    timeserial = millis();
  }
  
}

if(tipoc==4){
  
  if((timeserial+200) < millis()){
    
    Serial.write(bytesl(di1));
    Serial.write(bytesh(di1));
    Serial.write(bytesl(di2));
    Serial.write(bytesh(di2));
    Serial.write(bytesl(di3));
    Serial.write(bytesh(di3));
    
    // prueba infrarrojos de pista
    for(int i=0;i<16;i++){
      Serial.write(val_sen_lin[i]);
    }
    
    // prueba infrarrojos de vela
    for(int i=0;i<20;i++){
      Serial.write(senvela[i]);
   } 
   
    Serial.write(bytesl(enco1));
    Serial.write(bytesh(enco1));
    Serial.write(bytesl(enco2));
    Serial.write(bytesh(enco2));
    
    // mando x,y,o de el robot ()
    //float x_robot,y_robot,o_robot;
    Serial.write(bytesl(x_robot*100));
    Serial.write(bytesh(x_robot*100));
    Serial.write(bytesl(y_robot*100));
    Serial.write(bytesh(y_robot*100));
    Serial.write(bytesl(o_robot*100*(180.0/PI)));
    Serial.write(bytesh(o_robot*100*(180.0/PI)));
    
    // envio matrix de pista
    //const byte xmax=50;
    //const byte ymax=74;
    
//    for(int xi = 0;xi<51;xi++){
//      for(int yi = 0;yi<75;yi++){
//        Serial.write(transitable[xi][yi]);   // envio los valores
//      }
//    }

//byte transitable[xmax][ymax];   // valor con las paredes incluidas
//byte explorado[xmax][ymax];     // valor con las zonas ya exploradas
//byte velas[xmax][ymax];         // valor donde se detecto una vela  
    
    // actualizo el contador
    Serial.print('F');
    timeserial = millis();
    
    
  }
}



if(tipoc==5){ 

Serial.print("\t X ");
Serial.print(x_robot);
  
Serial.print("\t Y ");
Serial.print(y_robot);
  
Serial.print("\t Tetha ");
  Serial.println(o_robot*180/PI);
}



//  // --------------------------------------- muestra de los sensores - (solo pruebas) ---------------------------------------
    
}  // fin loop
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ


void localizacion(){
  
//  tiempo = millis(); //Tomando el tiempo transcurrido en segundos
//  timer = (tiempo - tiempo_p)/1000; //delta de tiempo
//   
//  encoder1 = pi*float(enco1)/480; //Se halla el angulo recorrido por cada rueda en radianes
//  encoder2 = pi*float(enco2)/480; //theta=2*pi/1856 <-----pulsos por vuelta, ver datasheet del encoder
//  
//  wl = (encoder1 - encoder1_p)/(timer); //Se halla la velocidad angular de cada motor en rad/s
//  wr = (encoder2 - encoder2_p)/(timer);
//  w = (wl + wr) / 2; //Se halla la velocidad angular promedio del robot
//  
//  sr=wr*3.5; //Se hallan las velocidades lineales de cada rueda en cm/s
//  sl=wl*3.5; //s = w * r <-----radio de las ruedas
//  s = (sr + sl) / 2; //Se halla la velocidad lineal promedio del robot
//  
//  encoder1_p = encoder1; //generando los valores de vueltas pasadas
//  encoder2_p = encoder2;
//  
//  tiempo_p = tiempo; //pasando el tiempo actual al tiempo pasado
////  
//  x_robot = x_robot + (timer*(sr+sl)/2)*cos(o_robot);
//  y_robot = y_robot + (timer*(sr+sl)/2)*sin(o_robot);
//  o_robot = o_robot + timer*(sr-sl)/20;
//  
// if (abs(o_robot*180.0/PI)>360){
//  o_robot = o_robot-360;
// } 
// 
//  // fin velocidad

}



// conversion de 10bits to 2 bytes
  byte bytesh(int val){
    return val/256;
  }
  
   byte bytesl(int val){
     return val;
  }
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz sub- funciones sensores zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz ultrasonidos zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void srf05(){
//  srf05_1(); // derecho
//  srf05_2(); // frontal
//  srf05_3(); // izquierdo
//  if(cu==0){srf05_1();}
//  if(cu==1){srf05_2();}
//  if(cu==2){srf05_3();}
//  cu=cu+1;
//  if(cu==3){cu=0;}
}


void srf05_1(){
  detachInterrupt(u1);
  d1 = 0;//di1 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u1, OUTPUT);digitalWrite(u1, HIGH);
  delayMicroseconds(10);
  digitalWrite(u1, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u1, INPUT);
  mu1 = false;attachInterrupt(u1, fu1, RISING);mu1 = true;
}
void fu1(){
  if (mu1){
    if (d1 == 0){d1 = micros();attachInterrupt(u1, fu1, FALLING);} 
    else{d1 = micros()-d1; di1 = d1/58.30;} 
  }
}
void srf05_2(){
  detachInterrupt(u2);
  d2 = 0;//di2 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u2, OUTPUT);digitalWrite(u2, HIGH);
  delayMicroseconds(10);
  digitalWrite(u2, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u2, INPUT);
  mu2 = false;attachInterrupt(u2, fu2, RISING);mu2 = true;
}
void fu2(){
  if (mu2){
    if (d2 == 0){d2 = micros();attachInterrupt(u2, fu2, FALLING);}
    else{d2 = micros()-d2; di2 = d2/58.30;}
  }
}
void srf05_3(){  
  detachInterrupt(u3);
  d3 = 0;//di3 = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(u3, OUTPUT);digitalWrite(u3, HIGH);
  delayMicroseconds(10);
  digitalWrite(u3, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(u3, INPUT);
  mu3 = false;attachInterrupt(u3, fu3, RISING);mu3 = true;
}
void fu3(){
  if (mu3){
    if (d3 == 0){d3 = micros();attachInterrupt(u3, fu3, FALLING);}
    else{d3 = micros()-d3; di3 = d3/58.30;}
  }
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

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
    vsen_linea1[i] = micros();//valsenl1[i]=0;
    pinMode(sen_linea2[i],INPUT);
    vsen_linea2[i] = micros();//valsenl2[i]=0;
  }
  // activo las interupciones de los de pista s1
  attachInterrupt(sen_linea1[0],s1p0,FALLING);
  attachInterrupt(sen_linea1[1],s1p1,FALLING);
  attachInterrupt(sen_linea1[2],s1p2,FALLING);
  attachInterrupt(sen_linea1[3],s1p3,FALLING);
  attachInterrupt(sen_linea1[4],s1p4,FALLING);
  attachInterrupt(sen_linea1[5],s1p5,FALLING);
  attachInterrupt(sen_linea1[6],s1p6,FALLING);
  attachInterrupt(sen_linea1[7],s1p7,FALLING);
  // activo las interupciones de los de pista s2
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

void s1p0(){valsenl1[0] = micros()-vsen_linea1[0];if(valsenl1[0] < calsen_linea1[0]){val_sen_lin[0]=HIGH;}else{val_sen_lin[0]=0;}}
void s1p1(){valsenl1[1] = micros()-vsen_linea1[1];if(valsenl1[1] < calsen_linea1[1]){val_sen_lin[1]=HIGH;}else{val_sen_lin[1]=0;}}
void s1p2(){valsenl1[2] = micros()-vsen_linea1[2];if(valsenl1[2] < calsen_linea1[2]){val_sen_lin[2]=HIGH;}else{val_sen_lin[2]=0;}}
void s1p3(){valsenl1[3] = micros()-vsen_linea1[3];if(valsenl1[3] < calsen_linea1[3]){val_sen_lin[3]=HIGH;}else{val_sen_lin[3]=0;}}
void s1p4(){valsenl1[4] = micros()-vsen_linea1[4];if(valsenl1[4] < calsen_linea1[4]){val_sen_lin[4]=HIGH;}else{val_sen_lin[4]=0;}}
void s1p5(){valsenl1[5] = micros()-vsen_linea1[5];if(valsenl1[5] < calsen_linea1[5]){val_sen_lin[5]=HIGH;}else{val_sen_lin[5]=0;}}
void s1p6(){valsenl1[6] = micros()-vsen_linea1[6];if(valsenl1[6] < calsen_linea1[6]){val_sen_lin[6]=HIGH;}else{val_sen_lin[6]=0;}}
void s1p7(){valsenl1[7] = micros()-vsen_linea1[7];if(valsenl1[7] < calsen_linea1[7]){val_sen_lin[7]=HIGH;}else{val_sen_lin[7]=0;}}

void s2p0(){valsenl2[0] = micros()-vsen_linea2[0];if(valsenl2[0] < calsen_linea2[0]){val_sen_lin[8]=HIGH;}else{val_sen_lin[8]=0;}}
void s2p1(){valsenl2[1] = micros()-vsen_linea2[1];if(valsenl2[1] < calsen_linea2[1]){val_sen_lin[9]=HIGH;}else{val_sen_lin[9]=0;}}
void s2p2(){valsenl2[2] = micros()-vsen_linea2[2];if(valsenl2[2] < calsen_linea2[2]){val_sen_lin[10]=HIGH;}else{val_sen_lin[10]=0;}}
void s2p3(){valsenl2[3] = micros()-vsen_linea2[3];if(valsenl2[3] < calsen_linea2[3]){val_sen_lin[11]=HIGH;}else{val_sen_lin[11]=0;}}
void s2p4(){valsenl2[4] = micros()-vsen_linea2[4];if(valsenl2[4] < calsen_linea2[4]){val_sen_lin[12]=HIGH;}else{val_sen_lin[12]=0;}}
void s2p5(){valsenl2[5] = micros()-vsen_linea2[5];if(valsenl2[5] < calsen_linea2[5]){val_sen_lin[13]=HIGH;}else{val_sen_lin[13]=0;}}
void s2p6(){valsenl2[6] = micros()-vsen_linea2[6];if(valsenl2[6] < calsen_linea2[6]){val_sen_lin[14]=HIGH;}else{val_sen_lin[14]=0;}}
void s2p7(){valsenl2[7] = micros()-vsen_linea2[7];if(valsenl2[7] < calsen_linea2[7]){val_sen_lin[15]=HIGH;}else{val_sen_lin[15]=0;}}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de vela zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void svela(){
  Serial1.print("R");
  Serial1.print("L");
}

void serialEvent1(){
  // Serial1.read();
  for(int i=0;i<11;i++){
    senvela[i] = Serial1.read();
  }
 while(Serial1.available()>0){Serial1.read();}  
}

void serialEvent2(){
  for(int i=11;i<20;i++){
    senvela[i] = Serial2.read();
  }
 while(Serial2.available()>0){Serial2.read();}  
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz encoders zzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// incremento los respectivos contadores de los encoders
void in_enco1a(){
  if(a1){enco1++;}
  else{enco1--;}
}

//void in_enco1a(){
//  if(a1){enco1--;a1 = 0;}
//  else{a1 = 1;}
//}
//void in_enco1b(){
//  if(a1){enco1++;a1 = 0;}
//  else{a1 = 1;}
//}

void in_enco2a(){
  if(a2){enco2++;}
  else{enco2--;}
}

//void in_enco2a(){
//  if(a2){enco2--;a2 = 0;}
//  else{a2 = 1;}
//}
//void in_enco2b(){
//  if(a2){enco2++;a2 = 0;}
//  else{a2 = 1;}
//}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar(){
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, 0);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, 0);
}
void adelante(){
  a1=1;a2=1;
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, vel2);
}
void atras(){
  a1=0;a2=0;
  digitalWrite(dm1, HIGH);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, HIGH);    // motor derecho
  analogWrite(vm2, vel2);
}
void derecha(){
  a1=1;a2=0;
  digitalWrite(dm1, LOW);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, HIGH);    // motor derecho
  analogWrite(vm2, vel2);
}
void izquierda(){
  a1=0;a2=1;
  digitalWrite(dm1, HIGH);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, LOW);    // motor derecho
  analogWrite(vm2, vel2);
}  

// ---------------- motores ---------------- 
// 1 - adelante
// 0 - atraz
void motor_d(bool dir,byte vel){
  digitalWrite(dm1, dir);      // pin direccion
  analogWrite(vm1, vel);       // pin velodcidad
}
void motor_i(bool dir,byte vel){
  digitalWrite(dm2, dir);      // pin direccion
  analogWrite(vm2, vel);       // pin velocidad
}
// ---------------- ventilador ----------------
void fan(bool v){
  digitalWrite(fanpin,v); 
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz mapeo zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
// se actualiza el mapa principal usando los datos de los sensores
void mapeo(){
  // añado las paredes y sitios explorados
  //bool obj1=false,obj2=false,obj3=false;
  int xobj = 0;
  int yobj = 0;
  // calculo la posicion de el objeto observado para el ultraosnido 1
  float x_obj1 = x_robot + (di1+10)*cos(o_robot-PI/3);
  float y_obj1 = y_robot + (di1+10)*sin(o_robot-PI/3);
  // reviso el tipo de objeto usando los infrarrojos - 5
  xobj = x_obj1/5;if(xobj < 0 || xobj > (xmax*5)){goto no_obj1;}
  yobj = y_obj1/5;if(yobj < 0 || yobj > (ymax*5)){goto no_obj1;}
  
  if (senvela[5] > rvela){
    if(velas[xobj][yobj]< 250){velas[xobj][yobj] += 50;}   // pendiente de añadir la apertura de los ultrasonidos e infrarrojos
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}
  }
  else{
    if(velas[xobj][yobj] > 0){velas[xobj][yobj] -= 50;}       // quito la vela
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}   // pongo pared
  }
  
  no_obj1: // salto aqui si punto de interes temrmina por fuera de el mapa
  
  // calculo la posicion de el objeto observado para el ultraosnido 2
  float x_obj2 = x_robot + (di2+10)*cos(o_robot-PI/3);
  float y_obj2 = y_robot + (di2+10)*sin(o_robot-PI/3);
  // reviso el tipo de objeto usando los infrarrojos - 10
  xobj = x_obj2/5;if(xobj < 0 || xobj > xmax*5){goto no_obj2;}
  yobj = y_obj2/5;if(yobj < 0 || yobj > ymax*5){goto no_obj2;}
   
  if (senvela[10] > rvela){
    if(velas[xobj][yobj]< 250){velas[xobj][yobj] += 50;}   // pendiente de añadir la apertura de los ultrasonidos e infrarrojos
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}
  }
  else{
    if(velas[xobj][yobj] > 0){velas[xobj][yobj] -= 50;}       // quito la vela
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}   // pongo pared
  }
  
  no_obj2: // salto aqui si punto de interes temrmina por fuera de el mapa
  
  // calculo la posicion de el objeto observado para el ultraosnido 3
  float x_obj3 = x_robot + (di3+10)*cos(o_robot-PI/3);
  float y_obj3 = y_robot + (di3+10)*sin(o_robot-PI/3);
  // reviso el tipo de objeto usando los infrarrojos - 15
  xobj = x_obj3/5;if(xobj < 0 || xobj > xmax*5){goto no_obj3;}
  yobj = y_obj3/5;if(yobj < 0 || yobj > ymax*5){goto no_obj3;}
   
  if (senvela[15] > rvela){
    if(velas[xobj][yobj]< 250){velas[xobj][yobj] += 50;}   // pendiente de añadir la apertura de los ultrasonidos e infrarrojos
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}
  }
  else{
    if(velas[xobj][yobj] > 0){velas[xobj][yobj] -= 50;}       // quito la vela
    if(transitable[xobj][yobj] < 250){transitable[xobj][yobj] += 50;}   // pongo pared
  }
  
  no_obj3: // salto aqui si punto de interes temrmina por fuera de el mapa
  
  ;
  
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz busca_velas zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void busca_velas(){
  
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
// ---------------- ventilador ---------------- 
//void fan(bool val){digitalWrite(fanpin,val);}
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz generar_trayectoria zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void generar_trayectoria(){
  
}
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz seguir_trayectoria zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void seguir_trayectoria(){
  
}
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz calcular_posicion zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
//float calcular_posicion(){
//  float x = 0;
//  return x;
//}
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz comentarios zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

// ------------------------------------ escombros ----------------------------------- 
