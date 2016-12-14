// EN ESTE CODIGO DE IMPLEMENTA EL ARGORITMO A*

// tamaño de la pista
int x = 13;
int y = 18;
int t = 234;
// matriz de la pista 
byte pista[13][18];  // pista actual

// ------------- eeprom -------------  
#include <EEPROM.h>

// ------------- variables encoders ------------- 
unsigned int enco1 = 0;
unsigned int enco2 = 0;

// ----------------------- inicializo el mapa ----------------------- 
// vector de posibles ubicaciones de la pelota A
byte ppelotaA[22][2] ={{ 4,13},{ 7,14}, 
                   { 8,13},{10,13},
                   {10,11},{ 8,11},
                   { 6,11},{ 0, 9},
                   { 1, 8},{ 1, 6},
                   { 0, 5},{ 0, 3},
                   { 0, 1},{10, 9},
                   {10, 7},{ 9, 8},
                   { 7, 8},{ 7, 6},
                   { 4, 1},{ 3, 2},
                   { 1, 2},{ 1, 0},
                  };
                  
byte pelotaA[] = {255,255};             // ubicacion de la pelota A
byte pelotaB[] = { 11,  0};             // ubicacion de la pelota B

byte fin[] = {12,14};                // ubicacion del final de la pista

// ----------------------- variables algoritmo A* ----------------------- 
// guardo si se puede transitar
boolean tra[13][18];                 // guardo: 0-> si se puede transitar
                                     //         1-> si hay obstaculo
// guardo si ya ha sido explorada                                       
boolean e[13][18];                   // guardo: 0-> no ha sido explorado 
                                     //         1-> ya fue explorado                                    
// guardo la direccion
byte d[13][18];                      // guardo: 0-> derecha
                                     //         2-> izquierda
                                     //         3-> arriba
                                     //         4-> abajo    

// guardo las penalizaciones                                       
int f[13][18];                       // penalizacion total
int h[13][18];                       // penalizacion por distacia heuristica

int fact = 0;                        // lo uso para comparar si el f del nodo a travez del nuevo camino explorado, es mejor que el que el nodo posee

// ---- valores penalizaciones  ---- 
// por continuar derecho
int pd = 10;
// por giro
int pg = 15;
// por ir hacia atraz
int pa = 20;
// para distancia h
int ph = 10;

// variables para recorrer la matriz
int xac = 0;
int yac = 0;

// vector de exploracion (ordenano) 
int expx[234];             // guardo: x-> a explorar
int expy[234];             // guardo: y-> a explorar

// vector de ruta 
int rutx[234];             // guardo: x-> a recorrer
int ruty[234];             // guardo: y-> a recorrer

int auxx,auxy;                      // lo uso para el ordenamiento
int c  = 0;                          // lo uso para saber cuantos valor hay llenos                            
// variables de ubicacion espacial
byte meta[] = {4,13};                // objetivo actual (primer pelota)              
byte robot[] = {10,17,2};            // posicion del robot y direccion - punto de partida - (ubico el robot en la posicion inicial mirando hacia adelante)

// lo uso para saber si ya obtube rruta
boolean llegue = true;

// ----------------------- fin variables algoritmo A* ----------------------- 
// ----------------------- variables fisicas ----------------------- 
// ----- motores ----- 
// motor izquierdo
const byte dm1 = 4;
const byte vm1 = 5;
// motor derecho
const byte dm2 = 7;  
const byte vm2 = 6;
// velocidades
byte vel1 = 155;
byte vel2 = 155;

// ----- sensores de pista -----
const byte sp2i = 0;
const byte sp1i = 1;
const byte sp1d = 2;
const byte sp2d = 3;
int sppar = 400;

// ----- sensores de pelota -----
const byte spe1 = 10;
const byte spe2 = 11;
int spepar1 = 300;
int spepar2 = 300;

// ----- sensores de pelota a frente -----
const byte spef = 9;
int spefpar = 600;

// ----- sensores de pared deposito -----
const byte spa = 8;
int spapar = 600; 

// ----- sevo-motores -----
#include <Servo.h> 
Servo servo1;
Servo servo2;

// ------------ variables defincion de objetivo -----------------
// valro tipo de rampa
byte rampa = 5;             // guardo: 0-> para rampa 1a
                            //         1-> para rampa 1b
                            //         2-> para rampa 2a
                            //         3-> para rampa 2b
// variable de busqueda A   
boolean bpelotaA = true;
byte ca = 0;                // lo uso para saber en que a voy 
// variable de busqueda B
boolean bpelotaB = false;
// variable paso por rampa
//boolean nrampa = true;
// variable usada para detectar rampa en caso de no haber subido luego de haber encontrado pelota A
//boolean aux1 = true;
  
