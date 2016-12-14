#include <Exbot.h>

unsigned long value;
int led1=10;
int led2=11;
int led3=12;
void setup(){

  IRInit(8);//Iniciar IR 
  Serial.begin(9600);
  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
}

void loop(){
  value=ReadIRKey();
  if(value!=0){
   //case
   switch(value){
   case 0x76044FE0:
   digitalWrite(led1,!digitalRead(led1));
   break;
   case 0x76044FD0:
   digitalWrite(led2,!digitalRead(led2));
   break;
   case 0x76044FF0:
   digitalWrite(led3,!digitalRead(led3));
   break;
 }
  Serial.println(value,HEX);
  ResumeIR();
}
  
}

