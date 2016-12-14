
  //---------------Controlador------------
//  if ((Medida<249) && (Medida>256)){
//    parar();}
//  else{
//    controlador = Control_PID();
//    if (millis() > tiempo + 50){
//      if((controlador) > 255){vel1 = 255;}
//        else {vel1 = abs(controlador);}
//        if (error>0) { adelante();}
//        if (error<0) { atras();}
//      tiempo = millis();
//      }
//  }


float Control_PID(){
  return kp*(tv*s + h/(tn) + 1);
}

void calcular_error(){
  // calculo -> cambio de tiempo entre calculo de errores
  del_t = (timeStep * 1000)-(timer1-timer);
//  tiempo_s = millis();           // actualizo tiempo
//  del_t = (del_t/1000.0);        // pongo el delta de tiempo en segundos
  
  //vel1 = (vel1/255)*255; // escalo la velocidad medida a 0 a 255
  
  // calculo el error (deseado - real)
  error = SetP - Medida;
  
  // calculo s
  s = (error-error_p)/del_t;
  error_p = error; // actualizo el error_p
  h = error * del_t;
//  Serial.print("del_t:");
//  Serial.print(del_t);
//  Serial.print("\t s:");
//  Serial.print(s);
//  Serial.print("\t error:");
//  Serial.print(error);
} 

void conf_controlador(){
    kp = 2;
    tn = 1.6;
    tv = 0.07;
  }
