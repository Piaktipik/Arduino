#include <Exbot.h>
unsigned long value;

void setup(){
                //InPin,Buzzer,pinConf 
  IRReceptorInit(8,2,9);
  Serial.begin(9600);
  EstadoConf=0;
}


void loop(){
  //Guardar Teclas-----------------
  if(digitalRead(9)==HIGH){
    delay(1000);
    Serial.println("HIGH");
    if(ConfKey(EstadoConf)==1){
  	EstadoConf++; 
  	T1=millis();
        Serial.println("Recibido");
    }else{
 	DT=millis()-T1;
  	if(DT>5000){
        EstadoConf=0;
 	WriteFunKeyEEPROM();
  	digitalWrite(BUZZ,HIGH);
  	delay(3000);
  	digitalWrite(BUZZ,LOW);
  			}
  		}
  }else{EstadoConf=0;}
//--------------- Fin guardar teclas

  value=ReadIRKey();
  if(value!=0){
    if(value==FunKey[1]){
      //Tarea 1
    }else if(value==FunKey[1]){
      //Tarea 2
    }
   
  Serial.println(value,HEX);
  ResumeIR();
}
  
}

  
}

