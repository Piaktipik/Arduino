

void inicio_sensores(){
  // ----------- infrarrojos de pista ----------- 
  // declaro puertos de control del mosfet de los leds como salida
  pinMode(sen_lineae1,OUTPUT);
  pinMode(sen_lineae2,OUTPUT);
  digitalWrite(sen_lineae1,1);
  digitalWrite(sen_lineae2,1);
}

// ------------------------------------- mido ultrasonidos ------------------------------------- 

void medir_ultrasonidos(){ 
  // --------- mido los sensores de ultrasonido ---------> minimo cada 60ms
  if((aux_tiem_medi_ultr + 20) < millis()){
    // prueba de los ultrasonidos
    if(cont_orde_ultr==0){srf05_3();}
    if(cont_orde_ultr==1){srf05_2();}
    if(cont_orde_ultr==2){srf05_1();}
    cont_orde_ultr = (cont_orde_ultr+1)%3;
    //if(cont_orde_ultr>=3){cont_orde_ultr=0;}
    aux_tiem_medi_ultr = millis(); // actualizo el contador
  }
}

// -------------------------------- mido sensores de pista y vela  ----------------------------

void medir_pista_vela(){
  // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
  if((aux_tiem_medi_infr + 20) < millis()){
    medirlinea();
    svela();
    aux_tiem_medi_infr = millis(); // actualizo el contador
  }
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz sub- funciones sensores zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
  void delayy(int tiempoaesperar){
    int contadort = 0;
    while (contadort<tiempoaesperar){
      if(contadort % 20 == 1){
      // prueba de los ultrasonidos
      if(cont_orde_ultr==0){srf05_1();}
      if(cont_orde_ultr==1){srf05_2();}
      if(cont_orde_ultr==2){srf05_3();}
      cont_orde_ultr = (cont_orde_ultr+1)%3;
      }
    // --------- mido los sensores de pista y vela ---------> minimo cada 60ms 
      if(contadort % 10 == 1){
        medirlinea();
        svela();
      }
      
      delay(10);
      contadort += 10;
      // muestro datos mietras espero 
      if(tipoc == 2){
        mostrar_processing();
      }
      if(tipoc == 1){
        mostrar_serial();
      }
    }
  }
  
  // funcion que se usa para ir al espacio con mayor distancia
void escapar(){
  if(medi_dist_ultr_dere <= medi_dist_ultr_izqu){izquierda();}
  else{derecha();}
}
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz ultrasonidos zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 

void srf05_1(){
  detachInterrupt(pin_ultr_1);
  medi_time_ultr_1 = 0;//medi_dist_ultr_dere = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_1, OUTPUT);digitalWrite(pin_ultr_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_1, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_1, INPUT);
  auxi_inte_ultr_1 = false;attachInterrupt(pin_ultr_1, func_ultr_1, RISING);auxi_inte_ultr_1 = true;
}
void func_ultr_1(){
  if (auxi_inte_ultr_1){
    if (medi_time_ultr_1 == 0){medi_time_ultr_1 = micros();attachInterrupt(pin_ultr_1, func_ultr_1, FALLING);} 
    else{medi_time_ultr_1 = micros()-medi_time_ultr_1; medi_dist_ultr_dere = medi_time_ultr_1/58.30;} 
  }
}

void srf05_2(){
  detachInterrupt(pin_ultr_2);
  medi_time_ultr_2 = 0;//medi_dist_ultr_fron = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_2, OUTPUT);digitalWrite(pin_ultr_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_2, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_2, INPUT);
  auxi_inte_ultr_2 = false;attachInterrupt(pin_ultr_2, func_ultr_2, RISING);auxi_inte_ultr_2 = true;
}
void func_ultr_2(){
  if (auxi_inte_ultr_2){
    if (medi_time_ultr_2 == 0){medi_time_ultr_2 = micros();attachInterrupt(pin_ultr_2, func_ultr_2, FALLING);}
    else{medi_time_ultr_2 = micros()-medi_time_ultr_2; medi_dist_ultr_fron = medi_time_ultr_2/58.30;}
  }
}

void srf05_3(){  
  detachInterrupt(pin_ultr_3);
  medi_time_ultr_3 = 0;//medi_dist_ultr_izqu = 0; // inicio valor en donde empiezo a contar y distancia
  // se manda un pulso de 10us
  pinMode(pin_ultr_3, OUTPUT);digitalWrite(pin_ultr_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_ultr_3, LOW);
  // se espera el eco y se activan las interrupciones
  pinMode(pin_ultr_3, INPUT);
  auxi_inte_ultr_3 = false;attachInterrupt(pin_ultr_3, func_ultr_3, RISING);auxi_inte_ultr_3 = true;
}
void func_ultr_3(){
  if (auxi_inte_ultr_3){
    if (medi_time_ultr_3 == 0){medi_time_ultr_3 = micros();attachInterrupt(pin_ultr_3, func_ultr_3, FALLING);}
    else{medi_time_ultr_3 = micros()-medi_time_ultr_3; medi_dist_ultr_izqu = medi_time_ultr_3/58.30;}
  }
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void medirlinea(){
  // enciendo leds
  
//  digitalWrite(sen_lineae1,1);   //*
//  digitalWrite(sen_lineae2,1);

  // cargo los condesandores poniendo en alto los pines
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],OUTPUT);digitalWrite(sen_linea1[i],HIGH);
    pinMode(sen_linea2[i],OUTPUT);digitalWrite(sen_linea2[i],HIGH);
  }
  // espero 10us a que cargue el capacitor
  delayMicroseconds(20);
  // pongo los pines como entrada y espero a la interrupcion
  for(int i=0;i<nsen;i++){
    pinMode(sen_linea1[i],INPUT);
    tiem_refl_1[i] = micros();medi_refl_1[i]=6000;medi_bool_line[i]=LOW;
    pinMode(sen_linea2[i],INPUT);
    tiem_refl_2[i] = micros();medi_refl_2[i]=6000;medi_bool_line[i+nsen]=LOW;
  }
  // activo las interupciones de los de pista s1
  attachInterrupt(sen_linea1[0],s1p0,FALLING);
  attachInterrupt(sen_linea1[1],s1p1,FALLING);
  attachInterrupt(sen_linea1[2],s1p2,FALLING);
  attachInterrupt(sen_linea1[3],s1p3,FALLING);
  attachInterrupt(sen_linea1[4],s1p4,FALLING);
  attachInterrupt(sen_linea1[5],s1p5,FALLING);
  attachInterrupt(sen_linea1[6],s1p6,FALLING);
  attachInterrupt(sen_linea1[7],s1p7,FALLING);
  // activo las interupciones de los de pista s2
  attachInterrupt(sen_linea2[0],s2p0,FALLING);
  attachInterrupt(sen_linea2[1],s2p1,FALLING);
  attachInterrupt(sen_linea2[2],s2p2,FALLING);
  attachInterrupt(sen_linea2[3],s2p3,FALLING);
  attachInterrupt(sen_linea2[4],s2p4,FALLING);
  attachInterrupt(sen_linea2[5],s2p5,FALLING);
  attachInterrupt(sen_linea2[6],s2p6,FALLING);
  attachInterrupt(sen_linea2[7],s2p7,FALLING);
  // apago leds
