/*
  The circuit:
 * LCD RS pin to digital pin 13
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 11
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 9
 * LCD D7 pin to digital pin 8
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */
// --------------------------- LIBRERIAS --------------------------- 
// libreria lcd
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// --------------------------- CONEXIONES --------------------------- 
const int bom = 7;      // pin que controla el foco
const int fan = 6;      // pin que contola el ventilador - PWM

// --------------------------- VARIALBES --------------------------- 
float tc = 0;            // temperatura de control
float ts = 0;            // temperatura medida

float m = 200/1024;      // constante convercion del termistor
float b = 0;             // constante convercion del termistor

int c = 0;               // contador de datos en puerto serial
byte dato = 0;           // lo uso para guardar los datos que llegan
// --------------------------- CONFIGURACION INICIAL --------------------------- 
void setup() {
  Serial. begin(9600);      // incializo el puerto serial mandar datos al pc
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("      HOLA");
  
  pinMode(bom,OUTPUT);    // salida 
  pinMode(fan,OUTPUT);    // salida 
}

// --------------------------- PROGRAMA PRINCIPAL --------------------------- 
void loop() {
  ts = analogRead(0);    // leo el termistor
  ts = m*ts + b;         // linealizo el valor del termistor
  Serial.print("temperatura medida: ");
  Serial.println(ts);
  // implemento el control de T pedido
  if(tc - 1 <= ts <= tc + 1){
    digitalWrite(bom,0);
    analogWrite(fan,0);
  }
  if(ts < tc - 1){
    digitalWrite(bom,1);
    analogWrite(fan,0);
  }
  if(tc + 1 < ts < tc + 3){
    digitalWrite(bom,0);
    analogWrite(fan,128);    // 50%
  }
  if(tc + 3 <= ts <= tc + 5){
    digitalWrite(bom,0);
    analogWrite(fan,204);    // 80%
  }
  if(ts > tc + 5){
    digitalWrite(bom,0);
    analogWrite(fan,255);    // 100%
  }
  // muestro valores por el lcd
  lcd.setCursor(0, 0);
  lcd.print("T control: ");
  lcd.print(tc);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("T medida: ");
  lcd.print(ts);
  lcd.print("   ");
  
  delay(100);
  if (Serial.available() > 3){
    tc = Serial.parseFloat();
  }
}

//void serialEvent(){
//  dato = Serial.read();
//  if (dato == 't'){
//    c++;
//  }
//  else if(c < 0){
//    
//  }
//  
//}

