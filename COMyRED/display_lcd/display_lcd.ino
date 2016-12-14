//por: Julian Galvez Serna

// ----------- libreria display lcd ----------- 
#include <LiquidCrystal.h>               // cargo la libreria
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);    // monto el display
/* LCD RS pin to digital pin 51
 * LCD Enable pein to digital pin 50
 * LCD D4 pin to digital pin 36
 * LCD D5 pin to digital pin 38
 * LCD D6 pin to digital pin 40
 * LCD D7 pin to digital pin 42
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */
char tecla = 0;         // dato obtenido por serial
bool inicio = 0;        // bandera que nos permite saber si se ha inciado el envio de caracteres
char tecla_aux = 0;     // guardamos la tecla antes de verificar el dato
void setup(){
  // inicializo diplay lcd 16x2
  lcd.begin(16, 2);
  lcd.print("   iniciando LCD... ");
  lcd.setCursor(0, 1);
  delay(500);  
  
  lcd.print("  LCD iniciada  ");
  delay(500);
  lcd.clear(); 

  // inicializamos el puerto serial
  Serial.begin(50);
}
////////////////////////////////////// programa principal ////////////////////////////////////// 
void loop(){
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print("   Tecla Pulsada:    ");
  lcd.setCursor(0, 1);
  lcd.print("        " + String(tecla));
  if (Serial.available() > 0) {
      // leemos la tecla que llego por el puerto y la mostramos:
      // dependiendo de la calidad de los datos que llegan por serial
      // se podria requerir verificar la valiz del dato enviando cabecera y checksum
      
      //leemos el dato
      char dato = (char)Serial.read();
      // varificamos el dato
      if(inicio == 0 && dato == '<'){
        inicio = 1; // caracter de inicio de trama detectado
        tecla_aux = 0; // limpiamos el caracter que almacena la tecla recivida
        
      }else if(inicio == 1 && tecla_aux == 0){
        tecla_aux = dato;
        
      }else if(inicio == 1 && tecla_aux != 0 && dato == '>'  ){
        // guardamos la tecla
        tecla = tecla_aux;
        Serial.print("<" + String(tecla) + ">");
        inicio = 0;
        
      }else{
        inicio = 0;
      }
      
    }
    
}// fin void loop


