#include "Exbot.h"
//#include "IRremote.h"
//#include <EEPROM.h>
//#include "src/EEPROM.h"
//Objetos

//#include "IRremote.h"
//#include "IRremoteInt.h"
#include "EEPROM.h"

MotorDC MI,MD;
int VI=210,VD=200;

#define DEBUG

//Secci贸n de Evitar Objetos
int PinProximidadIR=A0; //Valor por defecto usado en las pruebas
int PoutProximidadIR=2; //Valor por defecto usado en las pruebas

void IniciarSensorPresenciaIR(int Pin, int Pout){
PinProximidadIR=Pin;
PoutProximidadIR=Pout;
pinMode(Pin,INPUT);
pinMode(Pout,OUTPUT);
digitalWrite(Pout,LOW);//Apagar Emisor IR
}

void InitEvitar(int Pin,int Pout){
PinProximidadIR=Pin;
PoutProximidadIR=Pout;
pinMode(Pin,INPUT);
pinMode(Pout,OUTPUT);
digitalWrite(Pout,LOW);//Apagar Emisor IR
}
void EvitarObjIR(MotorDC* MotI,MotorDC* MotD,int Pin, int Pout,int Umbral)
{

}

int LeerPresencia(void)
{
int VOFF=0,VON=0; //Valores del Sensor
int Medida=0; //Medida 

  //Cuerpo de la Funci贸n
  VOFF=analogRead(PinProximidadIR); //Leer valor con Emisor Apagado
  digitalWrite(PoutProximidadIR,HIGH);//Encender Emisor IR
  delay(10); //Esperar a que encienda el IR
  VON=analogRead(PinProximidadIR);//Leer valor con Emisor Apagado
  digitalWrite(PoutProximidadIR,LOW);//Apagar Emisor IR
  delay(10);
  Medida=VON-VOFF;
  
  
  //Leer Sensor
  //Imprimir valores
  Serial.print(VOFF);
  Serial.print("\t");
  Serial.print(VON);
  Serial.print("\t");
  Serial.println(Medida);
  
  return Medida;

}

void EsperarNoPresencia(int Umbral)
{	int P=LeerPresencia();
	while(P>Umbral){
	P=LeerPresencia();}
}


void EvitarObjetosIR(int Umbral,int VI,int VD){
//Variables Locales de Funci贸n
int VOFF=0,VON=0; //Valores del Sensor
int Medida=0; //Medida 
//int VI=100,VD=200; //Velocidad cada Motor Ya han sido declaradas
int Dir=0; //Direccion del Motor


//Cuerpo de la Funci贸n
VOFF=analogRead(PinProximidadIR); //Leer valor con Emisor Apagado
  digitalWrite(PoutProximidadIR,HIGH);//Encender Emisor IR
  delay(10); //Esperar a que encienda el IR
  VON=analogRead(PinProximidadIR);//Leer valor con Emisor Apagado
  digitalWrite(PoutProximidadIR,LOW);//Apagar Emisor IR
  delay(10);
  Medida=VON-VOFF;
  
  
  //Leer Sensor
  //Imprimir valores
  Serial.print(VOFF);
  Serial.print("\t");
  Serial.print(VON);
  Serial.print("\t");
  Serial.println(Medida);
  
  //Acci贸n y Decisi贸n
  if(Medida>Umbral)
  {//Hay Obstaculo
    switch(Dir){
    case 0:
    MI.Atras(VI);
    MD.Atras(VD);
    Dir=1;
    break;
    case 1:
    MI.Adelante(VI);
    MD.Atras(VD);
    Dir=2;
    break;
    case 2:
    MI.Atras(VI);
    MD.Adelante(VD);
    Dir=0;
    break;
    }
    delay(1000);
  }else
  {//No Hay Obstaculo
    MI.Adelante(VI);
    MD.Adelante(VD);
  }
 
}

//Secci贸n de Controlador Remoto

int RECV_PIN = 8;
int BUZZ=12;
int CONFIGPIN=11;
unsigned long T1,T2;
unsigned long DT;
int Estado=1; //Variable que define el comportamiento del Robot;
int EstadoConf=0; //Estado en el cual se encuentra la configuraci贸n
decode_results results; //Objeto para guardar los Resultados
unsigned long FunKey[NUMCOMMANDS]; //Se admiten 10 comandos;
IRrecv *irrecv;