//  digitalWrite(sen_lineae1,0);
//  digitalWrite(sen_lineae2,0);
}


void s1p0(){medi_refl_1[0] = micros()-tiem_refl_1[0];if(medi_refl_1[0] > valo_refe_line1[0]){medi_bool_line[0]=LOW;}else{medi_bool_line[0]=LOW;}}  // desactivado
void s1p1(){medi_refl_1[1] = micros()-tiem_refl_1[1];if(medi_refl_1[1] > valo_refe_line1[1]){medi_bool_line[1]=LOW;}else{medi_bool_line[1]=LOW;}}  // desactivado
void s1p2(){medi_refl_1[2] = micros()-tiem_refl_1[2];if(medi_refl_1[2] > valo_refe_line1[2]){medi_bool_line[2]=HIGH;}else{medi_bool_line[2]=LOW;}}
void s1p3(){medi_refl_1[3] = micros()-tiem_refl_1[3];if(medi_refl_1[3] > valo_refe_line1[3]){medi_bool_line[3]=HIGH;}else{medi_bool_line[3]=LOW;}}
void s1p4(){medi_refl_1[4] = micros()-tiem_refl_1[4];if(medi_refl_1[4] > valo_refe_line1[4]){medi_bool_line[4]=HIGH;}else{medi_bool_line[4]=LOW;}}
void s1p5(){medi_refl_1[5] = micros()-tiem_refl_1[5];if(medi_refl_1[5] > valo_refe_line1[5]){medi_bool_line[5]=HIGH;}else{medi_bool_line[5]=LOW;}}
void s1p6(){medi_refl_1[6] = micros()-tiem_refl_1[6];if(medi_refl_1[6] > valo_refe_line1[6]){medi_bool_line[6]=HIGH;}else{medi_bool_line[6]=LOW;}}
void s1p7(){medi_refl_1[7] = micros()-tiem_refl_1[7];if(medi_refl_1[7] > valo_refe_line1[7]){medi_bool_line[7]=HIGH;}else{medi_bool_line[7]=LOW;}}

