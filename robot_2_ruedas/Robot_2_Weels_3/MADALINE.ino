float fs = 0.1;//Factor de sensibilidad de controlador
float ys; //Salida del sistema
float sp; //SetPoint
float ers = 0; //Error del sistema
//const int m1 = 0; 
const int NE = 4; //Numero de entradas
const int NO = 10; //Numero de neuronas ocultas
float uk; //Entrada al sistema
float de2du; //Derivada del error respecto a la entrada
//float PR; 
//float rs;
float alpha = 0.03; //Factor de aprendizaje

float X[NE] = {1, 0, 0, 0}; //Entradas del sistema autoregresivo
//float X[NE - 1]; //Entradas de sistema para control PID
float W[NO][NE - 1]; //Pesos de la RN
float S[NO]; 
float C[NO];
float XC[3];
float WP[NO][3];
float CP[NO];

float PA;
float IA;
float DA;
void iniciar_MAD() {
  // Generación de pesos de la red neuronal
  for(int j = 1; j <= NO; j++){
    for(int i = 1; i <= NE; i++){
      W[j][i] = random(-1, 1);
    }
    C[j] = random(-1, 1);
  }
  
  // Generación de pesos del controlador
  for(int j = 1; j <= NO; j++){
    for(int i = 1; i <= 3; i++){
      WP[j][i] = random(-1, 1);
    }
    CP[j] = random(-1, 1);
  }
}

float madaline(float ys) {

  // Poner aquí el código para que lectura de la planta sea solo 
  // Angulo que necesitamos
  //ys = Serial.read();

  sp = 0;
  ers = sp - ys; //Parte proporcional
  
  DA = (sp - ys) - ers; //Parte derivativa
  IA = (sp - ys) + ers; //Parte integral
  
  PA = ers;
  XC[1] = PA;
  XC[2] = DA;
  XC[3] = IA;
  
  de2du = 0;
  
  //Calculo del controlador
  for (int j = 1; j <= NO; j++){
    S[j] = 0;
    for (int i = 1; i <= 3; i++){
      S[j] = S[j] + WP[j][i]*XC[i];
    }
    de2du = de2du + CP[j]*S[j];
  }
  
  
  
  //uk es la señal del controlador que se manda a la planta
  
  for(int i = 1; i <= 3; i++){
    X[i] = XC[i];
  }
  
  //Actualización de pesos
  for (int j = 1; j <= NO; j++){
    S[j] = 0; 
    for (int i = 1; i <= 3; i++){
      S[j] = S[j] + W[j][i]*XC[i];
    } 
    C[j] = C[j] + alpha*ers*S[j];
  }
  
  for (int j = 1; j <= NO; j++){
    CP[j] = CP[j] - fs*alpha*ers*S[j];
    for (int i = 1; i <= 3; i++){
      WP[j][i] = WP[j][i] - (fs*fs)*alpha*ers*CP[j]*XC[i]; //Factor raro en el programa de visual
    }
  }
  
  uk = uk - fs*de2du;
  
  abs(uk)>1?uk>0?uk=1:uk=-1:0;
  
  return uk;
  
} // fin madaline
