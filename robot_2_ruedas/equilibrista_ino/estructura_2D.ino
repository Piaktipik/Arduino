
const int n = 5; // orden del sistema

//double num[] = {1.625,-2.231,0.7463}; // An
//double den[] = {1,-1,0}; // Bn

//double num[] = {1.454,-1,577,0.4204}; // An
//double den[] = {1,-1,0}; // Bn

//double num[] = {3.059,1.045,0.2147}; // An
//double den[] = {1,1,0}; // Bn

double num[] = {0,12.71,-13.06,4.223,0,0}; // An
double den[] = {1,0,0,-0.1669,-0.7402,-0.09283}; // Bn

// se inicializan vectores

//double X = 0;
double P[n];   // p en k
double Pa[]={0,0,0,0,0,0};  // p en k-1


// para el sistema de orden 3 calculamos los primeros valores de p
double estructura_2D(double X){
  double Y = num[n]*X+Pa[0];
  // ejecuto esta sentencia hasta n-1, lo resto de nuevo porque aqui empiezo en 0
  for(int i=0; i < n-1; i++){
    P[i] = Pa[i+1] + num[n-i]*X - den[n-i]*Y;
  }
  
  P[n] = num[0]*X-den[0]*Y;
  // guardo los valores pasados
  for(int i=0; i < n; i++){
    Pa[i] = P[i];
  }
  
  //abs(Y)>1?Y>0?Y=1:Y=-1:0; // limito velo entre -1 y 1
  
  return Y;
}


