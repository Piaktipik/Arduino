// Variables de trabajo.
unsigned long lastTime;
double Input, Output, Setpoint;
double ITerm, lastInput;
double kp, ki, kd;
int SampleTime = 20; // Tiempo de muestreo 1 segundo.
double outMin, outMax;
bool inAuto = false;
#define MANUAL 0
#define AUTOMATIC 1

void Compute(){
  if(!inAuto) return;
  unsigned long now = millis();
  int timeChange = (now - lastTime);
  if(timeChange>=SampleTime){
    // Calculamos todos los errores.
    double error = Setpoint - Input;
    
    ITerm+= (ki * error);              // costante integral
    
    if(ITerm> outMax) ITerm= outMax;
    
    else if(ITerm< outMin) ITerm= outMin;
    
    double dInput = (Input - lastInput);       // se deriva la entrada
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
  double SampleTimeInSec = ((double)SampleTime)/1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;
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
  if(ITerm> outMax) ITerm= outMax;
  else if(ITerm< outMin) ITerm= outMin;
}