// ------------ variables generales -----------------
  // enteros para los ciclos for
  int i = 0, j = 0;
  
// --------- variable para activar la visualizacion --------- 
boolean vis = true;
  
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// ----------------------------------------- configuracion --------------------------------------------- 

void setup() {
  // lo uso solo una vez para limpiar la eeprom
//  EEPROM.write(0,255);
//  EEPROM.write(1,255);
  // cargo la posicion  de la pelota en memoria
  pelotaA[0] = EEPROM.read(0);
  pelotaA[1] = EEPROM.read(1);
  
  rampa = 2;
  // ------ motores ------
  pinMode(dm2,0);digitalWrite(dm2,0); // adelante
  pinMode(vm2,0);analogWrite(vm2,0);
  pinMode(dm1,0);digitalWrite(dm1,0); // adelante 
  pinMode(vm1,0);analogWrite(vm1,0);
  // ------ servo-motores ------
  servo1.attach(8);servo1.write(0);
  servo2.attach(9);servo2.write(0);

  // ------------ lo uso para detectar los ecnoders -----------------
  attachInterrupt(0, in_enco1,CHANGE); // DERECHA
  attachInterrupt(1, in_enco2,CHANGE); // IZQUIERDA
  
  // inicio la busqueda revisando la posicion de la posible pelota a1
  // objetivo actual (fin)
  meta[0] = ppelotaA[0][0];
  meta[1] = ppelotaA[0][1];
  
  // posicion actual (inicio)
  robot[0] = 10;
  robot[1] = 17;
  robot[2] = 2; 
  
  // ------- inicializo serial (solo pruebas) ------- 
  if(vis){Serial.begin(115200);} // si la visualizacion esta activada conecto el arduino por serial
  // ------- fin me conecto con el programa ------- 
  
  pinMode(13,1);digitalWrite(13,0); // pin 13 como salida
  
  // ---------------------- incializo valores pista ---------------------- 
  // inicializo pista
  for(i=0;i<x;i++){
    for(j=0;j<y;j++){
      pista[i][j] = 0;
    }
  }
  // -------- lleno la pista -------- 
  // ubico el final
  pista[fin[0]][fin[1]] = 7;
  // ubico el robot
  pista[robot[0]][robot[1]] = 4;
  // ubico la posible posicion de la pelota A
  for(i=0;i<22;i++){
    pista[ppelotaA[i][0]][ppelotaA[i][1]] = 2;
  }
  // ubico la posicion de la pelota B
  pista[pelotaB[0]][pelotaB[1]] = 3;
  // ubico paredes iniciales
  for(j=10;j<18;j++){pista[11][j] = 6;}
  for(j=1;j<11;j++){pista[j][15] = 6;}
  for(j=0;j<10;j++){pista[j][17] = 6;}
  pista[12][17] = 6;
  // ubico paredes en los puntos criticos de acceso a la rampa
  pista[2][5] = 6;
  pista[4][9] = 6;
  pista[6][9] = 6;
  pista[10][3] = 6;
  // vuelvo los centros de la pista paredes
  for(i=1;i<13;i=i+2){
    for(j=1;j<15;j=j+2){pista[i][j] = 6;}
  }
  // ubico espacio destinado siempre a la rampa incial
  for(j=3;j<8;j++){pista[j][3] = 6;}//pista[9][3] = 6;
  for(j=5;j<10;j++){pista[j][5] = 6;}//pista[3][5] = 6;
  
  pista[meta[0]][meta[1]] = 1; // coloreo la meta actual

  // defino rampa
  dpista(2);
  
  // --------------- cargo mapa --------------- 
  pista[2][13] = 6;
  pista[2][14] = 6;
  pista[1][14] = 6;
  pista[3][14] = 6;
  pista[5][10] = 6;
  pista[5][8] = 6;
  
  pista[0][11] = 6;
  pista[2][11] = 6;
  pista[4][11] = 6;
  pista[5][12] = 6;
  pista[6][13] = 6;
  pista[7][12] = 6;
  pista[7][10] = 6;
  pista[8][ 9] = 6;
  pista[9][10] = 6;
  pista[9][12] = 6;

  pista[0][ 7] = 6;
  
  pista[2][ 9] = 6;
  pista[3][ 8] = 6;
  pista[3][ 6] = 6;
  pista[9][ 6] = 6;
  pista[8][ 7] = 6;
  pista[10][5] = 6;
  pista[11][6] = 6;
  pista[11][8] = 6;
  
  pista[12][1] = 6;
  pista[12][2] = 6;
  pista[11][2] = 6;
  pista[12][3] = 6;
  pista[12][3] = 6;
  
  adelante(); // voy en todo momento hacia adelante
  // --------------- pruebas --------------- 
  //parar();
//  adelante();
//  subirpinza();
} // fin setup

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void loop() {
  digitalWrite(13,1);
  // -------------------------------- limpio mapa -------------------------------- 
    for(i=0;i<x;i++){
      for(j=0;j<y;j++){
        if (pista[i][j] == 1){
          pista[i][j] = 0;            // limpio la trayectoria recorrida
        }                
      }
    }
    
  // -------------------------------- Algoritmo A* -------------------------------- 
    // ---- inicializo matrices usadas en los calculos ---- 
    // lleno matriz de recorridos
    for(i=0;i<x;i++){
      for(j=0;j<y;j++){
        if (pista[i][j] < 6){
          tra[i][j] = false;            // si no hay obtaculo pongo 0
        } 
        else{
          tra[i][j] = true;            // si hay obtaculo pongo 1
        }                 
      }
    } // fin llenado matriz de recorridos
    
    // inicializo matrices
    for(i=0;i<x;i++){
      for(j=0;j<y;j++){
        f[i][j] = 32000;
        e[i][j] = false;
        d[i][j] = 4;
      }
    }
    
    // inicializo vector de exploracion
    for(i=0;i<t;i++){
      expx[0]= 255;
      expy[0]= 255;
    } // fin inicializo vector de exploracion
    c = 0;  // exp esta vacio
    // variable de estado de busqueda
    llegue = false;  // empezamos sin saber si ya llegamos
    
    // pregunto si ya se esta en la posicion deseada
    if (robot[0] == meta[0] && robot[1] == meta[1]){llegue = true;} // llegue
    // si no nos encontramos en la pocicion deseada inicializamos parametros de busqueda
    else{
      
      //calculo penalizacion h
      for(i=0;i<x;i++){
        for(j=0;j<y;j++){
          h[i][j] = (abs(meta[0]-i) + abs(meta[1]-j))*ph; // calculo penalizacion h
        }
      } // fin llenado h
      
      // --------------- parametros iniciales A* --------------- 
      // empiezo a trabajar con la posicion y direccion actual del robot
      xac = robot[0];
      yac = robot[1];
      d[xac][yac] = robot[2];    // uso la direccion actual del robot 
      // guardo en vector exploracion la pocision actual del robot (para empezar a esplorar desde ese punto)
      expx[0] = xac;
      expy[0] = yac;
      c = 1;             // c = 1 -> para indicar que hay un valor
      // guardo penalizacion
      f[xac][yac] = h[xac][yac]; // hago penalizacion total igual a h, ya que no se ha recorrido camino desde posicion actual
    } // fin else origen = destino -> no se ha llegado -> conf inciales
    // -------------------------------------------------- busqueda A* -------------------------------------------------- 
    // busco mejor trayectoria mientras que alla donde buscar
    while(c>0){
      // trabajo con el valor guardado en exp[0] -> el valor de menor peso
      xac = expx[0];
      yac = expy[0];
      // pregunto si el nodo tomado es el nodo final
      if (xac == meta[0] && yac == meta[1]){llegue = true;break;} // llegue y salgo del while de busqueda
      // si no he llegado -> sigo buscando
      else{
        // --------------- procedo a explorar nodos adyacentes a (xac,yac)--------------- 
        // reviso borde izquierdo
        if((xac-1)>=0){
          // pregunto si el nodo puede agregarse -> no ha sido explorada y se puede transitar?
          if(e[xac-1][yac] == false && tra[xac-1][yac] == false){
            // ---------- calculo f para este nodo ---------- 
            // f(n) = g(n) + h(n)
            // calculo g = g(nodo actual) + penalizacion direccion
            fact = f[xac][yac]-h[xac][yac];       // hago la penalizacion igual al camino hasta el nodo anterior
            // penalizacion direccion
            fact += pdir(d[xac][yac],(byte)1);    // penalizo direccion izquierda
            // le agrego h(n)
            fact += h[xac-1][yac];          // penalizo la h del nodo a agregar
            // pregunto si la funcion calculada es menor a la que ya posee el nodo izquierdo
            if (fact < f[xac-1][yac]){
              // pongo el nuevo nodo a explorar en el ultimo valor de exp 
              expx[c] = xac-1;
              expy[c] = yac;
              c++;  // incremento c para que se agrege el siguiente nodo
              // ---------- guardo los nuevos datos del nodo ---------- 
              // guardo la penalizacion mejor
              f[xac-1][yac] = fact;
              // guardo la direccion de llega mas eficiente
              d[xac-1][yac] = 1;            // direccion izquierda
            } // fin agregar nodo con mejor f 
            
          } // fin nodo valido
        } // fin borde izquierdo valido
        
        // reviso borde derecho
        if((xac+1)<x){
          // pregunto si el nodo puede agregarse -> no ha sido explorada y se puede transitar?
          if(e[xac+1][yac] == false && tra[xac+1][yac] == false){
            // ---------- calculo f para este nodo ---------- 
            // f(n) = g(n) + h(n)
            // calculo g = g(nodo actual) + penalizacion direccion
            fact = f[xac][yac]-h[xac][yac];       // hago la penalizacion igual al camino hasta el nodo anterior
            // penalizacion direccion
            fact += pdir(d[xac][yac],(byte)0);    // penalizo direccion derecha
            // le agrego h(n)
            fact += h[xac+1][yac];          // penalizo la h del nodo a agregar
            // pregunto si la funcion calculada es menor a la que ya posee el nodo derecho
            if (fact < f[xac+1][yac]){
              // pongo el nuevo nodo a explorar en el ultimo valor de exp 
              expx[c] = xac+1;
              expy[c] = yac;
              c++;  // incremento c para que se agrege el siguiente nodo
              // ---------- guardo los nuevos datos del nodo ---------- 
              // guardo la penalizacion mejor
              f[xac+1][yac] = fact;
              // guardo la direccion de llega mas eficiente
              d[xac+1][yac] = 0;            // direccion derecha
            } // fin agregar nodo con mejor f 
            
          } // fin nodo valido
        } // fin borde derecho valido
        
        // reviso borde superior
        if((yac-1)>=0){
          // pregunto si el nodo puede agregarse -> no ha sido explorada y se puede transitar?
          if(e[xac][yac-1] == false && tra[xac][yac-1] == false){
            // ---------- calculo f para este nodo ---------- 
            // f(n) = g(n) + h(n)
            // calculo g = g(nodo actual) + penalizacion direccion
            fact = f[xac][yac]-h[xac][yac];             // hago la penalizacion igual al camino hasta el nodo anterior
            // penalizacion direccion
            fact += pdir(d[xac][yac],(byte)2);    // penalizo direccion superior
            // le agrego h(n)
            fact += h[xac][yac-1];          // penalizo la h del nodo a agregar
            // pregunto si la funcion calculada es menor a la que ya posee el nodo superior
            if (fact < f[xac][yac-1]){
              // pongo el nuevo nodo a explorar en el ultimo valor de exp 
              expx[c] = xac;
              expy[c] = yac-1;
              c++;  // incremento c para que se agrege el siguiente nodo
              // ---------- guardo los nuevos datos del nodo ---------- 
              // guardo la penalizacion mejor
              f[xac][yac-1] = fact;
              // guardo la direccion de llega mas eficiente
              d[xac][yac-1] = 2;            // direccion superior
            } // fin agregar nodo con mejor f 
            
          } // fin nodo valido
        } // fin borde superior valido
        
        // reviso borde inferior
        if((yac+1)<y){
          // pregunto si el nodo puede agregarse -> no ha sido explorada y se puede transitar?
          if(e[xac][yac+1] == false && tra[xac][yac+1] == false){
            // ---------- calculo f para este nodo ---------- 
            // f(n) = g(n) + h(n)
            // calculo g = g(nodo actual) + penalizacion direccion
            fact = f[xac][yac]-h[xac][yac];             // hago la penalizacion igual al camino hasta el nodo anterior
            // penalizacion direccion
            fact += pdir(d[xac][yac],(byte)3);    // penalizo direccion inferior
            // le agrego h(n)
            fact += h[xac][yac+1];          // penalizo la h del nodo a agregar
            // pregunto si la funcion calculada es menor a la que ya posee el nodo inferior
            if (fact < f[xac][yac+1]){
              // pongo el nuevo nodo a explorar en el ultimo valor de exp 
              expx[c] = xac;
              expy[c] = yac+1;
              c++;  // incremento c para que se agrege el siguiente nodo
              // ---------- guardo los nuevos datos del nodo ---------- 
              // guardo la penalizacion mejor
              f[xac][yac+1] = fact;
              // guardo la direccion de llega mas eficiente
              d[xac][yac+1] = 3;            // direccion inferior
            } // fin agregar nodo con mejor f 
            
          } // fin nodo valido
        } // fin borde inferior valido
        e[xac][yac] = true;           // ya explore los nodos adyacentes esta posicion
        
        // --------------- organizo expx y expy -> quito primer valor y ordeno de menor a mayor por f --------------- 
        // quito primer valor
        for(i=0;i<c;i++){
          expx[i] = expx[i+1];
          expy[i] = expy[i+1];
        }// quito primer valor
        
        if(c>0){c--;} // le quito a la lista el valor eliminado
        
        // ordeno expx y expy en funcion de sus f de menor a mayor (ordeno los ultimos cuatro valores en c)
        for(i = 0;i<c;i++){
          for(j=i+1;j<c-1;j++){
            if (f[expx[i]][expy[i]] > f[expx[j]][expy[j]]){
              // guardo valor
              auxx = expx[i];
              auxy = expy[i];
              // sobre escribo valor menor
              expx[i] = expx[j];
              expy[i] = expy[j];
              // guardo el valor en la posicion j
              expx[j] = auxx;
              expy[j] = auxy;
            }
          }
        }
  
        } // fin (xac,yac) != destino -> sigo explorando nodos
        
      } // fin while busqueda de trayectoria
      // ya encontre una ruta o no se puedieron encontrar rutas
      
      // si llego construllo la ruta
      if(llegue){
        // obtengo la ruta
        // armo la ruta desde la meta
        xac = meta[0];
        yac = meta[1];
        c = 0;
        while(xac != robot[0] || yac != robot[1]){
          rutx[c] = xac;
          ruty[c] = yac;
          switch(d[xac][yac]) {
            case 0:xac--;break;   
            case 1:xac++;break;
            case 2:yac++;break;
            case 3:yac--;break; 
          }
          c++;
        }
        c--; // pongo a c en la ultima posicion del vector
        for(i=0;i<=c;i++){  
          pista[rutx[i]][ruty[i]] = 1;
        }
      } // fin llego
      // ---------------------------------------------------------------- fin Algoritmo A* ---------------------------------------------------------------- 
      
      
  digitalWrite(13,0);
  
  // empiezo buscando la pelota A
  if(bpelotaA){
    if(pelotaA[0]!= 255 || pelotaA[1]!= 255){
      // ya encontre en una ejecucion anterior A y voy directamente por ella
      meta[0] = pelotaA[0];
      meta[1] = pelotaA[1];
    }else{
      // pregunto si la posicion actual de la pelota A ya fue explorada y si hay camino hasta ella
      if(0 < pista[ppelotaA[ca][0]][ppelotaA[ca][1]] < 3 && llegue){
        meta[0] = ppelotaA[ca][0];
        meta[1] = ppelotaA[ca][1];
      }
      else{    // si no ya fue esplorado o no hay ruta posible paso a la siguiente pelota
        ca++;
        meta[0] = ppelotaA[ca][0];
        meta[1] = ppelotaA[ca][1];
      } // fin busqueda pelota A entre posibles ubicaciones
    } // fin ubicacion de pelota A conosida
  } // fin estoy buscando A
  // fin - empiezo buscando la pelota A
  
  
  // si deteccion de pelotaA
  // añadir sensores de pelotas
  // || analogRead(spe2) > spepar2 )
  
  if((analogRead(spe1) > spepar1) && bpelotaA){
    bpelotaA = false;                                // lla encontre la pelota A
    bpelotaB = true;                                 // empiezo a buscar la pelota B           
//    EEPROM.write(0,robot[0]);                        // guardo la posicion de la pelota A
//    EEPROM.write(1,robot[1]);                           
    for(i=0;i<22;i++){                               // quito las ubicaciones de posibles pelotas si no han sido exploradas
      if(pista[ppelotaA[i][0]][ppelotaA[i][1]] == 2){
        pista[ppelotaA[i][0]][ppelotaA[i][1]] = 0;
      }
    }
  }
  
  // busco la pelotaB
  if(bpelotaB){
      meta[0] = pelotaB[0];
      meta[1] = pelotaB[1];
  } // fin busqueda pelotaB

  // si obtengo la pelotaB voy al final
  // falta añadir sensores de pelota
  // && (robot[0] - 3 < pelotaB[0] && robot[0] + 3 > pelotaB[0]) && (robot[1]-3 < pelotaB[1] && robot[1]+3 > pelotaB[1])
  if(bpelotaB && analogRead(spe1) > spepar1 && analogRead(spe2) > spepar2){
    bpelotaB = false;
    meta[0] = fin[0];      // me dirijo al final de la pista
    meta[1] = fin[1];
  }
  
  // recojo la pelota
  if(analogRead(spef) > spefpar){
    subirpinza();
    delay(500);
    bajarpinza();
  }
  
  // cuando llego al final deposito las bolas
  if(robot[0] == fin[0] && robot[1] == fin[1]){
    int ct = 0;
    // cambio de sentido
    cambiars();
    // agrego lectura del sensor trasero
    while(analogRead(spa) < spapar && ct < 100){atras();ct++;delay(30);}
    parar();
    descargarpelota(); // descargo las pelotas
  }  
  
  // agrego busqueda de linea
  // seguidor de linea simple
  
  // si todos los sensores estan en blanco acelero
  if(analogRead(sp1d) < sppar && analogRead(sp2d) < sppar && analogRead(sp1i) < sppar && analogRead(sp2i) < sppar){
    if(vel1 < 100 && vel2 < 100){vel1 += 5; vel1 += 5;}  
  }
  // si el lado izquierdo entra en negro freno llanta izquierda
  if(analogRead(sp1i) > sppar && analogRead(sp1d) < sppar){
    if(vel1 > 5){vel1 -= 5;}
  }
  
  // si el lado derecho entra en negro freno llanta derecha
  if(analogRead(sp1i) < sppar && analogRead(sp1d) > sppar){
    if(vel2 > 5){vel2 -= 5;}
  }
  
  
  // algoritmo de correcion de posicion del robot
  // si avanzo 15cm paso al siguiente cuadrante
  if (60 < enco1 || 60 < enco2){
    enco1 = 0; enco2 = 0;
    pista[robot[0]][robot[1]] = 0;
    switch(robot[2]) {
      // avanzo a la derecha
      case 0:robot[0]++;break;   
      // avanzo a la izquierda
      case 1:robot[0]--;break;
      // avanzo hacia adelante
      case 2:robot[1]--;break;
      // avanzo hacia atraz
      case 3:robot[1]++;break; 
     }
    // ubico el robot
    pista[robot[0]][robot[1]] = 4; 
  }
  
  // actulizo el vector de recorridos
  if(robot[0] == rutx[c] && robot[1] == ruty[c]){
    c--; //voy al siguiente punto  
  }
  
  //pregunto si llegue a interseccion
  if(analogRead(sp2i) > sppar || analogRead(sp2d) > sppar){
    parar();
    atras();
    delay(200);
    int d2 = d[rutx[c]][ruty[c]];
    // si conserva la direccion
    if(robot[2] == d2){adelante();delay(300);} // si las direcciones son iguales no hago nada sigo siguiendo linea
    // si no son iguales
    else{
      switch(robot[2]) { 
        case 0:
          if(d2==1){cambiars();}
          else{
            if(d2==2){giroizq();}
            else{giroder();}
          }
          enco1 = 0; enco2 = 0; // limpio encoders
        break;     
        case 1:
          if(d2==0){cambiars();}
          else{
            if(d2==2){giroder();}
            else{giroizq();}
          }
          enco1 = 0; enco2 = 0; // limpio encoders  
        break;
        case 2:
          if(d2==3){cambiars();}
          else{
            if(d2==0){giroder();}
            else{giroizq();}
          }
          enco1 = 0; enco2 = 0; // limpio encoders  
        break;
        case 3:
          if(d2==2){cambiars();}
          else{
            if(d2==0){giroizq();}
            else{giroder();}
          }
          enco1 = 0; enco2 = 0; // limpio encoders
       break; 
      } // fin switch
    } // fin direccion diferente
  }
      
  // ---------------------------------------------------------------- imprimo datos ---------------------------------------------------------------- 
  if(vis){
    Serial.print('P');
    // pista
    for(i = 0;i<x;i++){
      for(j = 0;j<y;j++){
        Serial.write(pista[i][j]);
      }
    }
    // direcciones
    for(i = 0;i<x;i++){
      for(j = 0;j<y;j++){
        Serial.write(d[i][j]);
      }
    }
    // transitable?
    for(i = 0;i<x;i++){
      for(j = 0;j<y;j++){
        Serial.write(tra[i][j]);
      }
    }
    // explorado?
    for(i = 0;i<x;i++){
      for(j = 0;j<y;j++){
        Serial.write(e[i][j]);
      }
    }
  }// -------------------------------- fin imprimo datos --------------------------------
  
} // fin loop 

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// subfunciones

