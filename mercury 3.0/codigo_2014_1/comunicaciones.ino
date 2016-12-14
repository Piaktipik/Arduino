
void inicio_comunicaciones(){
  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas
  
  // ----------------- seriales sensores de vela ----------------- 
  Serial1.begin(57600);       // lado derecho
  Serial2.begin(57600);       // lado izquierda
}

// ------------------------------------- muestro datos por serial ------------------------------------- 
void mostrar_serial(){   
// ultrasonidos
  Serial.print(medi_dist_ultr_dere);
  Serial.print(',');
  Serial.print(medi_dist_ultr_fron);
  Serial.print(',');
  Serial.print(medi_dist_ultr_izqu);
  Serial.print(',');
  
  // infrarrojos pista
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_1[i]);
    Serial.print(',');
  }
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_2[i]);
    Serial.print(',');
  }
  for(int i=0;i<2*nsen;i++){
    Serial.print(medi_bool_line[i]);
    Serial.print(',');
  }
  
  
  
  // infrarrojos vela
  for(int i=0;i<20;i++){
    Serial.print(senvela[i]);
    Serial.print(',');
  } 
  
  // encoders
  Serial.print(enco1);
  Serial.print(',');
  Serial.print(enco2);
  
  // posicion del robot
  Serial.print(',');
  Serial.print(x_robot);
  Serial.print(',');
  Serial.print(y_robot);
  Serial.print(',');
  Serial.print(o_robot);
  
  Serial.print(",");
  Serial.print(vel_med1);
  Serial.print(",");
  Serial.print(dist1);
  Serial.print(",");
  Serial.print(dist2);
  
  // imprimo salto de linea
  Serial.println();
}
  
// ------------------------------------- muestro datos a processing ------------------------------------- 
void mostrar_processing(){
  // ultrasonidos
  Serial.print(medi_dist_ultr_dere);
  Serial.print(',');
  Serial.print(medi_dist_ultr_fron);
  Serial.print(',');
  Serial.print(medi_dist_ultr_izqu);
  Serial.print(',');
  
  // infrarrojos pista
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_1[i]);
    Serial.print(',');
  }
  for(int i=0;i<nsen;i++){
    Serial.print(medi_refl_2[i]);
    Serial.print(',');
  }
  
  // infrarrojos vela
  for(int i=0;i<20;i++){
    Serial.print(senvela[i]);
    Serial.print(',');
  } 
  
  // encoders
  Serial.print(enco1);
  Serial.print(',');
  Serial.print(enco2);
  
  // posicion del robot
  Serial.print(',');
  Serial.print(x_robot);
  Serial.print(',');
  Serial.print(y_robot);
  Serial.print(',');
  Serial.print(o_robot);
  
  // imprimo salto de linea
  Serial.println();
}
  
  
// ------------------------------------- actuo frente a datos por serial ------------------------------------- 
void datoserial(char dato){
    if(dato == 'a'){
      adelante();
    }
     if(dato == 'i'){
      izquierda();
    } 
    if(dato == 'd'){
      derecha();
    }
    if(dato == 'p'){
      parar();
    }
    if(dato == 'h'){
      atras();
    }
    if(dato == 'f'){
      fan(1);  // prendo el ventilador
    }
    if(dato == 'b'){
      buz(1);  // prendo el buzzer
    }
    if(dato == 'o'){
      fan(0);  // apago el ventilador
      buz(0);  // apago el buzzer
    }  
    if(dato == 'v'){
      delay(100);
      velo_moto_izqu = Serial.read(); // vel izquierda
      velo_moto_dere = Serial.read(); // vel derecha
      buz(1);
      delay(200);
      buz(0);
    }  
//    if(dato == 'c'){
//      delay(100);
//      dueFlashStorage.write(0,Serial.read());
//      dueFlashStorage.write(1,Serial.read());
//      ruid_vela = dueFlashStorage.read(0);
//      rlinea = int(dueFlashStorage.read(1))*100;
//      for(int i=0;i<nsen;i++){
//        valo_refe_line1[i] = rlinea;
//        valo_refe_line2[i] = rlinea;
//      }    
//      buz(1);
//      delay(200);
//      buz(0);
//    } 
    if(dato == 's'){
      delay(100);
      x_robot = Serial.parseFloat();
      y_robot = Serial.parseFloat();
      o_robot = Serial.parseFloat();
      
//      dueFlashStorage.write(2,Serial.read());  // x baja
//      
//      dueFlashStorage.write(3,Serial.read());  // y baja
//      dueFlashStorage.write(4,Serial.read());  // y alta
//      
//      dueFlashStorage.write(5,Serial.read());  // o baja
//      dueFlashStorage.write(6,Serial.read());  // o alta
//      
//      x_robot = float(dueFlashStorage.read(2));
//      y_robot = float(int(dueFlashStorage.read(4))*255 + dueFlashStorage.read(3))/100.0;
//      o_robot = float(int(dueFlashStorage.read(6))*255 + dueFlashStorage.read(5))*(PI/180.0)/100.0;
//      
//      buz(1);
//      delay(200);
//      buz(0);
    }
  }