void s2p0(){medi_refl_2[0] = micros()-tiem_refl_2[0];if(medi_refl_2[0] > valo_refe_line2[0]){medi_bool_line[8]=HIGH;}else{medi_bool_line[8]=LOW;}}
void s2p1(){medi_refl_2[1] = micros()-tiem_refl_2[1];if(medi_refl_2[1] > valo_refe_line2[1]){medi_bool_line[9]=HIGH;}else{medi_bool_line[9]=LOW;}}
void s2p2(){medi_refl_2[2] = micros()-tiem_refl_2[2];if(medi_refl_2[2] > valo_refe_line2[2]){medi_bool_line[10]=HIGH;}else{medi_bool_line[10]=LOW;}}
void s2p3(){medi_refl_2[3] = micros()-tiem_refl_2[3];if(medi_refl_2[3] > valo_refe_line2[3]){medi_bool_line[11]=HIGH;}else{medi_bool_line[11]=LOW;}}
void s2p4(){medi_refl_2[4] = micros()-tiem_refl_2[4];if(medi_refl_2[4] > valo_refe_line2[4]){medi_bool_line[12]=HIGH;}else{medi_bool_line[12]=LOW;}}
void s2p5(){medi_refl_2[5] = micros()-tiem_refl_2[5];if(medi_refl_2[5] > valo_refe_line2[5]){medi_bool_line[13]=HIGH;}else{medi_bool_line[13]=LOW;}}
void s2p6(){medi_refl_2[6] = micros()-tiem_refl_2[6];if(medi_refl_2[6] > valo_refe_line2[6]){medi_bool_line[14]=LOW;}else{medi_bool_line[14]=LOW;}}  // desactivado
void s2p7(){medi_refl_2[7] = micros()-tiem_refl_2[7];if(medi_refl_2[7] > valo_refe_line2[7]){medi_bool_line[15]=LOW;}else{medi_bool_line[15]=LOW;}}  // desactivado


