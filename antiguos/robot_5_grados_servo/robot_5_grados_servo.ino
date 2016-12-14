
// libreria para el manejo de servos
#include <Servo.h> 

// creo los objetos tipo servo
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// variables para guardar la posicion de los servos
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
 
void setup(){ 
  
  // digo el pin del servo correspondiente
  servo1.attach(4);
  servo2.attach(4); 
  servo3.attach(4); 
  servo4.attach(4); 
  servo5.attach(4); 
  
} 
 
 
void loop(){
  // se programa la secuencia
  moverservo(0,90,servo1);
  moverservo(90,0,servo1);
} 


void moverservo(int posi, int posf, Servo servo){
  int pos;
  for(pos = posi; pos < posf;pos++){
    if(posi < posf){
      servo.write(pos);
    }
    else{
      servo.write(2*posi-pos);
    }
    delay(15);                       
  } 


}
