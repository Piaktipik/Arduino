
//unsigned long lastTime;
double ITerm; 
//double lastInput;
double lastError = 0;

const int sua = 20;
double prome[sua];
int num1 = 0;
double num2;

double kp, ki, kd;
int SampleTime = 20; //en ms Tiempo de muestreo 1 segundo.

double outMin, outMax;
//bool inAuto = false;
//#define MANUAL 0
//#define AUTOMATIC 1
//#define DIRECT 0
//#define REVERSE 1
//int controllerDirection = DIRECT;

void Compute(){
  
  //if(!inAuto) return;
  //unsigned long ahora = millis();
  //int timeChange = (ahora - lastTime);
  //if(timeChange>=SampleTime){
    
    // Calculamos todos los errores.
    error = Setpoint - Input;
    
    // se obtiene el termino integral
    ITerm += (ki * error);
    if(ITerm > outMax) ITerm = outMax;
    else if(ITerm < outMin) ITerm = outMin;
    
    // se obtiene el termino derivativo
    //double dInput = (Input - lastInput);  // derivada de la entrad
    double dError = (error - lastError);  // derivada del error
    
    // suabizo la repsesta del error
    prome[num1] = dError;
    num1 = (num1+1)%sua;
    num2 = 0;
    for(int i=0;i<sua;i++){
      num2 += prome[i];
    }
    dError = num2/sua;
    
    // Calculamos la función de salida del PID.
    //Output = kp * error + ITerm- kd * dInput;
    Output = kp * error + ITerm + kd * dError;
    
    if(Output > outMax) Output = outMax;
    else if(Output < outMin) Output = outMin;
    
    // Guardamos el valor de algunas variables para el próximo recálculo.
    //lastInput = Input;
    lastError = error;
    //lastTime = ahora;
  //}
}

void SetTunings(double Kp, double Ki, double Kd){
//  if (Kp<0 || Ki<0|| Kd<0) return;
//  double SampleTimeInSec = ((double)SampleTime)/1000;
  kp = Kp;
  ki = Ki * timeStep;
  kd = Kd / timeStep;
//  if(controllerDirection == REVERSE){
//    kp = (0 - kp);
//    ki = (0 - ki);
//    kd = (0 - kd);
//  }
}

//void SetSampleTime(int NewSampleTime){
//  if (NewSampleTime > 0){
//    double ratio = (double)NewSampleTime / (double)SampleTime;
//    ki *= ratio;
//    kd /= ratio;
//    SampleTime = (unsigned long)NewSampleTime;
//  }
//}

void SetOutputLimits(double Min, double Max){
  if(Min > Max) return;
  outMin = Min;
  outMax = Max;
  if(Output > outMax) Output = outMax;
  else if(Output < outMin) Output = outMin;
  if(ITerm> outMax) ITerm= outMax;
  else if(ITerm< outMin) ITerm= outMin;
}

void Initialize(){
  //lastInput = Input;
  lastError = error;
  ITerm = Output;
  if(ITerm> outMax) ITerm = outMax;
  else if(ITerm< outMin) ITerm = outMin;
}

//void SetControllerDirection(int Direction){
//  controllerDirection = Direction;
//}

