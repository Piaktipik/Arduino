boolean estador[5]={1,1,1,1,1};
boolean estadora[5];
boolean estadop[25];
boolean estadopa[25];
boolean encoder[9];
int dato,i=0;
boolean secuancia=false;
unsigned int t1=0,t2=0;

void setup() {
  Serial.begin(9600);                                                                                        
  establishContact();  // send a byte to establish contact until Processing responds
  //---------------declaro la salida de los relevos----------
  for(i=8;i<13;i++){
  pinMode(i, OUTPUT);} 
 
//  for(i=22;i<48;i++){
//  pinMode(i, OUTPUT);}   
}

void loop() {
  //t1 = millis();
  //zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
  // aqui pongo la secunacia de los encoders
//  for(i=0;i<9;i++){
//    dato = analogRead(i);
//    if(dato > 800){
//    encoder[i]=false;
//    }
//    else{
//    encoder[i]=true;
//    }
//}
//  
  
  
  
  
  
  //zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
  if(Serial.available() > 0 && secuancia==false){
  dato = Serial.read();
  if (dato=='P'){secuancia=true;}
  } 
    
  if(Serial.available() > 4 && secuancia){ 
    
    //------------------------------------------------------------------------------------
    
    for(i=0;i<5;i++){estadora[i] = estador[i];} //guardo los estados por si fallan los datos
    
    for(i=0;i<5;i++){
      dato = int(Serial.read()-48);  // leeo todo datos
      //Serial.print(dato);
      if (dato == 1 || dato == 0){  // los reviso
        estador[i] = dato;          // los guardo en mi vector
      }
      else{                         // si algun dato no es valido retablesco al estado anterior
        for(int j=0;j<5;j++){estador[i] = estadora[i];}
        //Serial.flush();             // limpio el serial
        secuancia = false;          // espero por una nueva p
        return;
      }
    }
    
////------------------------rutina para modo eficiente--------------------------
    
//      for(int j=0;j<25;j++){
//        for(int k=0;k<25;k++){estadopa[k] = estadop[k];} //guardo los estados por si fallan los datos
//        dato = int(Serial.read()-48);  // leeo todo datos
//        
//        if (dato == 1 || dato == 0){  // los reviso
//          estadop[j] = dato;          // los guardo en mi vector
//        }
//        else{                         // si algun dato no es valido retablesco al estado anterior
//          for(int k=0;k<25;k++){estadop[k] = estadopa[k];}
//          //Serial.flush();             // limpio el serial
//          secuancia = false;          // espero por una nueva p
//          return;
//        }    
//      }

      //t2 = millis();
    
    //if(int(Serial.read())== 80){}
    //else{for(int i=0;i<6;i++){estador[i]=estadora[i];}}
    //Serial.print('A');
    secuancia = false;
    //Serial.flush();
    
    //------------------------------------------------------------------------------------
    
  } // fin de la rutina cuando llegan los 5 datos de la posicion
  
  // pongo los estados en los puertos
  
  for(i=8;i<13;i++){
    digitalWrite(i,estador[i-8]);
  }
  //~
//  for(i=22;i<48;i++){
//    digitalWrite(i,estadop[i-22]);
//  }
  
  //if (t1 >(t2+5000)){setup();}
  //t1 = millis();  
}

//----------------------------me trato de conectar al arduino----------------------------

void establishContact() {
 while (Serial.available() <= 0) {
      Serial.write('A');   // send a capital A
      delay(50);
  }
  delay(500);
  //Serial.flush();
}
