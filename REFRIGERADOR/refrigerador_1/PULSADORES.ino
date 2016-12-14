
//////////////////////////////////////// botones ////////////////////////////////////// 
const byte PIN_BOTONES = 3; // PIN ANALOGO DE LOS BOTONES


void mostrar_med_botones(){
  Serial.print("\t");
  Serial.print("BOT: "); 
  Serial.print(analogRead(PIN_BOTONES));
  Serial.print("\t");
  Serial.print("BOTP: "); 
  Serial.print(boton);
}


void revisar_botones(){
  int MEDI = analogRead(PIN_BOTONES);
  boton = 0;
  if(MEDI> 850 && MEDI < 950){boton = 3;}
  if(MEDI> 450 && MEDI < 550){boton = 2;}
  if(MEDI< 100){boton = 1;}
}
