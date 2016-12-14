#ifndef EXBOT_H
#define EXBOT_H
/*

Autor: Sebastian Figueroa Ca馻s

*/


//Librerias Para Incluir


#include "Arduino.h"
#include "utility/IRremote.h"

//Definicion de Clases

//Clase para el Motor
/*
Esta clase permite controlar un motor DC conectado a un puente H
Se usa un pin Analogo y un pin Digital
*/
class MotorDC {
public:
  int State;
  int pA,pD;
  MotorDC(){}; //Constructor
  ~MotorDC(){}; //Destructor
  void Init(int PA,int PD){
  pA=PA;//Almacenar Valor del Pin
  pD=PD;//Almacenar Valor del Pin
  pinMode(pA,OUTPUT);//Definir como Pin de Salida
  pinMode(pD,OUTPUT);//Definir como Pin de Salida
  analogWrite(pA,0); //Valor Inicial
  digitalWrite(pD,LOW);//Valor Inicial
  }
  void Adelante( int Vel){
    analogWrite(pA,Vel);
    digitalWrite(pD,LOW);
    State=0;
  };

  void Atras(int Vel){
  analogWrite(pA,255-Vel);
  digitalWrite(pD,HIGH);
  State=1;
  };

  void ActVel(int Vel){
    if (State==0){
      analogWrite(pA,Vel);
    digitalWrite(pD,LOW);
    State=0;
    }else{
      analogWrite(pA,255-Vel);
  digitalWrite(pD,HIGH);
  State=1;
    }
  }
};
//----------------------------------
extern MotorDC MI,MD;
//----------------------------------

void InitEvitar(int Pin,int Pout);
void EvitarObjetosIR(int Umbral,int VI, int VD);



//Secci贸n IR----------

#define NUMCOMMANDS 10

extern int RECV_PIN;
extern int BUZZ;
extern int CONFIGPIN;
extern unsigned long T1,T2;
extern unsigned long DT;
extern int Estado; //Variable que define el comportamiento del Robot;
extern int EstadoConf; //Estado en el cual se encuentra la configuraci贸n
extern decode_results results; //Objeto para guardar los Resultados
extern unsigned long FunKey[NUMCOMMANDS]; //Se admiten 10 comandos;
extern IRrecv *irrecv;
void IRPrint(void);



extern int PinProximidadIR; //Valor por defecto usado en las pruebas
extern int PoutProximidadIR; //Valor por defecto usado en las pruebas

void IRInit(int RecPin);
unsigned long ReadIRKey(void);
void ResumeIR(void);
void IRReceptorInit(int RecPin, int Buzzer,int PinCon);
void SaveIRValues(void);
void IRControllerAll(void);
int ConfKey(int Number);
void WriteFunKeyEEPROM(void);
void ReadFunKeyEEPROM(void);

//Secci贸n de Linea
void SeguirLinea(int Umbral, int VelI,int VelD);


//Secci贸n 7 Segmentos
void Pabcdefg(int T,int AN);
void Iniciar7Seg(void);
void Mostraren7Seg(int Number,int AN);
int LeerPresencia(void);
void EsperarNoPresencia(int Umbral);
void IniciarSensorPresenciaIR(int Pin, int Pout);

//Secci髇 de Control Serial para tablets
void IniciarSerial(void);
void ControlSerial(void);

#endif