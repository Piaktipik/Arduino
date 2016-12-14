// Variables de trabajo.
unsigned long lastTime;             // guardo el instante donde se ralizo el ultimo calculo
//double Input, Output, Setpoint;   // parametros de  entrada, salida y deseados del controlador
double ITerm = 0, lastInput;            // Termino integral, termino diferencial
double kp, ki, kd;                  // constanstes del pid
int SampleTime = 20;                // Tiempo de muestreo 20m segundos.
double outMin, outMax;              // valor maximo y minimo de la salida
bool inAuto = false;

int controllerDirection = 0;

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void Compute(){
  if(!inAuto) return; // esto nos permite poner el pid en modo manual
  
  //  ------------  si esta en modo automatico ------------ 
  unsigned long now = millis();            // guardamos e tiempo actual
  int timeChange = (now - lastTime);       // calculamos el tiempo trascurrido desde la ultima ejecucion
  if(timeChange>=SampleTime){
    // Calculamos todos los errores.
    double error = Setpoint - Input;
    // ------------ costante integral ------------ 
    ITerm += (ki * error);
    // limitamos al rango
    if(ITerm > outMax) ITerm = outMax;        // se limita al valor maximo
    else if(ITerm< outMin) ITerm = outMin;    // se limita al se al valor minimo
    
    // ------------ calculamos derivada ------------ 
    double dInput = (Input - lastInput);      // se deriva la entrada
    
    //  ------------ Calculamos la función de salida del PID ------------ 
    Output = kp*error + ITerm - kd*dInput;
    // limitamos al rango
    if(Output > outMax) Output = outMax;
    else if(Output < outMin) Output = outMin;
    
    // Guardamos el valor de algunas variables para el próximo recálculo.
    lastInput = Input;  // valor para la derivada
    lastTime = now;     // tiempo del fin de la ultima ejecucion
  }
}

// ---------------- se configaran los parametros del controlador ---------------- 
void SetTunings(double Kp, double Ki, double Kd){
  if (Kp<0 || Ki<0|| Kd<0) return;
  double SampleTimeInSec = ((double)SampleTime)/1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;
  if(controllerDirection == 1){
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}

//  ---------------- se configura el tiempo de muestreo ---------------- 
void SetSampleTime(int NewSampleTime){
  if (NewSampleTime > 0){
    double ratio = (double)NewSampleTime / (double)SampleTime;
    ki *= ratio;
    kd /= ratio;
    SampleTime = (unsigned long)NewSampleTime;
  }
}

//  ---------------- se configuran los valores ultimos ---------------- 
void SetOutputLimits(double Min, double Max){
  if(Min > Max) return;
  outMin = Min;
  outMax = Max;
  
  if(Output > outMax) Output = outMax;
  else if(Output < outMin) Output = outMin;
  
  if(ITerm > outMax) ITerm = outMax;
  else if(ITerm < outMin) ITerm = outMin;
}

// ---------------- se configura el modo de muestreo ---------------- 
void SetMode(int Mode){
  bool newAuto = (Mode == 1);
  if(newAuto && !inAuto){ // Para cambiar de manual a automático, inicializamos algunos parámetros.
    Initialize();
  }
  inAuto = newAuto;
}

//  ---------------- se inicializa el controlador ---------------- 
void Initialize(){
  lastInput = Input;
  ITerm = Output;
  if(ITerm > outMax) ITerm = outMax;
  else if(ITerm < outMin) ITerm = outMin;
}

//  ---------------- se donfigura la direccion del controlador ---------------- 
void SetControllerDirection(int Direction){
  controllerDirection = Direction;
}
