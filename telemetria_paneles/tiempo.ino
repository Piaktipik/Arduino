
 // funcion que optiene el tiempo y lo formatea para ser enviado

String obt_tiempo() {
  String tiemp = "";
  int aux = year() - 2000;
  tiemp = String(aux);
  if(String(month()).length() == 1){tiemp +='0';}
  tiemp += month();
  if(String(day()).length() == 1){tiemp +='0';}
  tiemp += day();
  if(String(hour()).length() == 1){tiemp +='0';}
  tiemp += hour();
  if(String(minute()).length() == 1){tiemp +='0';}
  tiemp += minute();
  if(String(second()).length() == 1){tiemp +='0';}
  tiemp += second();
  
  return tiemp;
}

//String obt_tiempo() {
//  String tiemp = "";
//  int aux = year() - 2000;
//  tiemp = tiemp + char(aux);
//  tiemp = tiemp + char(month());
//  tiemp = tiemp + char(day());
//  tiemp = tiemp + char(hour());
//  tiemp = tiemp + char(minute());
//  tiemp = tiemp + char(second());
//  return tiemp;
//}