void WriteFunKeyEEPROM(void){
   int i=0;
   byte *ptr;
   ptr=(byte *)&FunKey[0];
   for(i=0;i<sizeof(FunKey[0])*NUMCOMMANDS;i++)
   {
     EEPROM.write(i,*ptr);
	 ptr++;
   }
   
   #ifdef DEBUG
   Serial.println("Valores Enviados");
   for(int j=0;j<NUMCOMMANDS;j++){
		Serial.print(j);
		Serial.print("\t");
		Serial.println(FunKey[j],HEX);
   }
   #endif
   
}
void ReadFunKeyEEPROM(void){
	int i=0;
	byte *ptr;
	ptr=(byte *)&FunKey[0];
	for(i=0;i<sizeof(FunKey[0])*NUMCOMMANDS;i++)
   {
     (*ptr)=EEPROM.read(i);
	 ptr++;
   }
   
//   #ifdef DEBUG
   Serial.println("Valores Leidos");
   for(int j=0;j<NUMCOMMANDS;j++){
		Serial.print(j);
		Serial.print("\t");
		Serial.println(FunKey[j],HEX);
   }
  // #endif
   
}
void IRInit(int RecPin){
RECV_PIN=RecPin;
	irrecv=new IRrecv(RECV_PIN);
	irrecv->enableIRIn(); // Iniciar el Receptor
    irrecv->blink13(false); //Deshabilitar pin 13 al Recibir	
}
void IRReceptorInit(int RecPin, int Buzzer,int PinCon){
	BUZZ=Buzzer;
	CONFIGPIN=PinCon;
	pinMode(CONFIGPIN,INPUT);
	pinMode(BUZZ,OUTPUT);
	RECV_PIN=RecPin;
	irrecv=new IRrecv(RECV_PIN);
	irrecv->enableIRIn(); // Iniciar el Receptor
    irrecv->blink13(false); //Deshabilitar pin 13 al Recibir	
	ReadFunKeyEEPROM();
	EstadoConf=0;
	
	//Falta Agregar lectura de los FunKey desde la EEPROM
	#ifdef DEBUG 
	Serial.println("Se ha hecho configuraci贸n Inicial del IR");
	#endif
}
void SaveIRValues(void)
{
if(digitalRead(CONFIGPIN)==HIGH){
    delay(1000);
    Serial.println("HIGH");
    if(ConfKey(EstadoConf)==1){
  	EstadoConf++; 
  	T1=millis();
        Serial.println("Recibido");
    }else{
 	DT=millis()-T1;
  	if(DT>3500){
        EstadoConf=0;
 	WriteFunKeyEEPROM();
  	digitalWrite(BUZZ,HIGH);
  	delay(3000);
  	digitalWrite(BUZZ,LOW);
  			}
  		}
  }else{EstadoConf=0;T1=millis();}
}

