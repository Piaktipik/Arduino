
  //---------------Controlador------------
//  if ((Medida<249) && (Medida>256)){
//    parar();}
//  else{
//    controlador = Control_PID();
//    if (millis() > tiempo + 50){
//      if((controlador) > 255){vel1 = 255;}
//        else {vel1 = abs(controlador);}
//        if (error>0) { adelante();}
//        if (error<0) { atras();}
//      tiempo = millis();
//      }
//  }

//
//float Control_PID(){
//  return kp*(tv*s + h/(tn) + 1);
//}
//
//void calcular_error(){
//  // calculo -> cambio de tiempo entre calculo de errores
//  del_t = (timeStep * 1000)-(timer1-timer);
////  tiempo_s = millis();           // actualizo tiempo
////  del_t = (del_t/1000.0);        // pongo el delta de tiempo en segundos
//  
//  //vel1 = (vel1/255)*255; // escalo la velocidad medida a 0 a 255
//  
//  // calculo el error (deseado - real)
//  error = SetP - Medida;
//  
//  // calculo s
//  s = (error-error_p)/del_t;
//  error_p = error; // actualizo el error_p
//  h = error * del_t;
////  Serial.print("del_t:");
////  Serial.print(del_t);
////  Serial.print("\t s:");
////  Serial.print(s);
////  Serial.print("\t error:");
////  Serial.print(error);
//} 
//
//void conf_controlador(){
//    kp = 2;
//    tn = 1.6;
//    tv = 0.07;
//  }


void Compute(){
  
  if(!inAuto) return;
  unsigned long now = millis();
  int timeChange = (now - lastTime);
  
  if(timeChange>=SampleTime){
    // Calculamos todos los errores.
    double error = Setpoint - Input;
    ITerm += (ki * error);
    if(ITerm > outMax) ITerm = outMax;
    else if(ITerm < outMin) ITerm = outMin;
    double dInput = (Input - lastInput);
    // Calculamos la función de salida del PID.
    Output = kp * error + ITerm- kd * dInput;
    if(Output > outMax) Output = outMax;
    else if(Output < outMin) Output = outMin;
    // Guardamos el valor de algunas variables para el próximo recálculo.
    lastInput = Input;
    lastTime = now;
  }
}

void SetTunings(double Kp, double Ki, double Kd){
  if (Kp<0 || Ki<0|| Kd<0) return;
  double SampleTimeInSec = ((double)SampleTime)/1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;
  if(controllerDirection == REVERSE){
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}

void SetSampleTime(int NewSampleTime){
  if (NewSampleTime > 0){
    double ratio = (double)NewSampleTime / (double)SampleTime;
    ki *= ratio;
    kd /= ratio;
    SampleTime = (unsigned long)NewSampleTime;
  }
}

void SetOutputLimits(double Min, double Max){
  if(Min > Max) return;
  outMin = Min;
  outMax = Max;
  if(Output > outMax) Output = outMax;
  else if(Output < outMin) Output = outMin;
  if(ITerm> outMax) ITerm= outMax;
  else if(ITerm< outMin) ITerm= outMin;
}

void SetMode(int Mode){
  bool newAuto = (Mode == AUTOMATIC);
  if(newAuto && !inAuto){ // Para cambiar de manual a automático, inicializamos algunos parámetros.
    Initialize();
  }
  inAuto = newAuto;
}

void Initialize(){
  lastInput = Input;
  ITerm = Output;
  if(ITerm> outMax) ITerm = outMax;
  else if(ITerm< outMin) ITerm = outMin;
}

void SetControllerDirection(int Direction){
  controllerDirection = Direction;
}

