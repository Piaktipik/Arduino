/* programa para leer los datos de un encoder
de 12 bits de resolucion
*/

// constants won't change. They're used here to 
// set pin numbers:
#include <math.h>

const int CSn = 52;     // Chip select
const int CLK = 51;      // Genera los pulsos de reloj
const int DO = 47;  //recibe los bits del encoder
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int conta =0;
int estado =0;
int suma =0;
int i = 0;
int dato = 0;

void setup() {
  // pin de habilitador del encoder
  pinMode(CSn, OUTPUT);      
  // pin de pulso de salida
  pinMode(CLK, OUTPUT);     
  // pin de lectura de datos
  pinMode(DO, INPUT);
  Serial.begin(9600);
}

void loop(){
  
  
  //ponemos en alto todos los chip selector de los encoder
        //para impedir lectura
      digitalWrite(CSn, HIGH);
        //pines de reloj en alto
      digitalWrite(CLK, HIGH);//CLK pin de reloj
        delayMicroseconds(1);//espera
        suma =0;
        for(i = 11; i>=0;i--)
        {
        //primer paso activar el encoder que vamos a leer
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
      estado = digitalRead(DO);
      if(estado == HIGH)
        {
        suma = suma + 2^i;
        }
        }
        //dato = suma;
        Serial.println(suma); 
}

