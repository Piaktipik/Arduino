//
//
//const int nce = 5;
//float u1;
//float u2;
//int Ce1;
//int Ce2;
//float erp = 0;
//float dxe;
//const int dnce = 5;
//float du1;
//float du2;
//int dCe1;
//int dCe2;
//float derp = 0;
//float ddxe;
//float de2du;
//float ys;
//
//float sp = 0;
//float alfa = 0.3;
//float alfa2 = 0.3;
//float PR;
//float um = 0;
//float erpp = 0; //Error pasado
//
//float npe[nce];
//float dnpe[dnce];
//float CM[7] = {-1, -0.6, -0.3, 0, 0.3, 0.6, 1};
//float R[4];
//int SR[4];
//float DCS[nce][dnce] = {{0, 0, 1, 2, 3},{0, 1, 1, 2, 2},{1, 1, 2, 2, 3},{3, 2, 3, 4, 4},{4, 3, 4, 4, 5}};
//
//
//void inciar_FUZ() {
//  // put your setup code here, to run once:
//  dxe = (1 - (-1))/(nce - 1);
//  for (int i = 1; i<=(nce - 1); i++){
//    npe[i] = -1 + i*dxe;
//  }
//  
//  ddxe = (1 - (-1))/(dnce - 1);
//  for (int i = 1; i<=(nce - 1); i++){
//    dnpe[i] = -1 + i*ddxe;
//  }
//}
//
//float fuzzy(float ys) {
//  
//  //Lectura del serial para ys
//  erpp = erp;
//  erp = sp - ys;
//  derp = erp - erpp;
//  
//  for (int i = 1; i <= (nce - 2); i++){
//    if ((npe[i] < erp) && erp <= npe[i + 1]){
//      u1 = (-1 / (npe[i + 1] - npe[i])*(erp - npe[i + 1]));
//      u2 = (1 / (npe[i + 1] - npe[i])*(erp - npe[i]));
//      Ce1 = i;
//      Ce2 = i + 1;
//    }
//  }
//  
//  for (int i = 1; i<=(dnce - 2); i++){
//    if ((dnpe[i] < derp) && (derp <= dnpe[i + 1])){
//      du1 = (-1 / (dnpe[i + 1] - dnpe[i])*(derp - dnpe[i + 1]));
//      du2 = (1 / (dnpe[i + 1] - dnpe[i])*(derp - dnpe[i]));
//      dCe1 = i;
//      dCe2 = i + 1;
//    }
//  }
//  
//  R[1] = u1*du1;
//  SR[1] = DCS[Ce1][dCe1];
//  R[2] = u1*du2;
//  SR[2] = DCS[Ce1][dCe2];
//  R[3] = u2*du1;
//  SR[3] = DCS[Ce2][dCe1];
//  R[4] = u2*du2;
//  SR[4] = DCS[Ce2][dCe2];
//  
//  de2du = R[1]*CM[SR[1]] + R[2]*CM[SR[2]] +R[3]*CM[SR[3]]+R[4]*CM[SR[4]];
//  um = um + alfa*de2du;
//  //mandar valor de um a serial
//  
//  return um;
//  
//}
