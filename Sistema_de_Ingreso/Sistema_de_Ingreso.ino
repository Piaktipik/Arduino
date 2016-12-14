//por: Julian Galvez Serna

// ----------- libreria display lcd ----------- 
#include <LiquidCrystal.h>               // cargo la libreria
LiquidCrystal lcd(51, 50, 38, 36, 42, 40);    // monto el display
/* LCD RS pin to digital pin 51
 * LCD Enable pein to digital pin 50
 
 * LCD D4 pin to digital pin 36 40
 * LCD D5 pin to digital pin 38 42
 * LCD D6 pin to digital pin 40 36
 * LCD D7 pin to digital pin 42 38
 
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */


// ----------- libreria tiempo -----------
// #include <Time.h>
 // hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

// ----------- libreria teclado tactil -----------  
/*  
 Hardware: 5V -> arduino mega
           SDA -> D20
           SCL -> D21
           IRQ -> D2
*/
 
#include <Wire.h>
#include <mpr121.h>
 
int key = 0;
String Cedula = "";
String dato = "";
char estado = 'I';
// ----------- varialbles menu -----------

////////////////////////////////////// configuraciones iniciales ////////////////////////////////////// 

void setup(){
  // --------------- configuro el tiempo --------------- 
  //setTime(12,0,0,12,4,15);                          // hora, minutos, segundos, dia, mes, añó.
  
  // inicializo diplay lcd 16x2
  lcd.begin(16, 2);
  lcd.print("   iniciando ...   ");
  lcd.setCursor(0, 1);
  
  //  --------------- se incia la comunicacion serial --------------- 
  Serial.begin(115200);
  
  conectarce(); // espera una respuesta de la raspberry para confirmar la conexion
  
  // --------------- se inicializa el teclado --------------- 
  Wire.begin();
  CapaTouch.begin();
 
  delay(500);
  Serial.println("START"); 
  
  // --------------- conecto con la SD ---------------

  lcd.print("  Buenos Dias  ");
  delay(1000);
  lcd.clear(); 
  
  // creo mensaje inicial
  lcd.setCursor(0, 0);
  lcd.print("Hola, Ingrese Cedula");
  
}

////////////////////////////////////// programa principal ////////////////////////////////////// 
void loop(){
  // leo las teclas
  key = CapaTouch.keyPad();
  // nos comunicamos indicando el estado del sistema
  if(estado == 'I'){
    registrando();
    estado='R';
  }
  // si hay una tecla presionada
  if(key>=0){
    
    // si las teclas son los caracteres especiales:
    if (key==11){
      // borrar
      //Serial.println(Cedula);
      Cedula = Cedula.substring(0, Cedula.length()-1);
      delay(300);  // tiempo de visualizacion del mensaje
    }
    else if(key==12){
      //enter
      Serial.println(Cedula);
      //delay(1000);  // tiempo de visualizacion del mensaje
      Cedula="";
      lcd.setCursor(0, 0);
      lcd.print("Registrando...      "); // este mensaje deve ser tomado de la raspberry
      
      lcd.setCursor(0, 0);
      Serial.read();
      lcd.print(leer_linea() + "          ");  // mensaje hombre/mujer
      
      lcd.setCursor(0, 1);
      //Serial.read();
      lcd.print(leer_linea() + "          "); // este mensaje deve ser tomado de la raspberry
      delay(1000);  // tiempo de visualizacion del mensaje
      
    }else{
      Cedula += String(key);
      delay(300);  // tiempo anti rebote
    }
    
    // muestro el mensaje por el lcd
    lcd.setCursor(0, 0);
    lcd.print("Hola, Ingrese Cedula");
    lcd.setCursor(0, 1);
    lcd.print("ID: " + Cedula + "                 ");
    
  }
  if(Serial.available() > 0) {
    leer_linea();
  }
  
}// fin void loop
void registrando() {
  while(1) {
    Serial.println("REGISTRANDO");   // avisamos que estasmos conectados
    delay(300);
    // leemos hasta el fin de carrera
    if(Serial.available() > 0) {
      if(leer_linea() == "REGISTRANDO"){
        return;
      }
    }
  }
}


void conectarce() {
  while(1) {
    Serial.println("ARDUINO");   // avisamos que estasmos conectados
    delay(300);
    // leemos hasta el fin de carrera
    if(Serial.available() > 0) {
      if(leer_linea() == "HOLAARD"){
        Serial.println("HOLA"); 
        return;
      }
    }
  }
}

String leer_linea(){
  String rv = "";   // se usa para almacenar los caracteres hasta el salto
  int c = 0;        // se añade tiempo maximo de espera
  char ch = ' ';
  //Serial.read();
  while(c<2000){
    if(Serial.available() > 0) {
      ch = Serial.read();
      if(ch =='\r') {
        Serial.println(rv);
        return rv;
      }
      rv += String(ch);
    }
    delay(1);
    c++;
  }
  return "";
}

