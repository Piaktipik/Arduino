#include<Exbot.h>
int VelI=150,VelD=150; //No esta permitido usar VI y VD

void setup(){

  MI.Init(5,4);//Inicializacion de Motor Izquierdo
  MD.Init(6,7);//Inicializacion de Motor Derecho

}

void loop(){

  //Secuencia Sencilla
  MI.Adelante(VelI);
  MD.Adelante(VelI);
  delay(1000);
  MI.Atras(VelI);
  MD.Atras(VelI);
  delay(1000);
  MI.ActVel(VelI);
  MD.ActVel(VelI);
  
}

