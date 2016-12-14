void accion_serial(){
  // String dato = Serial.readBytesUntil('\r');
  // Serial.println(dato);
  char nomb;
  int c = 0;                                       // contador de espera
  nomb = char(Serial.read());                      // guardo el dato que llego en nomb
  
  if (nomb == 'B'){                                // si llega un dato de la base
    c = 0;                                         // reinicio el contador de 10ms  
    while(Serial.available() < 0){                 // espero el caracter que sigue
      if(c>10){c=0;return;}else{delay(10);c=c+1;}  // si demora mas de 100ms -> reinicio el loop
    } // llego el dato
    nomb = char(Serial.read());                    // leo el dato que llego
    if(nomb == 'O' || nomb == 'C' || nomb == 'T' || nomb == 'L'|| nomb == 'M'){}  // pregunto si es valido
    else{return;}                                  // si el caracter no es valido -> dato erroneo -> salgo del ciclo
    // dato valido
    // ---------- tomo la direccion de destino ------------
    nom = leer_direccion();
//    // -------------------  
    Serial.print(nom);                             // imprimo la direccion de destino (depuracion)
//    // -------------------
    Serial.print("(0) respuesta: \r"); 
    // -> para quien es?
    if((nom == ID)||(nom == "00000000000000")){    // pregunto si la direccion es la de el globo y ejecuto la accion correspondiente
      if(nomb == 'O'){                             // si es O: (confirmacion dato recibido)
        confirmacion_dato();
      }
      if(nomb == 'C'){                             // si es C: (configuracion pines)
        configurar_pin();
      }
      if(nomb == 'T'){                             // si es T: (configuracion del tiempo)
        configuracion_tiempo();
      }
      if(nomb == 'L'){                             // si es L: (socilicitud lectura pin)
        leer_pin();
      }
      if(nomb == 'M'){                             // si es M: (configuracion tiempo de muestreo)
        tiempo_muestreo();
      }
    }// fin if el mensaje es para este globo
  }lserial();// fin if B
  
}

// ------------------------------------------- 
String leer_direccion(){
  String dir = "";
  int c = 0;                                     // reinicio el contador de 10ms
  while(Serial.available() < 16){                // leo la direccion de destino del mensaje
    if(c>10){c=0;return "F";}else{delay(10);c = c+1;} // si demora mas de 100ms -> reinicio el loop
  }
  for(int i=0;i<16;i++){                         // recorro el bufer (leo mas de los 16 -> espacio intermedio)                  
    dir = (dir + char(Serial.read()));           // leo cada byte de la direccion y la armo        
  }
  //nom = limpiar_string(dir);                   // limpio la direccion de destino
  return dir;
}

// ------------------------------------------- O (confirmacion dato recibido) ------------------------------------------ 
void confirmacion_dato(){
  String aux_tiempo = "";
  String aux_tiempo_ult = "";
  int c = 0;                                                     // reinicio el contador de espera
  // año,mes,dia,hora,min,seg,F
  while(Serial.available() < 12){                                // leo la hora y el caracter de finalizacion
    if(c>500){c=0;lserial();return;}else{delay(1);c=c+1;}        // si demora mas de 100ms -> se cancela el comando
  }
  for(int i=0;i<12;i++){                                         // recorro el buffer                   
    aux_tiempo = (aux_tiempo + char(Serial.read()));             // armo el vector de tiempo  
  }
  // pregunto si el ultimo caracter es F
  if(Serial.read() != 'F'){lserial();return;}                    // si no es F el paquete es invalido -> salgo de la funcion
  // si el paquete es valido comparo la hora del ultimo valor y si coincide paso a enviar el dato siguiente
  // tomo el valor de tiempo del ultimo valor enviado
  for(int i=0;i<12;i++){                                         // recorro el buffer                   
    aux_tiempo_ult = (aux_tiempo_ult + vector_salida[vec_i][i]); // leo el tiempo        
  }
  // pregunto si son el mismo valor el enviado y el recivido
  if(aux_tiempo == aux_tiempo_ult){
    if (vec_i != vec_f){                                         // verifico que el valor no a enviar no sea el ultimo (evita que el indice de salida supere el de llenado)
      vec_i = (vec_i + 1)%(bufsalida-1);                             // aumento el indice del buffer de salida
//      Serial.print("vec_i: ");
//      Serial.println(vec_i);
    }
  }
}

