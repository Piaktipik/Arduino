// ----------------------------------------------------------- chipcap -----------------------------------------------------------
  
void medir_chipcap(){
  // leo los valores analogos
  tem = analogRead(tempin);
  hum = analogRead(humpin);
  // lo paso a voltajes
  ftem = float(tem*5)/1023;
  fhum = float(hum*5)/1023;
  
  // modo radiometrico
  // %RH = ((Vout - (0.1 * Vsupply))/(0.8 * Vsupply)) * 100
  // Tc = ((Vout - (0.1 * Vsupply))/(0.8 * Vsupply)) * 200 - 50
  ftem = ((ftem-(0.5))/(4))*200-50;
  fhum = ((fhum-(0.5))/(4))*100;
  
  
  // los combierto en string
   s_tem = String(int(ftem*100));
   s_hum = String(int(fhum*100));
   
  if(s_tem.length()<4){s_tem = '0' + s_tem;}
  if(s_tem.length()<4){s_tem = '0' + s_tem;}
  if(s_tem.length()<4){s_tem = '0' + s_tem;}
  if(s_tem.length()<4){s_tem = '0' + s_tem;}
  if(s_hum.length()<4){s_hum = '0' + s_hum;}
  if(s_hum.length()<4){s_hum = '0' + s_hum;}else{return;}
  if(s_hum.length()<4){s_hum = '0' + s_hum;}else{return;}
  if(s_hum.length()<4){s_hum = '0' + s_hum;}else{return;}
} 


// -------- muestro datos chipcap -----------
void mostrar_chipcap(){
  Serial.print("<cp/>");
  Serial.print("t");
  Serial.print(s_tem);
  Serial.print("h");
  Serial.print(s_hum);
  Serial.print("</cp>");
}