// cambio de sentido el robot
void cambiars(){
  derecha();
  derecha();
}

void giroizq(){
  
  int ct = 0;
  vel1=100;vel2=100;
  while(analogRead(sp2i) > sppar && ct < 100){izquierda();ct++;delay(20);}
  ct = 0;
  while(analogRead(sp2i) < sppar && ct < 100){izquierda();ct++;delay(20);}
//  ct = 0;
//  while(analogRead(sp2i) > sppar && ct < 100){izquierda();ct++;delay(20);}
  switch(robot[2]) {
    // avanzo a la derecha
    case 0:robot[2]=2;break;   
    // avanzo a la izquierda
    case 1:robot[2]=3;break;
    // avanzo hacia adelante
    case 2:robot[2]=1;break;
    // avanzo hacia atraz
    case 3:robot[2]=0;break; 
  }
}

void giroder(){
  int ct = 0;
  vel1=100;vel2=100;
  while(analogRead(sp2d) > sppar && ct < 100){derecha();ct++;delay(20);}
  ct = 0;
  while(analogRead(sp2d) < sppar && ct < 100){derecha();ct++;delay(20);}
//  ct = 0;
//  while(analogRead(sp2d) > sppar && ct < 100){derecha();ct++;delay(20);}
  switch(robot[2]) {
    // avanzo a la derecha
    case 0:robot[2]=3;break;   
    // avanzo a la izquierda
    case 1:robot[2]=2;break;
    // avanzo hacia adelante
    case 2:robot[2]=0;break;
    // avanzo hacia atraz
    case 3:robot[2]=1;break; 
  }
}

