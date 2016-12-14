
// -------------------------------- motores -------------------------------- 

// ------------- se configuran los puertos ------------- 

// Motor A conectado entre A01 y A02
int PWMA = 6; // Señal vel a
int AIN1 = 7; // Direccion a

// Motor B conectado entre B01 y B02
int PWMB = 9; // Señal vel b
int BIN2 = 8; // Direccion b

// ------------- incializacion de motores ------------- 
void iniciar_motores(){
  // Motor A
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  // Motor B
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  motor(0,0);  // apago el motor A
  motor(1,0);  // apago el motor B
}

// ------------- seteo de motores ------------- 
void motor(int MOT, float VELO){
  abs(VELO)>1?VELO>0?VELO=1:VELO=-1:0; // limito velo entre -1 y 1
  if(MOT){
    // Motor A -Izquierdo
    digitalWrite(AIN1, (VELO>0?1:0));
    analogWrite(PWMA, abs(VELO*255));
  }else{
    // Motor B - Derecho
    digitalWrite(BIN2, (VELO>0?1:0));
    analogWrite(PWMB, abs(VELO*255));
  }
}

void probar_motores(){
  for (float i=-1;i<1;i=i+0.01){
    motor(0,i); // configuracion motores (motor 0-A,1-B, direccion y velocidad valor = {-1,1})
    motor(1,-i);
    delay(50);
    mostar_encoders();
  }
  for (float i=1;i>-1;i=i-0.01){
    motor(0,i); // configuracion motores (motor 0-A,1-B, direccion y velocidad valor = {-1,1})
    motor(1,-i);
    delay(50);
    mostar_encoders();
  }
}  
