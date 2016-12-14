//#include <math.h>
//
//float fs = 0.3;//Factor de sensibilidad de controlador
//float ys; //Salida del sistema
//float sp; //SetPoint
//float ers; //Error del distema
////const int m1 = 0; 
//const int NE = 4; //Numero de entradas
//const int NO = 10; //Numero de neuronas ocultas
//float uk; //Entrada al sistema
//float de2du; //Derivada del error respecto a la entrada
//float alpha = 0.3; //Factor de aprendizaje
//
//float X[NE];//Entradas del sistema autoregresivo
//float S[NO];
//float S1[NO];
//float S2[NO];
//float h[NO];
//float C[NO];
//float XC[NO][NE];
//float XCO[NE] = {1, 0, 0, 0}; ;
//float W[NO][NE];
//float CP[NO]; //Linea 25
//
//void iniciar_BRA() {
// // Generación de pesos de la red neuronal
//  for(int j = 0; j < NO; j++){
//    for(int i = 0; i < NE; i++){
//      XC[j][i] = random(-1, 1);
//    }
//    C[j] = random(-1, 1);
//  }
//} //Linea 35
//
//float base_radial_adaptativo(float ys) {
//  // Poner aquí el código para que lectura de la planta sea solo 
//  // Angulo que necesitamos
//  //ys = Serial.read();
//  
//  X[3] = ys;
//  sp = 0;
//  ers = sp - X[3]; //Parte proporcional  
//  de2du = 0;
//  
//  //Calculo del controlador
//  
//    for (int j = 0; j < NO; j++){ //Ojo con esta parte
//    S[j] = 0;
//      for (int i = 0; i < NE; i++){
//        S[j] = S[j] + pow((X[i] - XC[j][i]), 2);
//        S1[j] = S1[j] + (XC[j][i] - X[i]);
//        S2[j] = S2[j] + (X[i] - XC[j][i]);
//      }
//    }
//  
//  for (int j = 0; j < NO; j++){
//    h[j] = exp(-S[j]/4);
//    S1[j] = S1[j] / 4;
//    S2[j] = S2[j] / 4;
//    de2du = de2du + C[j]*S[j]*S1[j]*XCO[j];
//  }
//  
//  uk = X[1] - fs*de2du;
//  
//  //uk es la señal del controlador que se manda a la planta
//  
//  X[1] = X[2];
//  X[2] = X[3];
//  X[3] = ys;
//  X[0] = uk;
//  
//  //Actualización de pesos
//  for (int j = 0; j < NO; j++){
//    S[j] = 0; 
//    for (int i = 0; i < NE; i++){
//      S[j] = S[j] + (X[i] - XC[j][i])*(X[i] - XC[j][i]);
//    }
//    h[j] = exp(-S[j]/4); 
//    C[j] = C[j] + alpha*ers*S[j];
//  }
//  
//  for (int j = 0; j < NO; j++){
//    for (int i = 0; i < NE; i++){
//      W[j][i] = W[j][i] + fs*alpha*ers*C[j]*h[j]*((X[i] - XC[j][i])/4); //Factor raro en el programa de visual
//    }
//  }
//  
//  return uk;
//}