// incremento los respectivos contadores de los encoders
void in_enco1(){enco1++;}
void in_enco2(){enco2++;}

// ------------------------------------ motores  ------------------------------------ 

void parar(){
  digitalWrite(dm1, 0);    // motor izquierdo
  analogWrite(vm1, 0);
  digitalWrite(dm2, 0);    // motor derecho
  analogWrite(vm2, 0);
}
void adelante(){
  digitalWrite(dm1, 0);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, 0);    // motor derecho
  analogWrite(vm2, vel2);
}
void atras(){
  vel1=100;vel2=100;
  digitalWrite(dm1, 1);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, 1);    // motor derecho
  analogWrite(vm2, vel2);
}
void derecha(){
  digitalWrite(dm1, 0);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, 1);    // motor derecho
  analogWrite(vm2, vel2);
}
void izquierda(){
  digitalWrite(dm1, 1);    // motor izquierdo
  analogWrite(vm1, vel1);
  digitalWrite(dm2, 0);    // motor derecho
  analogWrite(vm2, vel2);
}
// ------------------------------------ servo-motores ------------------------------------ 
void subirpinza(){
  int pos;
  for(pos = 0; pos < 180; pos += 2)  {                                  
    servo1.write(pos);delay(1);
  }
}
void bajarpinza(){
  int pos;
  for(pos = 180; pos > 0; pos -= 2)  {                                  
    servo1.write(pos);delay(1);
  }
}
void descargarpelota(){
  int pos;
  for(pos = 0; pos < 90; pos += 2)  {                                  
    servo2.write(pos);delay(1);
  }
  delay(2000);
  for(pos = 90; pos > 0; pos -= 2)  {                                  
    servo2.write(pos);delay(1);
  }
}

