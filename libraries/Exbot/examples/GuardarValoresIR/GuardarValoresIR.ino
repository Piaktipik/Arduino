#include <Exbot.h>
unsigned long value;
int led1=10;
int led2=11;
int led3=12;
void setup(){
                //InPin,Buzzer,pinConf 
  IRReceptorInit(8,2,9);
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  
}


void loop(){
 
 SaveIRValues();
  value=ReadIRKey();
  if(value!=0){
    if(value==FunKey[0]){
      digitalWrite(led1,!digitalRead(led1));
    }else if(value==FunKey[1]){
      //Tarea 2
      digitalWrite(led2,!digitalRead(led2));
    }   
  Serial.println(value,HEX);
  ResumeIR();
}  
}

