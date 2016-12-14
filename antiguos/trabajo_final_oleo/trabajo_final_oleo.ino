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
const int si1 = 7;      // sensor infrarrojo 1
const int si2 = 6;      // sensor infrarrojo 2

const int p1 = 5;      // piston 1
const int p2 = 4;      // piston 2
const int p3 = 3;      // piston 3

// --------------------------- VARIABLES --------------------------- 
int indu = 0;        // variable sensor inductivo

boolean ind = 0;        // variable sensor inductivo
boolean sif1 = 0;        // variable sensor infrarrojo 1
boolean sif2 = 0;        // variable sensor infrarrojo 2

// lo uso para saber si es metal o prlastico
boolean met = 0;        // variable metal / plastico

// --------------------------- CONFIGURACION INICIAL --------------------------- 
void setup() {
  Serial. begin(9600);      // incializo el puerto serial para mandar datos al pc
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("      HOLA");
  
  pinMode(p1,OUTPUT);    // salida 
  pinMode(p2,OUTPUT);    // salida
  pinMode(p3,OUTPUT);    // salida
  
  pinMode(si1,INPUT);    // entrada
  pinMode(si2,INPUT);    // entrada
  delay(2000);
}

// --------------------------- PROGRAMA PRINCIPAL --------------------------- 
void loop() {
  mindu();      // mido sensor inductivo
  
  if (ind){
    met = 1;
  }
  
  sif1 = digitalRead(si1);
  sif2 = digitalRead(si2);
  
  // muestro valores por el lcd
  if (met){
    lcd.setCursor(0, 0);
    lcd.print("     METAL      ");
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("    PLASTICO");
  }
  // muestro en la segunda linea
  lcd.setCursor(0, 1);
  lcd.print("s1:");
  lcd.print(sif1);
  lcd.print("  s2:");
  lcd.print(sif2);
  lcd.print("  in:");
  lcd.print(ind);
  
  if (sif1 && met){
    delay(500);
    digitalWrite(p1,1);
    delay(500);
    met = 0;
  }
  else{
    digitalWrite(p1,0); 
  }
  
  if (sif2){
    delay(300);
    digitalWrite(p2,1);
    delay(500);
  }
  else{
    digitalWrite(p2,0);
  }
  
}

void mindu(){
  indu = analogRead(0);    // leo el sensor inductivo
  if (indu > 10){
    ind = 0;
  }
  else{
    ind = 1;
  }
}