int dpista(byte tipo){
  switch(tipo) { 
        // objentos transitables
        case 0:
          // pared de 9x7 en (3,3)
          for(i=3;i<12;i++){
            for(j=3;j<10;j++){
              pista[i][j] = 6;
            }
          }
          // quito pared de seguridad subida de rampa
          pista[2][5] = 0;
          // hago el camino en la rampa
          for(i=3;i<11;i++){
            pista[i][8] = 5;
          }
          for(i=4;i<11;i++){
            pista[i][6] = 5;
          }
          for(i=4;i<12;i++){
            pista[i][4] = 5;
          }
          pista[10][7] = 5;
          pista[4][5] = 5;
          // quito la esquina de 19
          pista[10][3] = 0;
          pista[10][4] = 0;
          pista[11][4] = 0;
        break;      
        case 1:
          // pared de 9x7 en (3,3)
          for(i=3;i<12;i++){
            for(j=3;j<10;j++){
              pista[i][j] = 6;
            }
          }
          // quito pared de seguridad subida de rampa
          pista[2][5] = 0;      
          // hago el camino en la rampa
          for(i=3;i<11;i++){
            pista[i][8] = 5;
          }
          for(i=4;i<11;i++){
            pista[i][6] = 5;
          }
          for(i=4;i<12;i++){
            pista[i][4] = 5;
          }
          pista[10][7] = 5;
          pista[4][5] = 5;
          // quito la esquina de 19
          pista[3][8] = 0;
          pista[4][8] = 0;
          pista[4][9] = 0;
        break; 
        case 2:
          // pared de 9x6 en (1,0)
          for(i=1;i<10;i++){
            for(j=0;j<6;j++){
              pista[i][j] = 6;
            }
          }
          // quito pared de seguridad subida de rampa
          pista[4][9] = 0;
          pista[6][9] = 0;
          pista[10][3] = 0;
          // hago el camino en la rampa
          for(i=2;i<10;i++){
            pista[i][0] = 5;
          }
          for(i=2;i<9;i++){
            pista[i][2] = 5;
          }
          for(i=1;i<9;i++){
            pista[i][4] = 5;
          }
          pista[2][1] = 5;
          pista[8][3] = 5;
          // quito la esquina de 19
          pista[8][0] = 0;
          pista[9][0] = 0;
        break;  
        case 3:
          // pared de 9x6 en (1,0)
          for(i=1;i<10;i++){
            for(j=0;j<6;j++){
              pista[i][j] = 6;
            }
          }
          // quito pared de seguridad subida de rampa
          pista[4][9] = 0;
          pista[6][9] = 0;
          pista[10][3] = 0;
          // hago el camino en la rampa
          for(i=2;i<10;i++){
            pista[i][0] = 5;
          }
          for(i=2;i<9;i++){
            pista[i][2] = 5;
          }
          for(i=1;i<9;i++){
            pista[i][4] = 5;
          }
          pista[2][1] = 5;
          pista[8][3] = 5;
          // quito la esquina de 19
          pista[1][4] = 0;
          pista[2][4] = 0;
          pista[2][5] = 0;
        break;  
  } // fin switch
}



