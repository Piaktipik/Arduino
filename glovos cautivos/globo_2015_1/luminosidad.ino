
// se toma la madida del sensor de luz
void medir_luz(){
  luminosidad = analogRead(lumpin);
  
  s_luminosidad = String(int(klum*luminosidad*100));
  
  if(s_luminosidad.length()<4){s_luminosidad = '0' + s_luminosidad;}else{return;}
  if(s_luminosidad.length()<4){s_luminosidad = '0' + s_luminosidad;}else{return;}
  if(s_luminosidad.length()<4){s_luminosidad = '0' + s_luminosidad;}else{return;}
  if(s_luminosidad.length()<4){s_luminosidad = '0' + s_luminosidad;}else{return;}
}

// -------- muestro datos luminosidad -----------
void mostrar_lum(){
  Serial.print("<lu/>");
  Serial.print("l");
  Serial.print(s_luminosidad);
  Serial.print("</lu>");
}
