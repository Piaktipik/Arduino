

void imu_razer(){
  
//  unsigned long time = 0;
//  char aux[] ={' ',' ',' ',' ',' '};  // inicalizo el vector de lectura de datos
//  double aux1 = 0;
//  time = micros();
//  while(Serial3.available() == 4 && (time < (micros()+15000)) ){}
//  if (Serial3.available() != 4){limpiar_Serial3();return;}
//  
//  // leo lo que llego por serial
//  for(int i=0;i<4;i++){
//    aux[i] = Serial3.read();
//  }
//  // si el ultimo valor es
//  if (aux[4] == '\r'){
//    aux1 = float(aux[0]+aux[1])+(float(aux[2]+aux[3])/100.0);
//    
//  }else{
//    limpiar_Serial3();
//  }

//  if (Serial3.available() > 4) {
//    Serial.readBytesUntil('\n', aux, 5);
//  }
  
  //Serial.print(roll);
  //Serial.print(aux[0]);
  //Serial.print('\n');
   //String aux = Serial.findUntil(4,'\n');
  
  //double aux = Serial3.parseInt();
  //delay(1);
//  while(Serial3.available() > 0){
//    if( char(Serial3.read()) == 'D'){
  
    double aux = Serial3.parseFloat();
    
    if (aux != 0.00){
      
//      if (aux > 0){
//        if ((aux > (roll*0.6)) && (aux < (roll*1.5))){
//          
//        }
//      }
//      else{
//        if ((aux > (roll*1.5)) && (aux < (roll*0.6))){
//          roll = aux;
//        }
//      }

      
      roll = aux/90.0;
//      
//      //roll = float(Serial3.parseInt()) + float(Serial3.parseInt())/100.0;
      Serial.print(roll);
      Serial.println();
//      return;
    }
//  }
  
  //yaw = Serial3.parseFloat();
  //pitch = Serial3.parseFloat();
  //
    
  //}
  // imprimo los valores para comprobar comunicacion
}

void limpiar_Serial3(){
  unsigned long time = micros();
  while(Serial3.available() >0 && (time < (micros()+5000)) ){
    if ( char(Serial3.read()) == '\r'){return;}
  }
}


//void mostrar_razer(){
//  //Serial.print(yaw);
//  //Serial.print(",");
//  //Serial.print(pitch);
//  //Serial.print(",");
//  Serial.print(roll);
//  Serial.println();
//  // imprimo los valores para comprobar comunicacion
//  
//}