// ------------------------------------------ T (configuracion del tiempo) ------------------------------------------ 
void configuracion_tiempo(){                               // si es T: configuro el tiempo actual
  String tiempo = "";                                      // variable auxiliar para la configuracion del puerto
  int c = 0;                                               // reinicio el contador de espera
  while(Serial.available() < 12){                          // leo la hora y el caracter de finalizacion
    if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}  // si demora mas de 100ms -> reinicio el loop
  }
  for(int i=0;i<12;i++){                                   // recorro el buffer                   
    tiempo += char(Serial.read());                         // leo el tiempo y guardo su valor byte     
  }
  // pregunto si el ultimo caracter es F
  if(Serial.read() != 'F'){lserial();return;}              // si no es F el paquete es invalido -> salgo de la funcion
    // si el paquete es valido configuro la hora actual
    setTime(est_tie(tiempo,6,2),est_tie(tiempo,8,2),est_tie(tiempo,10,2),est_tie(tiempo,4,2),est_tie(tiempo,2,2),est_tie(tiempo,0,2));  // hora, minutos, segundos, dia, mes, añó.
    // envio mensaje indicando que la hora ha sido configurada
    Serial.print("GT" + ID + "F");
  }
  
// funcion usada para extraer los numeros del vector de tiempo enviado 
int est_tie(String strvec, int ini, int tam){      // se toma el string a analizar, el valor a partir del cual tomar datos y la cantidad de datos
  String valacu = "";
  for(int i=ini;i<(ini+tam);i++){ 
    valacu += strvec[i];
  } 
  return valacu.toInt();
}

// ------------------------------------------ C (configuracion pines) ------------------------------------------ 
void configurar_pin(){                                            // si es C: configuro el tiempo actual
  byte c = 0;                                                     // reinicio el contador de espera
  byte c1 = 0;                                                    // guarda el valor a poner en el puerto
  byte ult = 0;
  while(Serial.available() < 2){                                  // leo los parametros, pin y valor
    if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
  }
  c = byte(Serial.read());                                        // leo el pin a configurar
  c1 = byte(Serial.read());                                       // leo el valor a poner el pin
  ult = Serial.read();
  // pregunto si el ultimo caracter es F
  if(char(ult) != 'F'){lserial();return;}                         // si no es F el paquete es invalido -> salgo de la funcion
  // si el paquete es valido hago el cambio y envio confirmacion
  pinMode(c,OUTPUT);
  if (c1 == 255 || c1 == 0) digitalWrite(c,c1);
  else analogWrite(c,c1);
  // envio mensaje indicando que la hora ha sido configurada
  Serial.print("GC" + ID + "F");
}


// ------------------------------------------ L (socilicitud lectura pin) ------------------------------------------ 
void leer_pin(){                                                  // si es L: envio el valor de el pin solicitado
  bool c1 = 0;                                                    // valor boolenao leido del puerto
  word c2 = 0;                                                    // valor analogo leido del puerto
  byte c = 0;                                                     // reinicio el contador de espera
  while(Serial.available() < 1){                                  // leeo el pin a leer y si es lectura analoga o digital
    if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
  }
  c = byte(Serial.read());                                        // leo el pin a leer si es >= 200 -> analogo  
  // pregunto si el ultimo caracter es F
  if(Serial.read() != 'F'){lserial();return;}                     // si no es F el paquete es invalido -> salgo de la funcion
  // si el paquete es valido procedo a enviar el valor de el pin
  // envio mensaje con el valor de el pin
  if (c >= 200){
    c -= 200;
    c2 = analogRead(c);
    Serial.print("GL" + ID + obt_tiempo() + char(bytesh(c2)) + char(bytesl(c2)) + "F");
  }
  else{
    pinMode(c,INPUT);
    c1 = digitalRead(c);
    Serial.print("GL" + ID + obt_tiempo() + char(c1) + "F");
  }
}


// ------------------------------------------ M (configuracion tiempo de muestreo) ------------------------------------------ 
void tiempo_muestreo(){                                            // si es M: configuro el tiempo actual
  byte tiempo;
  byte c = 0;                                                      // reinicio el contador de espera
  while(Serial.available() < 1){                                   // leo tiempo muestreo y el caracter de finalizacion
    if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}          // si demora mas de 100ms -> reinicio el loop
  }
  tiempo = Serial.read();                                          // leo el tiempo y guardo su valor byte
  // pregunto si el ultimo caracter es F
  if(Serial.read() != 'F'){lserial();return;}                      // si no es F el paquete es invalido -> salgo de la funcion
  // si el paquete es valido configuro el tiempo de muestreo
  parametros(tiempo);
  tmuestreo = leer_tie();
  if(tmuestreo == 0){tmuestreo = 1;}
  // envio mensaje indicando que la hora ha sido configurada
    Serial.print("GM" + ID + "F");
  }
            // -------------------- fin M -------------------- 

