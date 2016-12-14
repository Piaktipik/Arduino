// por: Julian Galvez Serna
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ librerias ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

  #include <EEPROM.h>      // se usara para guardar valores de configuracion
  
  //-------------------------tiempo----------------------------------
  
  #include <Time.h>
  // hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Variables ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
//-------------------------------------vector de salida de datos---------------------------------
  const int bufsalida = 50;             // tamaño del buffer de salida 
  byte vector_salida[bufsalida][13];    // vector usado para guardar los datos generados mientras se envian
  // los uso para moverme a travez del vector de salida de datos
  byte vec_i=0;                    // puntero posicion inicial -> de donde se toma el dato a enviar
  byte aiv=0;                     // auxiliar error estraño en due
  byte vec_f=0;                    // puntero posision final -> en donde se guardan los nuevos datos
  bool fvr = false;
  //--------------------------------------otras variables---------------------------------------------
  unsigned long tmuestreo = 0;
  int tiempomu = 0;
  
  String ID = "";          // identificacion del globo -> serial del xbee
 
  
  //-------------------------------------variables analogas del entrono---------------------------------
  
  int chipcap1 = 0;        // variable sensor temperatura/humedad
  int chipcap2 = 0;        // variable sensor temperatura/humedad
  int luminosidad = 0;     // variable sensor intensidad luminosa
  
  //---------------------------variables para la lectrura del anemometro-------------------------------
  
  unsigned long tanemometro = 0;       // variable anemometro (tiempo entre pulsos)
  int anemometro = 0;
  unsigned long tantes = 0;          // tiempo a el pulso anterior
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ configuraciones iniciales ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

  void setup(){
    
    byte c;                      // contador usado para los retardos 0-255
    Serial.begin(9600);          // incializo serial
    
    xbee:
    //---------me conecto con el xbee y obtengo su numero serial, para usarlo como id del globo--------- 
    lserial();                                           // limpio el puerto serial
    delay(1000);                                         // espero un instante antes de iniciar la comunicacion con el xbee
    Serial.print("+++");                                 // entro en modo AT
    //delay(1000);                                       // espero un instante antes de iniciar la comunicacion con el xbee
    c = 0;
    while(Serial.available() < 1){                       // si no he recivido almenos 2 valores -> espero (OK)
      if(c>150){goto xbee;}else{delay(10);c=c+1;}    // reviso si envio el OK cada 1ms, si demora mas de 150ms reintenta la comunicacion 
    }    
    Serial.read();Serial.read();delay(50);               // elimino el OK del buffer y espero un instante
    lserial();
    Serial.write("ATSH\r");delay(50);                    // solicito direcion parte alta
    Serial.write("ATSL\r");delay(50);                    // solicito direcion parte baja
    Serial.write("ATCN\r");                              // cierro el modo AT
    c = 0;                                                 // reinicio el contador de retardos de 1ms
    while(Serial.available() < 15) {                     // reviso si llego la dirrecion (14 bits ?) cada 10ms
      if(c > 200){goto xbee;}else{delay(10);c = c+1;}     // si demora mas de 200ms reintenta la comunicacion
    }
    for(int i=0;i < 16;i++){
      ID = ID + char(Serial.read());                     // armo el ID del xbee
    }
    
    ID = limpiar_string(ID);                             // limpio el ID de caracteres especiales
    if (ID.length() < 15){ID = "0" + ID;}
    if (ID.length() < 16){ID = "0" + ID;}
    

    //--------------configuro el tiempo-------------------------
    // si el tiempo no ha sido configurado pido la hora a la base
    //setTime(23,59,50,22,10,12);                          // hora, minutos, segundos, dia, mes, añó.
    
    //--------------leeo el tiempo de muestreo-------------------------
    
    tiempomu = leer_tie();
    if(tiempomu==0 || tiempomu==-1){tiempomu = 100;}
    //---------------------------------------------------------------------------------------
    
    pinMode(13,OUTPUT);digitalWrite(13,LOW);             // apago el led superficila para ahorrar energia
    
    // configuracion anemometro
    pinMode(2, INPUT);                                   // configuro como entrada el pin del anemometro
    attachInterrupt(0, anemometro1, RISING);            // abilito interrupcion en pin 2 - acciono la funcion: anemometro1
    
    lserial();                                           // limpio el serial antes de empezar 
    
  } // fin configuracion
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ programa principal ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ 

  void loop(){
    String tem0 = "";
    String tem1 = "";
    String tem2 = "";
    //--------------inicializo el globo-------------------------
    ini:
    // si hay datos disponibles en el puerto serial lo reviso
    if (Serial.available() > 0) {serialdata();}
    // si no se ha configurado la hora se pide la hora
    if(timeStatus() == timeNotSet){Serial.print("GI" + ID + "F");delay(200);goto ini; }
    // si el tiempo requiere ser sincronizado envio el mensaje para sincronizarlo
    if(timeStatus() == timeNeedsSync){Serial.print("GI" + ID + "F");}
     
     //-------------------------------- leeo los sesores ---------------------------------
    // esta parte se ejecuta cada tiempomu*100 instantes de tiempo
    // 
    
    if((millis()>(tmuestreo+tiempomu*100)) && ((vec_f!=vec_i) || ((vec_f == vec_i) && !fvr))){
//       Serial.print("i1=");
//       Serial.print(vec_i);
       // porcedo a tomar poner el vector de salida un dato valio de los sensores y el tiempo de lectura
       tmuestreo = millis(); // ya muestre este periodo timepomu
       //vector_salida[bufsalida][13]
       tem0="";
       tem0 = obt_tiempo();
       for(int i=0;i<6;i++){
         vector_salida[vec_f][i] = tem0.charAt(i);
       }
       chipcap1 = analogRead(0);        // variable sensor temperatura
       chipcap2 = analogRead(1);        // variable sensor humedad
       luminosidad = analogRead(2);     // variable sensor intensidad luminosa
//       Serial.print(" i2=");
//       Serial.print(iv);  
       // tomo las medidas
       aiv = vec_i;
       
       vector_salida[vec_f][6]= bytesh(luminosidad);
       vector_salida[vec_f][7]= bytesl(luminosidad);
       vector_salida[vec_f][8]= bytesh(chipcap2);
       vector_salida[vec_f][9]= bytesl(chipcap2);
       vector_salida[vec_f][10]= bytesh(chipcap1);
       vector_salida[vec_f][11]= bytesl(chipcap1);
       //anemometro = (1000000/float(tanemometro))*111,76;
       vector_salida[vec_f][12]= bytesh(tanemometro);
       vector_salida[vec_f][13]= bytesl(tanemometro);
       
//       Serial.print(" i3=");
//       Serial.print(iv);
       
       vec_f = vec_f+1;                       // aumento el indice de llenado
       if((vec_f % bufsalida) == 0){
         vec_f = 0;
         fvr = true;
       }
       
       vec_i=aiv;           // soluciona un herror extraño?
//     Serial.print(" f=");
//     Serial.print(vec_f);
//     Serial.print("i4=");
//     Serial.println(vec_i);
     } 
     
     // si hay datos para enviar los envio
     //
     
     if((vec_f!=vec_i)||((vec_f==vec_i)&&fvr)){
       Serial.print("f=");
       Serial.print(vec_f);
       Serial.print(" i=");
       Serial.print(vec_i);
       // paso el vector a un arreglo de caracteres
       Serial.print(" vecs0= ");
       Serial.print(vector_salida[vec_i][0]);
       tem1 = "";
       for(int i=0;i<14;i++){
         tem1 += char(vector_salida[vec_i][i]);
       }
       //if((vec_f==vec_i)&&fvr){tem.charAt(0) = vector_salida[vec_i-1][0];} // correccion de otro error misterioso
       //  armo el dato
       Serial.print(" tem1 =");
       Serial.print(tem1); 
      
       tem2 = ("GD"+ ID + tem1 + "F"); 
       Serial.print(tem2);
       
       Serial.println();
       //Serial.print("GD" + ID + tem + "F");
     }
    delay(200);
    
    
    // filtro de datos anemometro
    // tamaño del espacio de muestras
//    int ndatos = 10;
//    int vec[ndatos];
//    int datonuevo;
//    int promedio; // promedio de los datos del anemometro
//    // rango maximo de diferencia pra ser valido
//    int variacion = 10;
    
    // filtro de datos anemometro
    /* convierto a velocidad
    1 pulse/second = 2.5 mph            // data sheet
    1 mph ->  0.44704 m/s
    2.5 mph -> 1,1176 m/s
    1 pulse/(anemometro/1000ms) = 1,1176 m/s
    lo multiplico por 10 para quitarle el punto decimal
    */
    
//    int datoanuevo;
//    datonuevo = 10,1176*(1000/anemometro); // calculo de el valor de anemometro

    // promedio = 0;
    //for(int i=0; i < ndatos ;i++){promedio = promedio + vec[i];}
    
    
//    promedio = int(promedio / ndatos);
//    if(promedio-variacion < datoanuevo < promedio+variacion){
//      anemo = datoanuevo;
//      promedio -= (vec[re]/ndatos + anemo/ndatos); // quito el valor mas atiguo y añado el mas reciente
//      for(int i=0; i < ndatos ;i++){
//        vec[i] = vec[i+1];
//      } 
    // falta llenar el vector
    
//    chipcap1 = analogRead(0);           // leo la temp/hum chipcap
//    chipcap2 = analogRead(1);           // leo la temp/hum chipcap
//    luminosidad = analogRead(2);        // leo el valor de la fotoresistencia
    
    //delay(mtime);                       // con este delay controlo el tiempo de muestreo -> usar interrupciones
    
    //------------------desactivo las interrupciones mientras envio los datos----------------------
    //noInterrupts();
    // armo la trama y la envio
    //Serial.print("holadatos");
//    Serial.print("GD" + ID);Serial.print(',');
//    Serial.print(year());Serial.print(',');
//    Serial.print(month());Serial.print(',');
//    Serial.print(day());Serial.print(',');
//    Serial.print(hour());Serial.print(',');
//    Serial.print(minute());Serial.print(',');
//    Serial.print(second());Serial.print(',');
//    Serial.print(chipcap1);Serial.print(',');
//    Serial.print(chipcap2);Serial.print(',');
//    Serial.print(luminosidad);Serial.print(',');
//    Serial.print(anemometro);
//    Serial.print("\r");
    // abilito de nuevo las interrupciones
    //interrupts();
    
  }
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ sub - funciones ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ llegada de datos por serial ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  void serialdata(){
//    Serial.print("sd");
    String nom = "";         // la uso para guardar la direccion de destino
    char nomb = ' ';         // guardo los datos que llegan del serial 
    String auxstr1 = "",auxstr2 = "";
    byte tiempo[6];
    //byte nomb = ' ';
    byte c, c1;
    int c2;
    
    if (Serial.available() > 0) {                        // reviso que alla datos para leer
    //Serial.print("se1----");
        nomb = Serial.read();                            // guardo el dato que llego en nomb
        //Serial.print(nomb);
        // busco datos de la base si hay continuo
        if (nomb == 'B'){                                // si llega un dato de la base 
          //Serial.print("se2-----"); 
          c = 0;
          while(Serial.available() < 0){                 // espero el caracter que sigue
            if(c>100){                                   // si demora mas de 100ms -> reinicio el loop
              c=0;                                       // si no hay datos en el serial pasados 100ms
              return;                                    // finaliza la funcion
            }else{
              delay(1);                                  // si no ha llegado nada espero 1ms y aumento enc ontador de espera
              c=c+1;
            }  
          }
          //Serial.print("se3-----");
          nomb = Serial.read();                                           // leo el dato que sigue
          //Serial.print(nomb);
          if(nomb == 'O' || nomb == 'C' || nomb == 'T' || nomb == 'L'|| nomb == 'M'){}  // pregunto si es valido
          else{lserial();return;}                                         // si el caracter no es valido -> dato erroneo -> salgo del ciclo
          
          //Serial.print("se4-----");
          
          // ---------- tomo la direccion de destino ------------
          c=0;                                                            // reinicio el contador de 10ms
          while(Serial.available() < 15){                                 // leo la direccion de destino del mensaje
            if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
          }
          //Serial.print("se5-----");
          // si llega la direccion, la tomo del buffer
          nom = "";                                      // limpio nom
          for(int i=0;i<16;i++){                         // recorro el buffer (leo mas de los 16 -> espacio intermedio)                  
            nom = (nom + char(Serial.read()));                 // leo la direccion de el buffer y la armo        
          }
          //Serial.print("se6-----");
          //Serial.print(nom);
          // pregunto si el dato que llego es para este globo o para otro
          
          if((nom == ID)||(nom == "0000000000000000")){
            // --------------------------------------------------------- acciones a los diferentes mensajes entrantes --------------------------------------------------------- 
            
            // ------------------------------------------ O (confirmacion dato recibido) ------------------------------------------ 
            if(nomb == 'O'){                             // si es O: borro el ultimo valor enviado y paso a enviar el siguiente
              c=0;                                                            // reinicio el contador de espera
              while(Serial.available() < 6){                                 // leo la hora y el caracter de finalizacion
                if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
              }
              auxstr1 = "";
              for(int i=0;i<6;i++){                         // recorro el buffer                   
                auxstr1 = (auxstr1 + byte(Serial.read()));  // leo el tiempo byte 0-225    
              }
              
              
              // pregunto si el ultimo caracter es F
              if(Serial.read() != 'F'){lserial();return;}      // si no es F el paquete es invalido -> salgo de la funcion
              // si el paquete es valido comparo la hora del ultimo valor y si coincide paso a enviar el dato siguiente
              // tomo el valor de tiempo del ultimo valor enviado
              auxstr2 = "";
              for(int i=0;i<6;i++){                               // recorro el buffer                   
                auxstr2 = (auxstr2 + vector_salida[vec_i][i]);     // leo el tiempo        
              }
              // pregunto si son el mismo valor el enviado y el recivido
              if(auxstr1 == auxstr2){
                //Serial.print("IV-----");
                //Serial.print(iv);
                vec_i = (vec_i+1);                       // aumento el indice del buffer de salida
                if((vec_i % bufsalida)==0){
                    vec_i=0;
                    fvr=false;
                 }
                //Serial.print("IV-----");
                //Serial.print(iv);
              }
            }  
            // -------------------- fin O -------------------- 
//              String dat = "";                           // creo un string para el vector con los datos a enviar
//              for(int i=0;i<7;i++){                      // recorro el vector de parametros que estan siendo utilizados
//                dat += String(para[i]);                  // concateno los datos
//                dat += ",";                              // separo los datos con ,
//              }
//              Serial.print("GS" + ID + dat + '/r');      // envio los datos a la base

            // ------------------------------------------ T (configuracion del tiempo) ------------------------------------------ 
            if(nomb == 'T'){                             // si es T: configuro el tiempo actual
              c=0;                                                            // reinicio el contador de espera
              while(Serial.available() < 6){                                 // leo la hora y el caracter de finalizacion
                if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
              }
              
              for(int i=0;i<6;i++){                         // recorro el buffer                   
                tiempo[i] = Serial.read();                   // leo el tiempo y guardo su valor byte     
              }
              
              // pregunto si el ultimo caracter es F
              if(Serial.read() != 'F'){lserial();return;}      // si no es F el paquete es invalido -> salgo de la funcion
              // si el paquete es valido configuro la hora actual
              setTime(tiempo[3],tiempo[4],tiempo[5],tiempo[2],tiempo[1],tiempo[0]);  // hora, minutos, segundos, dia, mes, añó.
              // envio mensaje indicando que la hora ha sido configurada
              Serial.print("GT" + ID + "F");
            }
            // -------------------- fin T --------------------
            
            // ------------------------------------------ C (configuracion pines) ------------------------------------------ 
            if(nomb == 'C'){                             // si es C: configuro el tiempo actual
              c=0;                                                            // reinicio el contador de espera
              while(Serial.available() < 2){                                  // leo los parametros, pin y valor
                if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
              }
              
              c = byte(Serial.read());                              // leo el pin a configurar
              c1 = byte(Serial.read());                             // leo el valor a poner el pin
              // pregunto si el ultimo caracter es F
              //Serial.print("se7-----");
              //Serial.print(c);
              
              if(Serial.read() != 'F'){lserial();return;}      // si no es F el paquete es invalido -> salgo de la funcion
              // si el paquete es valido hago el cambio y envio confrimacion
              pinMode(c,OUTPUT);
              
              //Serial.print("se8-----");
              //Serial.print(c1);w
              if (c1 == 255 || c1 == 0) digitalWrite(c,c1);
              else analogWrite(c,c1);
              // envio mensaje indicando que la hora ha sido configurada
              Serial.print("GC" + ID + "F");
            }
            // -------------------- fin C -------------------- 
            
            // ------------------------------------------ L (socilicitud lectura pin) ------------------------------------------ 
            if(nomb == 'L'){                             // si es L: envio el valor de el pin solicitado
              c=0;                                                            // reinicio el contador de espera
              while(Serial.available() < 1){                                  // leeo el pin a leer y si es lectura analoga o digital
                if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
              }
              
              c = byte(Serial.read());                              // leo el pin a leer si es >= 200 -> analogo
              //c1 = byte(Serial.read());                             // leo el tipo de pin
              
              // pregunto si el ultimo caracter es F
              if(Serial.read() != 'F'){lserial();return;}      // si no es F el paquete es invalido -> salgo de la funcion
              // si el paquete es valido procedo a enviar el valor de el pin
              
              if (c >= 200){
                c -= 200;
                c2 = analogRead(c);
//                Serial.print("se8-----analog");
//                Serial.print("-----" + c2);
                
                Serial.print("GL" + ID + obt_tiempo() + char(bytesh(c2)) + char(bytesl(c2)) + "F");
              }
              else{
                pinMode(c,INPUT);
                c1 = digitalRead(c);
                Serial.print("se8-----digital");
                Serial.print("-----" + c1);
                Serial.print("GL" + ID + obt_tiempo() + char(c1) + "F");
              }
              // envio mensaje con el valor de el pin
              
            }
            // -------------------- fin L -------------------- 
            
            // ------------------------------------------ M (configuracion tiempo de muestreo) ------------------------------------------ 
            if(nomb == 'M'){                             // si es M: configuro el tiempo actual
              c=0;                                                            // reinicio el contador de espera
              while(Serial.available() < 2){                                 // leo la hora y el caracter de finalizacion
                if(c>100){c=0;lserial();return;}else{delay(1);c=c+1;}         // si demora mas de 100ms -> reinicio el loop
              }
              
              for(int i=0;i<2;i++){                         // recorro el buffer                   
                tiempo[i] = Serial.read();                  // leo el tiempo y guardo su valor byte     
              }
              
              // pregunto si el ultimo caracter es F
              if(Serial.read() != 'F'){lserial();return;}      // si no es F el paquete es invalido -> salgo de la funcion
              // si el paquete es valido configuro el tiempo de muestreo
              
              parametros(tiempo[0],tiempo[1]);
              
              tiempomu = leer_tie();
              if(tiempomu==0){tiempomu = 1;}
              // envio mensaje indicando que la hora ha sido configurada
              Serial.print("GM" + ID + "F");
            }
            // -------------------- fin M -------------------- 
            
          // ---------------------------------- fin acciones a los diferentes mensajes entrantes ---------------------------------- 
          }// fin identificacion de ID
          
        }// fin dato de base llego
        
      }// fin serial.available > 0
      
  }// fin serial event
  
  
  // funcion que optiene el tiempo y lo formatea para ser enviado
  String obt_tiempo() {
    String tiemp = "";
    //int aux;
    int aux = year() - 2000;
    tiemp = tiemp + char(aux);
    //tiemp = tiemp + byte(year());
    tiemp = tiemp + char(month());
    tiemp = tiemp + char(day());
    tiemp = tiemp + char(hour());
    tiemp = tiemp + char(minute());
    tiemp = tiemp + char(second());
    return tiemp;
  }
  
  // conversion de 10bits to 2 bytes
  byte bytesh(int val){
    return val/256;
  }
  
   byte bytesl(int val){
     return val;
  }
  
// ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ medicion anemometro ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ

  // --------------cuenta el tiempo entre pulsos-----------------
  void anemometro1(){
    tanemometro = micros() - tantes;                         // calculo el tiempo pasado entre la ultima medicion y la actual
    tantes = micros();                                      // guardo el tiempo de la ultima medicion
  }
  
  // ------------ limpio el puerto serial -------------
   void lserial(){                             
    while(Serial.available() > 0){Serial.read();}           // si hay datos en el buffer, los quito
  }
  
  // ---------------- almaceno los parametros en la eeprom ---------------
  void parametros(byte tie1,byte tie2){
    if (EEPROM.read(0) != tie1) EEPROM.write(0,tie1);
    if (EEPROM.read(1) != tie2) EEPROM.write(1,tie2);
  }
  
  // ----------------- leeo los datos almacenados en la eeprom -----------------
  int leer_tie(){
    int x = 256*(EEPROM.read(0));
    x +=(EEPROM.read(1));
    return x;
  }
  
  // ---- limpio una cadena de strings de: saltos de lineas, enter, espacios, tabulaciones ----
  String limpiar_string(String ID){
    String auxs = "";                                       // creo una varialbe auxiliar                
    for(int i=0; i<ID.length();i++){                        // recorro el vector ID
      if(ID.charAt(i)=='\n'||ID.charAt(i)=='\r'||ID.charAt(i)==' '||ID.charAt(i)=='\t'){}
      else{auxs = auxs + ID.charAt(i);}                     // si el caracter es especial lo descarto, de lo contrario lo armo.
    }
    return auxs;                                            // debuelvo el valor
  }
  
  // ---------------- genero un dato para el vector de salida ---------------
  void gendato(){
    
  }
  
 // ------------------------------------ escombros ----------------------------------- 
