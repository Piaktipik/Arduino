
// esta funcion se usa para leer la direccion de los modulos xbee desde el arduino
String leer_dir_xbee(){
    String IDe;
    byte c = 0;                                           // contador de espera
    xbee:
    parpadear(100,5);                                    // parpadeo 5 veces rapidamente antes de iniciar de nuevo
    //delay(500);
    lserial(); 
    Serial.print("+");                                   // entro en modo AT
    delay(20);
    Serial.print("+");
    delay(20);
    Serial.print("+");
    delay(20);
    while(Serial.available() < 1){                       // si no he recivido almenos 2 valores -> espero (OK)
      if(c>200){c=0;goto xbee;}else{delay(10);c=c+1;}    // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
    }
    delay(50);                                           // espero un instante
    lserial();                                           // elimino el OK del buffer y otros posibles datos
    
    Serial.write("ATSH\r");delay(50);                    // solicito direcion parte alta
    Serial.write("ATSL\r");delay(50);                    // solicito direcion parte baja
    Serial.write("ATCN\r");                              // cierro el modo AT
    
    c = 0;                                               // reinicio el contador de retardos de 10ms
    while(Serial.available() < 14) {                     // reviso si llego la dirrecion (16 bits) cada 10ms
      if(c>150){c=0;goto xbee;}else{delay(10);c=c+1;}    // si demora mas de 1500ms reintenta la comunicacion
    }
    
    c = 0;                                               // reinicio el contador de retardos de 10ms
    for(int i=0;i<16;i++){
      IDe = IDe + char(Serial.read());                   // armo el ID del xbee
    }
    
    IDe = limpiar_string(IDe);                           // limpio el ID de caracteres especiales
    if (IDe.length() < 14){goto xbee;}
    if (IDe.length() < 15){IDe = "0" + IDe;}
    if (IDe.length() < 16){IDe = "0" + IDe;}
    
    parpadear(1000,1);                                    // parpadeo 1 vez lentamente para indicar conexion con xbee
    return IDe;
}

// ------------ limpio el puerto serial -------------
void lserial(){
  while(Serial.available() > 0){Serial.read();}           // si hay datos en el buffer, los quito
}

// ---- limpio una cadena de strings de: saltos de lineas, enter, espacios, tabulaciones ----
  
String limpiar_string(String IDl){
  String auxs = "";                                       // creo una varialbe auxiliar                
  for(unsigned int i=0; i<IDl.length();i++){                        // recorro el vector ID
    if(IDl.charAt(i) =='\n' || IDl.charAt(i) =='\r' || IDl.charAt(i) ==' ' || IDl.charAt(i) =='\t'){}
    else{auxs = auxs + IDl.charAt(i);}                     // si el caracter es especial lo descarto, de lo contrario lo armo.
  }
  return auxs;                                            // debuelvo el valor
}


// obtener velocidad - codigo xbee
long cod2vel(int cod){
  long vel[]={1200,2400,4800,9600,19200,38400,57600,115200};
  if(cod > 8){
    return 9600;
  }
  return vel[cod];
} 

