#include <Exbot.h>
//Parte declarativa

unsigned long value;
void setup(){
  IRInit(8);//Iniciar Sensor Infrarrojo
}

void loop(){
  ControlSerial();//Controlar comunicacion Serial (Reemplaza IniciarSerial)
  value=ReadIRKey(); //Leer el Codigo IR
  if(value!=0){ //Si el valor leido es diferente de "0" se muestra
  Serial.println(value,HEX);
  ResumeIR(); //Reinicar el Modulo IR para que reciba mas codigos
}  
}