//void s1p0(){medi_refl_1[0] = micros()-tiem_refl_1[0];if(medi_refl_1[0] < valo_refe_line1[0]){medi_bool_line[0]=LOW;}else{medi_bool_line[0]=LOW;}}  // desactivado
//void s1p1(){medi_refl_1[1] = micros()-tiem_refl_1[1];if(medi_refl_1[1] < valo_refe_line1[1]){medi_bool_line[1]=LOW;}else{medi_bool_line[1]=LOW;}}  // desactivado
//void s1p2(){medi_refl_1[2] = micros()-tiem_refl_1[2];if(medi_refl_1[2] < valo_refe_line1[2]){medi_bool_line[2]=HIGH;}else{medi_bool_line[2]=LOW;}}
//void s1p3(){medi_refl_1[3] = micros()-tiem_refl_1[3];if(medi_refl_1[3] < valo_refe_line1[3]){medi_bool_line[3]=HIGH;}else{medi_bool_line[3]=LOW;}}
//void s1p4(){medi_refl_1[4] = micros()-tiem_refl_1[4];if(medi_refl_1[4] < valo_refe_line1[4]){medi_bool_line[4]=HIGH;}else{medi_bool_line[4]=LOW;}}
//void s1p5(){medi_refl_1[5] = micros()-tiem_refl_1[5];if(medi_refl_1[5] < valo_refe_line1[5]){medi_bool_line[5]=HIGH;}else{medi_bool_line[5]=LOW;}}
//void s1p6(){medi_refl_1[6] = micros()-tiem_refl_1[6];if(medi_refl_1[6] < valo_refe_line1[6]){medi_bool_line[6]=HIGH;}else{medi_bool_line[6]=LOW;}}
//void s1p7(){medi_refl_1[7] = micros()-tiem_refl_1[7];if(medi_refl_1[7] < valo_refe_line1[7]){medi_bool_line[7]=HIGH;}else{medi_bool_line[7]=LOW;}}
//
//void s2p0(){medi_refl_2[0] = micros()-tiem_refl_2[0];if(medi_refl_2[0] < valo_refe_line2[0]){medi_bool_line[8]=HIGH;}else{medi_bool_line[8]=LOW;}}
//void s2p1(){medi_refl_2[1] = micros()-tiem_refl_2[1];if(medi_refl_2[1] < valo_refe_line2[1]){medi_bool_line[9]=HIGH;}else{medi_bool_line[9]=LOW;}}
//void s2p2(){medi_refl_2[2] = micros()-tiem_refl_2[2];if(medi_refl_2[2] < valo_refe_line2[2]){medi_bool_line[10]=HIGH;}else{medi_bool_line[10]=LOW;}}
//void s2p3(){medi_refl_2[3] = micros()-tiem_refl_2[3];if(medi_refl_2[3] < valo_refe_line2[3]){medi_bool_line[11]=HIGH;}else{medi_bool_line[11]=LOW;}}
//void s2p4(){medi_refl_2[4] = micros()-tiem_refl_2[4];if(medi_refl_2[4] < valo_refe_line2[4]){medi_bool_line[12]=HIGH;}else{medi_bool_line[12]=LOW;}}
//void s2p5(){medi_refl_2[5] = micros()-tiem_refl_2[5];if(medi_refl_2[5] < valo_refe_line2[5]){medi_bool_line[13]=HIGH;}else{medi_bool_line[13]=LOW;}}
//void s2p6(){medi_refl_2[6] = micros()-tiem_refl_2[6];if(medi_refl_2[6] < valo_refe_line2[6]){medi_bool_line[14]=LOW;}else{medi_bool_line[14]=LOW;}}  // desactivado
//void s2p7(){medi_refl_2[7] = micros()-tiem_refl_2[7];if(medi_refl_2[7] < valo_refe_line2[7]){medi_bool_line[15]=LOW;}else{medi_bool_line[15]=LOW;}}  // desactivado

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de vela zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void svela(){
  Serial1.print("R");
  Serial1.print("L");
}

void serialEvent1(){
  // Serial1.read();
  for(int i=0;i<11;i++){
    senvela[i] = Serial1.read();
  }
 while(Serial1.available()>0){Serial1.read();}  
}

void serialEvent2(){
  for(int i=11;i<20;i++){
    senvela[i] = Serial2.read();
  }
 while(Serial2.available()>0){Serial2.read();}  
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz encoders zzzzzzzzzzzzzzzzzzzzzzzzzzzzz
// incremento los respectivos contadores de los encoders
void in_enco1a(){
  const float dist_paso = 0.1;
  vel_med1 = dist_paso/((micros()-t_enc1)); // velocidad en milisegundos
  if(a1){enco1++;}
  else{enco1--;}
  t_enc1 = micros();
}

void in_enco2a(){
  const float dist_paso = 0.1;
  vel_med2 = dist_paso/((micros()-t_enc2)); // velocidad
  if(a2){enco2++;}
  else{enco2--;}
  t_enc2 = micros();
}

//void calcular_desplazamiento(){
//  const int pulsos = 484; // numero de pulsos en un giro de 360
//  const float diametro = 69.3; // diametro en milimetros
//  //float dist1=0;
//  dist1 += PI*(diametro)*(float(enco1)/pulsos);
//  enco1 = 0;
//  
//  dist2 += PI*(diametro)*(float(enco2)/pulsos);
//  enco2 = 0;
//}
