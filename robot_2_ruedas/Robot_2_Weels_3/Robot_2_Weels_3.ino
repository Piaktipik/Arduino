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
float timeStep = 0.03;          //en s. Need a time step value for integration of gyro angle from angle/sec

// ---------------- Velocidad Motores ------------------------

float VEL1 = 0; // velocidad motor1
float VEL2 = 0; // velocidad motor2

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
  SetTunings(1,0,0.1);          // bien -> v = 0.4
  
  // otros parametros del controlador
  
  SetSampleTime(30);           // indicamos el periodo de muestreo
  SetOutputLimits(-1,1);       // delimitamos los valores min, max
  Setpoint = 0.00;             // iniciamos el setpoint en 0
  SetMode(1);                  // ponemos el PID en modo automatico
  SetControllerDirection(0);   // se pone el controlador de accion directa
  Initialize();
  
  
  //Serial3.print("#o1");
  // se inicia la red neuronal madaline
  iniciar_MAD();
  //iniciar_BRA();
  //inciar_FUZ();
}

// ----------------------- programa principla ----------------------- 
void loop() {
  timer = micros();       // tiempo de inicio del loop
  
  //if ((roll<90) && roll>-90){
    Input = (roll1p/90.0); // envio entrada al PID
  //}
  //Serial.print("\t");
  Serial.print(Input);
  //Serial.print('c');
  
  float red_res = madaline(Input);
  //float red_res = base_radial_adaptativo(Input);
  //float red_res = fuzzy(Input);
  
  Serial.print("\t");
  Serial.print(red_res);
  Serial.print("\t");
  
  //error = roll;
  
  if (Input < -0.05 || Input > 0.05){
    motor(0,-red_res);  // motor A
    motor(1,-red_res);  // motor B
  }else{
    motor(0,0);  // motor A
    motor(1,0);  // motor B
  }
  
  Compute();   // calculo el PID
  
  //rollp = roll;
  //}
  // -------------- calculo la inclinacion -------------- 
  //read sensors
  readGyro();readAcc();readMag();
  //calcualte pitch, roll, yaw, kalman etc
  Calculations();
  //print values
  
  PrintVals();
  
  // --------- ejecuto controlador ----------
  // calculo error
//  if (toma_datos){
//    if((error > 0)){
//      motor(0,0);  // motor A
//      motor(1,0);  // motor B
//    }
////    vel_m_izq = float(vel1)/255;
////    vel_m_der = float(vel2)/255;
//  }
  
  //error = Setpoint - roll1p;
  //error = Setpoint - rollajustado;
  
//  Serial.print(Output);
//  Serial.print("\t");
  
//  if (controlador_on){
//    motor(0,Output);  // motor A
//    motor(1,Output);  // motor B
//  }
  
//  motor(0,VEL1);  // motor A
//  motor(1,VEL1);  // motor B
  
//  double res = estructura_2D(error);
//  Serial.print("\t error:");
//  Serial.print(error);
//  Serial.print("\t res: ");
//  Serial.print(res);
//  Serial.println();
//  
//  motor(0,res);  // motor A
//  motor(1,res);  // motor B
//  Serial3.print("#1");
//  mostrar_razer();
  
  // -------------- reviso el serial en busca de datos -------------- 
  if(Serial1.available() > 0){datoserial(Serial1.read());}
  
  if(Serial3.available() > 0){imu_razer();}
  
  // -------------- fin del programa -------------- 
  // calculo la demora del programa, y ajusto el tiempo de clada ciclo a 20ms
  timer1 = micros(); // tiempo de finalizacion del loop
  unsigned int ajuste_t = (timer1-timer);
  // si el tiempo en menor lo ajusto
  if (ajuste_t < (timeStep*1000000)){ 
    ajuste_t = (timeStep*1000000) - ajuste_t; // calculo el tiempo muerto restante
    // espero el tiempo necesario para llegar a los 20ms (teniendo en cuanta del desvorde de delayMicroseconds() a 16000)
    if (ajuste_t >= 16000){
      delayMicroseconds(16000);
      delayMicroseconds(ajuste_t - 16000);
    }else{
      delayMicroseconds(ajuste_t);
    }
  }else{
    Serial.println("tiempo de integracion de giroscopo mayor a 30ms");
  }
}
// --------------------------- fin loop ------------------------------

void PrintVals(){
  
  errore = (error*90);
  
  Serial1.print(millis());
  Serial1.print(",");
  Serial1.print(roll1p);
  //Serial.print(roll);
  Serial1.print(",");
  Serial1.print(Setpoint);
  Serial1.print(",");
  Serial1.print(errore);
  Serial1.print(",");
  Serial1.print(VEL1); // velocidad motores
  Serial1.print(",");
  Serial1.print(VEL2); 

  Serial1.println();
}

//void PrintVals1(){
//  
//  Serial.print(millis());
//  Serial.print(",");
//  Serial.print(roll1p);
//  //Serial.print(roll);
//  Serial.print(",");
//  Serial.print(Setpoint);
//  Serial.print(",");
//  Serial.print(error);
//  Serial.print(",");
//  Serial.print(VEL1);  
//  Serial.print(",");
//  Serial.print(VEL2); 
//
//  Serial.println();
//}

// ----------------- respuesta serial ---------------
void datoserial(char dato){
    if(dato == 'a'){
      motor(0,VEL1);  // motor A
      motor(1,VEL1);  // motor B
      VEL2 = 0;
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
      VEL2 = 0;
    }
    if(dato == 'r'){
//      rollmax = 0;
//      rollmin = 0;
      motor(0,0);  // motor A
      motor(1,0);  // motor B
    
    }
    if(dato == 't'){
      toma_datos = !toma_datos;
      error = 0; // indico que entro en modo toma de datos
    }
    if(dato == 'v'){
      delay(100);
      VEL1 = float(Serial1.read())/255; // velocidad de motores
      VEL2 = float(Serial1.read())/255;
      error = 0; // indico que entro en modo toma de datos
    }
}

/////////////////////////////////////////////// comentarios ///////////////////////////////////////////////

//  if (roll1p < rollmin){rollmin = roll1p;}
//  if (roll1p > rollmax){rollmax = roll1p;}
  
  // obtengo la señal maxima y minima de roll para escalarla correctamente
  // rollajustado = ((roll1p/(rollmax-rollmin)))*100;
  
//  rollajustado = roll1p;


  //float rollmax = 0;
//float rollmin = 0;
//float rollajustado = 0;  // Output of Kalman filter, final roll value
//  if (roll1p < rollmin){rollmin = roll1p;}
//  if (roll1p > rollmax){rollmax = roll1p;}
  
  // obtengo la señal maxima y minima de roll para escalarla correctamente
  // rollajustado = ((roll1p/(rollmax-rollmin)))*100;
  
//  rollajustado = roll1p;

//  valmin = min(valmin,roll1p);
//  valmax = max(valmax,roll1p);
//  //Medida = roll1p;
//  Medida1 = map(roll1p, -90.0,90.0,2.5,67);
//  // 360/(67-3) = 5.2941
//  //Medida = (roll1p-1)*5.2941 - 180;
//  Medida = (roll1p-31)*2.5714;
  
