// por: julian galvez, juan diego cardenas

// final values
float pitchPrediction = 0; // Output of Kalman filter, final pitch value
float yawRaw = 0;          // final yaw value. yaw doesnt go through kalman! has its own calc.

float roll1p = 0;  // Output of Kalman filter, final roll value

//----------------------Fin de configuracion IMU-------------------------------------------------------
//----------------Variables controlador------------------------
// Variables de trabajo.
double error;
double errore;

double Input, Output;
double Setpoint = 0.0;

bool controlador_on = false;  // estado inicial del controlador
bool toma_datos = false;

// ---------------- Variables temporales kalman ------------------------

//time, used in kalman filtering to keep a constant loop time
unsigned long timer = 0;
unsigned long timer1 = 0;
float timeStep = 0.02;          //en s. Need a time step value for integration of gyro angle from angle/sec

// ---------------- Velocidad Motores ------------------------

float VEL1 = 0; // velocidad motor1
float ACIO = 0;  // velocidad motor2

// ---------------- variables imu 2 ---------------- 
double roll = 10;   // imu razor
double rollp = 10;  // imu pololu


// ----------------------- configuracion ----------------------- 
void setup(){
  Serial.begin(115200);  // begin serial com
  Serial1.begin(115200); // begin serial com
  Serial3.begin(57600);  // incio puerto serial para comunicacion con imu razer
  iniciar_IMU();        // configuro IMU y inicializo
  CalibrateIMU();       // calibrate the IMU for level starting pos.
  iniciar_motores();    // configuro puertos motores y los detengo
  //conf_controlador();   // Inicializa el controlador
  // ------ configurar controlador -------
  
  // otros parametros del controlador
  
  //SetSampleTime(30);         // indicamos el periodo de muestreo  -> peiodo de muestreo dado por loop
  SetOutputLimits(-1,1);       // delimitamos los valores min, max
  Setpoint = 0.00;             // iniciamos el setpoint en 0
  //SetMode(1);                  // ponemos el PID en modo automatico
  //SetControllerDirection(0);   // se pone el controlador de accion directa
  
  
  SetTunings(0,0,0);          // parametros kp, ki, kd
  
  Initialize();          // se inicializa el controlador
}

// ----------------------- programa principla ----------------------- 
void loop() {
  timer = micros();       // tiempo de inicio del loop
  // -------------- calculo la inclinacion -------------- 
  // se miden los sensores
  // calcualte pitch, roll, yaw, kalman etc
  Calculations();
  
  // -------------- calculo el PID -------------- 
  //Input = (roll1p/90.0); // envio entrada al PID escalada entre -1 y 1 
  Input = roll1p;
  //Compute();   // calculo el PID
  
  error = (Setpoint - Input);
  Output = estructura_2D(-error);
  
  Output = Output/255;
  
  // -------------- aplico salida PID a los motores--------------
  if(controlador_on){
    VEL1 = Output;
    motor(0,Output);  // motor A
    motor(1,Output);  // motor B
  }
  // -------------- toma de datos planta -----------------
  // apago cuando llega al algulo deseado
  if(toma_datos && Input > 0){
    motor(0,0);  // motor A
    motor(1,0);  // motor B 
  }
  
  // -------------- envio datos a processing --------------
  //print values
  PrintVals();
  
  // -------------- muestro informacion por serial -------------- 
  Serial.print(Input);
  Serial.print("\t");
  
  Serial.print(Output);
  Serial.print("\t");

  Serial.print(micros()-timer);
  Serial.print("\t");
  
  Serial.println();
  
  // -------------- reviso el serial en busca de datos -------------- 
  if(Serial1.available() > 0){datoserial(Serial1.read());}
  //delay(12);
  
  // -------------- fin del programa -------------- 
  // calculo la demora del programa, y ajusto el tiempo de clada ciclo a 20ms
  timer1 = micros(); // tiempo de finalizacion del loop
  unsigned int ajuste_t = (timer1-timer);
  // si el tiempo en menor lo ajusto
  if (ajuste_t < (20000)){ 
    ajuste_t = (20000 - ajuste_t); // calculo el tiempo muerto restante
    // espero el tiempo necesario para llegar a los 20ms (teniendo en cuanta del desvorde de delayMicroseconds() a 16000)
    if (ajuste_t >= 16000){
      //delayMicroseconds(16000);
      delayMicroseconds(15870);
      delayMicroseconds(ajuste_t - 16000);
    }else{
      delayMicroseconds(ajuste_t-130);
    }
    // imprimo el tiempo demorado por este ciclo
    //Serial.print(micros()-timer);
    //Serial.print("\t");
  }else{
    Serial.println("tiempo de integracion de giroscopo mayor a 20ms");
  }
}
// --------------------------- fin loop ------------------------------

void PrintVals(){
  
  //errore = (error*90);
  
  Serial1.print(millis());
  Serial1.print(",");
  Serial1.print(roll1p);
  Serial1.print(",");
  Serial1.print(Setpoint);
  Serial1.print(",");
  Serial1.print(errore);
  Serial1.print(",");
  Serial1.print(VEL1); // velocidad motores
  Serial1.print(",");
  Serial1.print(ACIO); 

  Serial1.println();
}

// ----------------- respuesta serial ---------------
void datoserial(char dato){
    if(dato == 'a'){
      motor(0,VEL1);  // motor A
      motor(1,VEL1);  // motor B
      ACIO = 1;
    }
    if(dato == 'p'){
      motor(0,0);  // motor A
      motor(1,0);  // motor B
    }
    if(dato == 'h'){
      motor(0,-1);  // motor A
      motor(1,-1);  // motor B
    }
    if(dato == 'd'){
      motor(0,-1);  // motor A
      motor(1,1);  // motor B
    }
    if(dato == 'i'){
      motor(0,1);  // motor A
      motor(1,-1);  // motor B
    }
    if(dato == 'c'){
      controlador_on = !controlador_on;
    }
    if(dato == 'r'){
//      rollmax = 0;
//      rollmin = 0;
      motor(0,0);  // motor A
      motor(1,0);  // motor B
      ACIO = 0;
    }
    if(dato == 't'){
      toma_datos = !toma_datos;
      ACIO = 1;
    }
    if(dato == 'v'){
      delay(100);
      VEL1 = float(Serial1.read())/255; // velocidad de motores
      //VEL1 = float(Serial1.read())/255;
      Serial1.read();
      ACIO = 0; // indico que entro en modo toma de datos
    }
}

/////////////////////////////////////////////// comentarios ///////////////////////////////////////////////

  
