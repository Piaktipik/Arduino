
  #include <math.h>
  //-----------------------------------------declaracion de variables-----------------------------------------------
  const int CSn = 7;                // Chip select
  const int CLK = 6;                // Genera los pulsos de reloj
  int inByte = 0;                   // serial
  boolean estado = false;
  long medidas[5];
  //int i,j,p,f;
  //-----------------------------------------inicializacion de varialbes--------------------------------------------
  void setup(){
    Serial.begin(9600);             // puerto serial a 9600
    pinMode(CSn, OUTPUT);           // pin de salida
    pinMode(CLK, OUTPUT);           // pin de salida
                             // pines de lectura de encoders
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    establishContact();             // llama al suprograma que establece conexion con processing
  }
  //--------------------------------------------programa principal--------------------------------------------------
  void loop(){
    if (Serial.available() > 0) {
      inByte = Serial.read();       // get incoming byte: 
      digitalWrite(CSn, HIGH);      // ponemos en alto todos los chip selector de los encoder para impedir lectura
      digitalWrite(CLK, HIGH);      // pines de reloj en alto
      delayMicroseconds(10);         // espera (ver datasheet encoder)
      //for (int p=0;p<=5;p++){for (int p1=0;p1<=11;p1++){medidas[p][p1]=0;}} 
      for(int i=11;i>=0;i--){
        //-----------------------------primer paso activar el encoder que vamos a leer------------------------------
        
        digitalWrite(CSn, LOW);                      // activamos CS1 (chip selector)
        delayMicroseconds(5);                        // esperamos tiempo primer pulso de reloj tCLKFE
        digitalWrite(CLK, LOW);                      // pulso de reloj
        delayMicroseconds(5);                        // tiempo de espera de semi periodo de reloj TCLK/2
        digitalWrite(CLK, HIGH);                     // pulso de subida
        
        //-------------------------------se produce salida de datos del encoder-------------------------------------
        delayMicroseconds(500);                      // tiempo de espera tDO valid
        
        for(int j=0;j<=5;j++){                      // se procede a leer
          estado = digitalRead(j+8);
          if(estado == HIGH){medidas[j] = medidas[j] + pow(2,i);}
        }
        
      }
      
      for(int j=0;j<=4;j++){
          Serial.println(medidas[j]);
      }
      //for (i=0;i<=5;i++){medidas[i]=0;}
    }// fin if establish contact
    
    else{establishContact();} // descomentar para hacer que funcione en todo momento
    
  }
  //--------------------------------------------subprogramas--------------------------------------------------------
  
  void establishContact() {
    while (Serial.available() <= 0) {           // envia A mienstras no se halla establecido conexion
      Serial.write('A');                     
      delay(500);}
  }// fin establishcontact
  
  
  
  
  
  
  //---------------------------------------------comentarios--------------------------------------------------------
  //const int DO = 9,D1 = 10,D2 = 11,D3 = 12,D4 = 13;  //recibe los bits del encoder
  //int conta =0;
  //int suma =0;
  //int i = 0;
  //int dato = 0;
  //suma =0;