
// -------------------------------- motores -------------------------------- 

// ------------- se configuran los puertos ------------- 

// Motor1
int PWM1 = 5; // Señal vel a
int DIR1 = 4; // Direccion a

// Motor2
int PWM2 = 6; // Señal vel b
int DIR2 = 7; // Direccion b

// ------------- incializacion de motores ------------- 
void iniciar_motores(){
  // Motor A
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  // Motor B
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  
  motor(0,0);  // apago el motor A
  motor(1,0);  // apago el motor B
}

// ------------- seteo de motores ------------- 
void motor(int MOT, float VELO){
  abs(VELO)>1?VELO>0?VELO=1:VELO=-1:0; // limito velo entre -1 y 1
  if(MOT){
    // Motor A -Izquierdo
    digitalWrite(DIR1, (VELO>0?1:0));
    analogWrite(PWM1, abs(VELO*255));
  }else{
    // Motor B - Derecho
    digitalWrite(DIR2, (VELO>0?1:0));
    analogWrite(PWM2, abs(VELO*255));
  }
}

void probar_motores(){
  for (float i=-1;i<1;i=i+0.01){
    motor(0,i); // configuracion motores (motor 0-A,1-B, direccion y velocidad valor = {-1,1})
    motor(1,i);
    delay(200);
  }
  for (float i=1;i>-1;i=i-0.01){
    motor(0,i); // configuracion motores (motor 0-A,1-B, direccion y velocidad valor = {-1,1})
    motor(1,i);
    delay(200);
  }
}  