// funcion para penalizar la direccion
int pdir(byte d1,byte d2){
  // si conserva la direccion
  if(d1 == d2){return pd;}
  // si no son iguales
  else{
    switch(d1) { 
        case 0:
          if(d2==1){return pa;}
          else     {return pg;}      
        case 1:
          if(d2==0){return pa;}
          else     {return pg;}   
        case 2:
          if(d2==3){return pa;}
          else     {return pg;}  
        case 3:
          if(d2==2){return pa;}
          else     {return pg;}  
    } // fin switch
    return pa;
  } // fin direccion diferente
} // fin funcion penalizadora de direccion  


// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz


//  // ----- inclinacion -----
//  pinMode(inc1,1);
//  pinMode(inc2,1);

// identifico la rampa
//  if(robot[0] == 4 && robot[1] == 8 && digitalRead(inc1) == 0){rampa = 0;}
//  if(robot[0] == 6 && robot[1] == 8 && digitalRead(inc2) == 0){rampa = 1;}
//  if(robot[0] == 2 && robot[1] == 4 && digitalRead(inc1) == 0){rampa = 2;}
//  if(robot[0] == 4 && robot[1] == 4 && digitalRead(inc2) == 0){rampa = 3;}

//    if(nrampa){
//      if(rampa > 4){
//        if(rampa < 3){
//          // pongo como objetivo el centro de la rampa 1
//          meta[0] = 7;
//          meta[1] = 6;
//        }else{
//          // pongo como objetivo el centro de la rampa 2
//          meta[0] = 5;
//          meta[1] = 2;
//        } // fin ir a centro de la rampa
//      } // si ya defini la rampa
//      else{
//        // ojo puede presetar errores aqui!!
//        // voy a buscar las pistas reviso el primer punto
//        if(aux1 && llegue){
//          meta[0] = 6;
//          meta[1] = 8;
//        }
//        // si el primer punto esta bloqueado busco en el segundo
//        else{
//          aux1 = false;
//          meta[0] = 4;
//          meta[1] = 4;
//        }    
//      }
//    } 
    // si ya subi la rampa busco la pelotaB
//    else{

  
  // digo que ya subi la rampa
  //if(rampa < 3 && robot[0] == 7 && robot[1] == 6 || 2 < rampa < 5 && robot[0] == 5 && robot[1] == 2){nrampa = false;} // lla estoy sobre la rampa
  


//agrego paredes usando datos de los ultrasonidos
  
  
  // calculo ubicacion usando encoders y infrarrojos
  
  
  // reinicio la posicion del robot sin reiniciar el mapa
//  if(digitalRead(rei) == 0){
//    // posicion actual (inicio)
//    ca = 0;
//    robot[0] = 10;
//    robot[1] = 17;
//    robot[2] = 2;
//    nrampa = true;
//    bpelotaA = true;
//    bpelotaB = false;
//    delay(5000);
//  }     
