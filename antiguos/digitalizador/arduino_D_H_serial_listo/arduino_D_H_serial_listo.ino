/* programa para leer los datos de un encoder
de 12 bits de resolucion
*/

// set pin numbers:
#include <math.h>

const int CSn = 8;     // Chip select
const int CLK = 9;      // Genera los pulsos de reloj
//const int DO = 47;  //recibe los bits del encoder
// variables will change:
//int buttonState = 0;         // variable for reading the pushbutton status
//int conta =0;
//int estado =0;
int suma[5] = {0,0,0,0,0};
int i = 0;
int dato = 0;

void setup() {
  pinMode(CSn, OUTPUT);                   // pin de habilitador del encoder
  pinMode(CLK, OUTPUT);                   // pin de pulso de salida
  for(i=2;i<7;i++){pinMode(i, INPUT);}    // pin de lectura de datos
  Serial.begin(19200);
  establishContact();  // send a byte to establish contact until Processing responds 
  Serial.read();
}

void loop(){
  if(Serial.available() > 0){
    Serial.read();
    //---------------ponemos en alto todos los chip selector de los encoder para impedir lectura-------------
    
    digitalWrite(CSn, HIGH);            //pines de reloj en alto
    digitalWrite(CLK, HIGH);            //CLK pin de reloj
    delayMicroseconds(1);               //espera
    
    for(i=0;i<5;i++){suma[i]=0;}
    
    for(i = 11; i>=0;i--){              //primer paso activar el encoder que vamos a leer
                                        //activamos encoder 1 CS1 (chip selector 1)
      digitalWrite(CSn, LOW);
      //esperamos tiempo primer pulso de reloj tCLKFE
      delayMicroseconds(1);
      //pulso de reloj
      digitalWrite(CLK, LOW);
      delayMicroseconds(1); //tiempo de espera de semi periodo de reloj TCLK/2
      digitalWrite(CLK, HIGH);//pulso de subida
      delayMicroseconds(1);//se produce salida de datos del encoder
      //tiempo de espera tDO valid
      //se procede a leer
      for(int j=0;j<5;j++){
        if(digitalRead(j+2)== HIGH){
          suma[j] = suma[j] + pow(2,i);
        }
      }

      }//for 12
       for(i=0; i<5;i++){
         Serial.print(suma[i]);
         Serial.print('a');
       }
       Serial.print('b');
       //Serial.println(suma[0]);
       //Serial.println("");
   } // if serialavailable
   //else{establishContact();} // (no sirve)descomentar para hacer que funcione en todo momento
}

void establishContact() {
 while (Serial.available() <= 0) {
      Serial.write('A');   // send a capital A
      delay(100);
  }
}
