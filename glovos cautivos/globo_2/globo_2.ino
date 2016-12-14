  //por: Julian Galvez Serna
  //------------------------------------------------librerias-------------------------------------------
  #include <EEPROM.h>      // se usara para guardar valores de configuracion
  
  //-------------------------tiempo----------------------------------
  
  # include <Time.h>
  // hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

  //-------------------------------------variables analogas del entorno---------------------------------
  
  int chipcap1 = 0;        // variable sensor temperatura/humedad
  int chipcap2 = 0;        // variable sensor temperatura/humedad
  int luminosidad = 0;     // variable sensor intensidad luminosa
  
  //--------------------------------------otras variables---------------------------------------------
  
  String ID = "";          // identificacion del globo -> serial del xbee
  int mtime = 400;         // (tiempo entre medidas) - variable configurable de la base
  int dato[1];             // variable para lectura de datos por serial
  word para[7];            // variable para guardar los parametros del globo
  String nom = "";         // la uso para guardar la direccion de destino
  char nomb =' ';          // guardo los datos que llegan del serial
 
  byte c1 = 0;             // variable usada para entrar solo una vez
  
  //int velserial[] = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200};  //futura configuracion de velocidades
  
  //---------------------------variables para la lectrura del anemometro-------------------------------
  
  unsigned long anemometro = 0;      // variable anemometro (tiempo entre pulsos)
  unsigned long tantes = 0;          // tiempo a el pulso anterior
  unsigned long tdespues = 0;        // timepo a el pulso actual
  
  //-------------------------------------configuraciones iniciales-------------------------------------
  void setup(){
    
    leerpara();                    // leeo los parametros de configuracion almacenados en la eeprom
    
    Serial.begin(9600);            // velocidad serial
    
    int c = 0;              // contador usado para los retardos
    
    xbee:
    
    //---------me conecto con el xbee y obtengo su numero serial, para usarlo como id del globo--------- 
    lserial();                                           // limpio el puerto serial
    delay(1000);                                         // espero un instante antes de iniciar la comunicacion con el xbee
    Serial.print("+++");                                 // entro en modo AT
    while(Serial.available() < 1){                       // si no he recivido almenos 2 valores -> espero (OK)
      if(c>150){c=0;goto xbee;}else{delay(10);c=c+1;}    // reviso si envio el OK cada 10ms, si demora mas de 1500ms reintenta la comunicacion 
    }
    
    Serial.read();Serial.read();delay(50);               // elimino el OK del buffer y espero un instante
    Serial.write("ATSH\r");delay(50);                    // solicito direcion parte alta
    Serial.write("ATSL\r");delay(50);                    // solicito direcion parte baja
    Serial.write("ATCN\r");                              // cierro el modo AT
    
    c=0;                                                 // reinicio el contador de retardos de 10ms
    while(Serial.available() < 14) {                     // reviso si llego la dirrecion (16 bits) cada 10ms
      if(c>200){c=0;goto xbee;}else{delay(10);c=c+1;}      // si demora mas de 2000ms reintenta la comunicacion
    }
    
    c=0;                                                 // reinicio el contador de retardos de 10ms
    for(int i=0;i<16;i++){ID = ID + char(Serial.read());}// armo el ID del xbee
    ID = limpiar_string(ID);                             // limpio el ID de caracteres especiales
    
    //--------------configuro el tiempo-------------------------
    
    //setTime(23,59,50,22,10,12);                          // hora, minutos, segundos, dia, mes, añó.
    
    //---------------------------------------------------------------------------------------
    pinMode(13, INPUT);
    //pinMode(13,OUTPUT);digitalWrite(13,LOW);             // apago el led superficila para ahorrar energia
    pinMode(2, INPUT);                                   // configuro como entrada el pin del anemometro
    attachInterrupt(0, pulsos, FALLING);                 // abilito interrupcion en pin 2 - acciono la funcion: pulsos
    lserial();                                           // limpio el serial antes de empezar 

  } // fin configuracion
  
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  //-----------------------------------------------programa principal----------------------------------------------
  void loop(){
    int c = 0;              // contador usado para los retardos
    
    // ----------------------- pregunto por datos de configuracion --------------------
    
    if (Serial.available() > 0) {                        // si llegan datos seriales 
        nomb = char(Serial.read());                      // guardo el dato que llego en nomb
        if (nomb == 'B'){                                // si llega un dato de la base  
          //Serial.print("hola1");
          while(Serial.available() < 0){                 // espero el caracter que sigue
            if(c>50){c=0;return;}else{delay(10);c=c+1;}  // si demora mas de 500ms -> reinicio el loop
            c=0;                                         // reinicio el contador de 10ms
          }
          nomb = char(Serial.read());                    // leo el dato que llego
          if(nomb == 'S' || nomb == 'T' || nomb == 'Q'){}// pregunto si segundo caracter es valido
          else{return;}                                  // si el caracter no es valido -> dato erroneo -> salgo del ciclo
          // ---------- tomo la direccion de destino ------------
          while(Serial.available() < 14){                // leo la direccion de destino del mensaje
            if(c>200){c=0;return;}else{delay(10);c=c+1;} // si demora mas de 2000ms -> reinicio el loop
              c=0;                                       // reinicio el contador de 10ms
          }
          nom = "";                                      // limpio nom
          for(int i=0;i<14;i++){                         // recorro el bufer (leo mas de los 16 -> espacio intermedio)                  
            nom = (nom + Serial.read());                 // leo la direccion de el buffer y la armo        
          }
          nom = limpiar_string(nom);                     // limpio la direccion de destino
          Serial.print(nom);                             // imprimo la direccion de destino (depuracion)
          if((nom == ID)||(nom == "00000000000000")){    // pregunto si la direccion es la de el globo y ejecuto la accion correspondiente
            if(nomb == 'S'){                             // si es S: (pedir los parametros del globo)
              String dat = "";                           // creo un string para el vector con los datos a enviar
              for(int i=0;i<7;i++){                      // recorro el vector de parametros que estan siendo utilizados
                dat += String(para[i]);                  // concateno los datos
                dat += ",";                              // separo los datos con ,
              }
              Serial.print("GS" + ID + dat + '/r');      // envio los datos a la base
            }
            if(nomb == 'T'){                             // si es T: (configurar el tiempo actual)
              
            }          
            if(nomb == 'Q'){                             // si es Q: ()
              
            }
          }                                              // fin identificacion de ID
        }                                                // fin if B
      }                                                  // fin serial.available                                    
      
    //--------------------------------leeo las 3 variables analogas---------------------------------
    
    chipcap1 = analogRead(0);           // leo la temp/hum chipcap
    chipcap2 = analogRead(1);           // leo la temp/hum chipcap
    luminosidad = analogRead(2);        // leo el valor de la fotoresistencia
    delay(mtime);                       // con este delay controlo el tiempo de muestreo -> usar interrupciones
    
    /* convierto a velocidad
    1 pulse/second = 2.5 mph
    */
    
    //------------------desactivo las interrupciones mientras envio los datos----------------------
    //noInterrupts();
    // armo la trama y la envio
    //Serial.print("holadatos");
    Serial.print("GD" + ID);Serial.print(',');
    Serial.print(year());Serial.print(',');
    Serial.print(month());Serial.print(',');
    Serial.print(day());Serial.print(',');
    Serial.print(hour());Serial.print(',');
    Serial.print(minute());Serial.print(',');
    Serial.print(second());Serial.print(',');
    Serial.print(chipcap1);Serial.print(',');
    Serial.print(chipcap2);Serial.print(',');
    Serial.print(luminosidad);Serial.print(',');
    Serial.print(anemometro);
    Serial.print("\r");
    // abilito de nuevo las interrupciones
    //interrupts();
    
  }
  
  // ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
  
  // ----------------------------------------------------------- sub-funciones -----------------------------------------------------------
  
  // --------------cuenta el tiempo entre pulsos-----------------
  
  void pulsos(){
    tdespues = millis();                                    // tomo el tiempo actual
    anemometro = tdespues - tantes;                         // calculo el tiempo pasado entre la ultima medicion y la actual
    tantes = millis();                                      // guardo el tiempo en el cual se realizo esta medicion
  }
  
  // ------------ limpio el puerto serial -------------
  
   void lserial(){                             
    while(Serial.available() > 0){Serial.read();}           // si hay datos en el buffer, los quito
  }
  
  // ---------------- almaceno los parametros en la eeprom ---------------
  
  void parametros(){
    for(int i=0;i<=7;i++){                                  // recorro la eeprom
      word x = word(EEPROM.read(2*i),EEPROM.read(2*i+1));   // tomo los valores de la eeprom
      if(x==para[i]){}                                      // pregunto si el valor a guardar es igual al almacenado, de ser asi no hago nada
      else{                                                 // si el valor es diferente procedo a almacenarlo
        EEPROM.write(2*i,highByte(para[i]));                // guardo la parte alta
        EEPROM.write(2*i+1,lowByte(para[i]));               // guardo la parte baja
      }
    }
  }
  
  // ----------------- leeo y armo los datos almacenados en la eeprom -----------------
  
  void leerpara(){
    for(int i=0;i<=7;i++){                                  // recorro las posiciones de la eeprom
      para[i] = word(EEPROM.read(2*i),EEPROM.read(2*i+1));  // leo de la eeprom y convierto a word
      //Serial.print(para[i]);                              // probposito de depuracion
    }  
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
  
 // ------------------------------------ escombros ----------------------------------- 
  /*
      Serial.print("hola3");
      // si llaman a este globo(ID) o a todos(0) ajusto la velocidad ej:(00000000000000ÿ)
      // ojo para mtime por debajo de 20 ms el arduino colapsa y solo es estable apartir de 40ms uno r3-50ms uno r2
      if (nom.equals(ID) || nom.equals("00000000000000")){
        Serial.print("hola4");
        int num = Serial.read();
        if (num > 3){mtime = 10 * num;}else{lserial;loop;}  //leeo el primer numero de 0 a 255
      }else{nom="";lserial;loop;}
      nom="";
      //Serial.print("hola-5");
      */
