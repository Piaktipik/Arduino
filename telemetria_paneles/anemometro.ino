  // ----------------------------------------------------------- anemometro -----------------------------------------------------------
  
void medir_anemometro(){
  /* convierto a velocidad
    1 pulso/segundo = 2.5 milla/hora 
    1 milla/hora -> 1.60934 kilometros/hora
    1 kilometro/hora -> 0.277778 metros/segundo 
  */
  anemometro = (1000000.0/tanemometro)*1,117597;
  if((tantes + 10000000) < micros()){
    tanemometro = 1000000000;
    tantes = micros();
  }
  
  // los combierto en string
  s_anemometro = String(int(anemometro*100));
  
  if(s_anemometro.length()<4){s_anemometro = '0' + s_anemometro;}else{return;}
  if(s_anemometro.length()<4){s_anemometro = '0' + s_anemometro;}else{return;}
  if(s_anemometro.length()<4){s_anemometro = '0' + s_anemometro;}else{return;}
  if(s_anemometro.length()<4){s_anemometro = '0' + s_anemometro;}else{return;}
  
}  

// -------- cuenta el tiempo entre pulsos -----------
void medir_tanemometro(){
  tanemometro = micros() - tantes;    // calculo el tiempo pasado entre la ultima medicion y la actual
  tantes = micros();                  // guardo el tiempo de la ultima medicion
}

// -------- muestro datos anemometro -----------
void mostrar_anemometro(){
  //Serial.print(" velocidad = "); 
  Serial.print(s_anemometro);
  //Serial.print(" m/s ");
}