//Imprimir Resultados del Receptor IR
void IRPrint(void){
  
  if (results.decode_type == NEC) {
      Serial.print("NEC: ");
    } else if (results.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (results.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (results.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (results.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    }
    Serial.println(results.value, HEX);
}
#define NOVAL 0
#define OK 1
unsigned long ReadIRKey(void){
if(irrecv->decode(&results))
{ 
return results.value;
}else
return 0;
}

unsigned long ReadIRType(void){
if(irrecv->decode(&results))
{ 
return results.value;
}else
return 0;
}

void ResumeIR(void){
	irrecv->resume();
}

int ConfKey(int Number)
{
	//Esta funci贸n mira si hay un valor en el IR y lo almacena en el FunKey
	if (irrecv->decode(&results)){
		#ifdef DEBUG 
		Serial.print("Codigo Recibido:");
		#endif
		#ifdef DEBUG 
		Serial.println(Number);
		#endif
		
		IRPrint();
		FunKey[Number]=results.value;	
		
		for(int i=0;i<Number+1;i++)
		{
			//Pitar
			digitalWrite(BUZZ,HIGH);
			delay(200);
			digitalWrite(BUZZ,LOW);
			delay(100);
			T1=millis();
		}
		irrecv->resume();
		return OK;
	}
	else
		return NOVAL; //
}
//
void IRControllerAll(void){

//Esta Funci贸n detecta el Valor y se mueve de acuerdo a los valores
	if(digitalRead(CONFIGPIN)==HIGH)
	{
		//Interface de Configuraci贸n
		Estado=0; //Estado de Configuraci贸n
		EstadoConf=0;
		digitalWrite(BUZZ,HIGH);
		delay(500);
		digitalWrite(BUZZ,LOW);
		T1=millis();
			
		//Al final se debe Guardar los datos en la EEPROM
	}

	if (irrecv->decode(&results)&&Estado!=0){
	
		IRPrint(); //Imprimir Resultados Controlador
		//Como funKey es variable no se pueden usar caso sino if anidados
		
		if (results.value==FunKey[0]){ //Parar
			MI.Atras(0);
			MD.Adelante(0);
			Estado=1;
		}else if (results.value==FunKey[1]){ //Ir hacia Adelante
			MI.Adelante(VI);
			MD.Adelante(VD);
			Estado=1; //El 1 indica estado de Control Manual;
		}else if (results.value==FunKey[2]){ //Ir hacia atr谩s
			MI.Atras(VI);
			MD.Atras(VD);
			Estado=1;
		}else if (results.value==FunKey[3]){ //Ir a Izquierda
			MI.Atras(VI);
			MD.Adelante(VD);
			Estado=1;
		}else if (results.value==FunKey[4]){ //Ir a Derecha
			MI.Adelante(VI);
			MD.Atras(VD); 
			Estado=1;			
		}else if (results.value==FunKey[5]){//Vel++
			if(VI<=245 && VD<=245)
			{
			VI+=10;
			VD+=10;
			MI.ActVel(VI);
			MD.ActVel(VD);
			}
			Estado=1;
			
		}else if (results.value==FunKey[6]){//Vel--
			if(VI>=10 && VD>=10)
			{
			VI-=10;
			VD-=10;
			MI.ActVel(VI);
			MD.ActVel(VD);
			}
			Estado=1;
			
		}else if (results.value==FunKey[7]){ //Pitar
			digitalWrite(BUZZ,HIGH);
			delay(100);
			digitalWrite(BUZZ,LOW);
		}else if (results.value==FunKey[8]){ //Evitar Objetos
			Estado=2;
			//EvitarObjetosIR(300,150,110) //Implementaci贸n HardCoded debe ser cambiada
		}else if (results.value==FunKey[9]){ //Modificar Umbral
			//No implemntada	
		}
		irrecv->resume();
	}

	switch(Estado){  //Maquina de Estados Mealy
	
		case 0: 
		//Pitar 500 ms para indicar inicio de Configuraci贸n
				Serial.println(Estado);
				if(ConfKey(EstadoConf)==OK)
				{
				  EstadoConf++; 
				  T1=millis();
				}else{
					DT=millis()-T1;
					if(DT>10000){
					WriteFunKeyEEPROM();
					Estado=1;
					digitalWrite(BUZZ,HIGH);
					delay(500);
					digitalWrite(BUZZ,LOW);
					}
				}
				
			
			//SubMaquina de Estados de Configuraci贸n
			//Para salir de este Estado:
			// Debe pasar 3 Segundos sin Acci贸nes
			
		
		break;
		
		case 1:
		//No se ejecuta acci贸n por ser de Control Manual;
		#ifdef DEBUG 
		Serial.println("Se ha entrado al control manual");
		#endif
		break;
		
		case 2:
			EvitarObjetosIR(400,VI,VD);
		break;
	}
	
}

// Secci贸n Seguidor de L铆nea
#define DEBUGL
#define pinIR PoutProximidadIR
int ADSensorI=A1; //No se declara Static para poder ser Modificada
int ADSensorD=A2; //No se declara Static para poder ser Modificada
void SeguirLinea(int Umbral, int VelI,int VelD){
	digitalWrite(pinIR,HIGH);
	VI=VelI;
	VD=VelD;
	int SensorI,SensorD;
	
	//Lectura de Sensores
	SensorI=analogRead(ADSensorI);
	SensorD=analogRead(ADSensorD);
	
	#ifdef DEBUGL
	Serial.print("SensorI:");
	Serial.print(SensorI);
	Serial.print("\t");
	Serial.print("SensorD:");
	Serial.println(SensorD);
	#endif
	
	//Accion y Decision
	if(SensorI>Umbral && SensorD>Umbral){//Adelante
		MI.Adelante(VI);
		MD.Adelante(VD);
	}else if(SensorI>Umbral && SensorD<Umbral){//Izquierda
		MI.Atras(VI);
		MD.Adelante(VD);
	}else if(SensorI<Umbral && SensorD>Umbral){//Derecha
		MI.Adelante(VI);
		MD.Atras(VD);
	}else if(SensorI<Umbral && SensorD<Umbral){ //Parar
		MI.Atras(0);
		MD.Atras(0);
	}	
	
}

void SeguirLineaInv(int Umbral, int VelI,int VelD){

	VI=VelI;
	VD=VelD;
	int SensorI,SensorD;
	//Lectura de Sensores
	SensorI=analogRead(ADSensorI);
	SensorD=analogRead(ADSensorD);
	
	#ifdef DEBUGL
	Serial.print("SensorI:");
	Serial.print(SensorI);
	Serial.print("\t");
	Serial.print("SensorD:");
	Serial.println(SensorI);
	#endif
	
	//Accion y Decision
	if(SensorI<Umbral && SensorD<Umbral){//Adelante
		MI.Adelante(VI);
		MD.Adelante(VD);
	}else if(SensorI<Umbral && SensorD>Umbral){//Izquierda
		MI.Atras(VI);
		MD.Adelante(VD);
	}else if(SensorI>Umbral && SensorD<Umbral){//Derecha
		MI.Adelante(VI);
		MD.Atras(VD);
	}else if(SensorI>Umbral && SensorD>Umbral){ //Parar
		MI.Atras(0);
		MD.Atras(0);
	}	
	
}


void SeguirLineaNL( int Umbral, int VelI,int VelD){
	int SensorI,SensorD;
	int SIOFF=0,SDOFF=0;
	int SION=0,SDON=0; 

	
	//Lectura de Sensores
  SIOFF=analogRead(A1);
  SDOFF=analogRead(A2);
  digitalWrite(pinIR,HIGH);//Encender Emisor IR
  delay(5); //Esperar a que encienda el IR
  SION=analogRead(A1);
  SDON=analogRead(A2);
  digitalWrite(pinIR,LOW);//Apagar Emisor IR
  SensorI=SION-SIOFF;
  SensorD=SDON-SDOFF;
  delay(5);
	
	#ifdef DEBUGL
	Serial.print("SensorI:");
	Serial.print(SensorI);
	Serial.print("\t");
	Serial.print("SensorD:");
	Serial.println(SensorI);
	#endif



	//Accion y Decision
	if(SensorI<Umbral && SensorD<Umbral){//Adelante
		MI.Adelante(VI);
		MD.Adelante(VD);
	}else if(SensorI<Umbral && SensorD>Umbral){//Izquierda
		MI.Atras(VI);
		MD.Adelante(VD);
	}else if(SensorI>Umbral && SensorD<Umbral){//Derecha
		MI.Adelante(VI);
		MD.Atras(VD);
	}else if(SensorI>Umbral && SensorD>Umbral){ //Parar
		MI.Atras(0);
		MD.Atras(0);
	}	
	
}

void SeguidorLineaProp(void)
{
     //Versi贸n 1
	 /*
	 La velocidad se compone de
	 VI=vi+vc; Velocidad Total Motor Izquierdo
	 VD=vd+vc; Velocidad Total Motor Derecho
	 
	 vi=Ki*SensorD; ------> map(SensorD,0,1024,0,200);
	 vd=Kd*SensorI; ------> map(SensorI,0,1024,0,150);
	 vc=Kc*SensorC; ------> map(SensorC,0,1024,0,200);
	 
	 Entonces
	 
	 
	 
	 */
	 
	 
}	


//Secci贸n de Seguidor de Luz

void SeguirLuz(int Umbral, int VelI, int VelD){

	VI=VelI;
	VD=VelD;
	int SensorI,SensorD;
	
	//Lectura de Sensores
	SensorI=analogRead(ADSensorI);
	SensorD=analogRead(ADSensorD);
	
	#ifdef DEBUGL
	Serial.print("SensorI:");
	Serial.print(SensorI);
	Serial.print("\t");
	Serial.print("SensorD:");
	Serial.println(SensorD);
	#endif
	
	//Accion y Decision
	if(SensorI>Umbral && SensorD>Umbral){//Adelante
		MI.Adelante(VI);
		MD.Adelante(VD);
	}else if(SensorI>Umbral && SensorD<Umbral){//Izquierda
		MI.Adelante(VI);
		MD.Atras(VD);		
	}else if(SensorI<Umbral && SensorD>Umbral){//Derecha	
		MI.Atras(VI);
		MD.Adelante(VD);
	}else if(SensorI<Umbral && SensorD<Umbral){ //Parar
		MI.Atras(0);
		MD.Atras(0);
	}
	
}

//Secci贸n BCD a 7 Segmentos
					// p  a,b,c,d,e,f,g
int P7Seg[8]={10,8,7,6,5,4,3,2};

const unsigned char D7Seg[16]={ //abcdefg	
								0b1111110,//0
								0b0110000,//1
								0b1101101,//2
								0b1111001,//3
								0b0110011,//4
								0b1011011,//5
								0b1011111,//6
								0b1110000,//7
								0b1111111,//8
								0b1111011,//9
								0b0000000,//10
								0b0000000,//11
								0b0000000,//12
								0b0000000,//13
								0b0000000,//14
								0b0000000 //15 
								};
								
void Pabcdefg(int T,int AN){

for(int i=0;i<8;i++)
		{
			
			digitalWrite(P7Seg[i],AN);
		}
	

	for(int i=0;i<8;i++)
		{
			digitalWrite(P7Seg[i],!AN);
			delay(T);
			digitalWrite(P7Seg[i],AN);
		}
	


}
				
								
void Iniciar7Seg(void)
{
	for(int i=0;i<8;i++)
	{
		pinMode(P7Seg[i],OUTPUT);
	}
}

//void Iniciar7Seg(int a,int b,int c,int d,int e,int f, int g)

void Mostraren7Seg(int Number,int AN)
{
	int Val=D7Seg[Number]; //Seleccionamos el Valor
	//Escribimos en los pines digitales
	if(AN){
	Val=~Val;
	}
	
	for(int i=0;i<8;i++)
	{	//Serial.println(Val,BIN);
		digitalWrite(P7Seg[7-i],Val%2);
		Val=Val>>1;
	}
	

}


void IniciarSerial(){ //Se deja para compatibilidad con codigos Anteriores
//Esta funcion permite deshabilitar la comunicacion Serial
//al conectar el pin 2 a un se馻l baja.
static int initpin=0;
static int inicializado=0;
if(initpin==0){
pinMode(2,INPUT);
digitalWrite(2,HIGH);
initpin=0;
}
if(digitalRead(2)==HIGH){
  if(inicializado==0){
  Serial.begin(9600);//Iniciar comunicaci髇 Serial
  Serial.setTimeout(1000);//Definir tiempo de espera de 1 segundo
  inicializado=1;
} 
}else if (inicializado==1){
  Serial.end();
  inicializado=0;
}

if(digitalRead(2)==HIGH){
  if(inicializado==0){
  Serial.begin(9600);//Iniciar comunicaci髇 Serial
  Serial.setTimeout(1000);//Definir tiempo de espera de 1 segundo
  inicializado=1;
} 
}else if (inicializado==1){
  Serial.end();
  inicializado=0;
}
}


void ControlSerial(){
//Esta funcion permite deshabilitar la comunicacion Serial
//al conectar el pin 2 a un se馻l baja.
static int initpin=0;
static int inicializado=0;
if(initpin==0){
pinMode(2,INPUT);
digitalWrite(2,HIGH);
initpin=0;
}
if(digitalRead(2)==HIGH){
  if(inicializado==0){
  Serial.begin(9600);//Iniciar comunicaci髇 Serial
  Serial.setTimeout(1000);//Definir tiempo de espera de 1 segundo
  inicializado=1;
} 
}else if (inicializado==1){
  Serial.end();
  inicializado=0;
}
}
