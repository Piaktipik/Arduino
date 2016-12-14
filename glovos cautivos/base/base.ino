//por: Julian Galvez Serna
//------------------------------------------------librerias----------------------------------------
/*-----------------------------SD-------------------------------------
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 8 (arduino uno -  microSD shield - sparkfun)
  CS - pin 4 (arduino uno - WIRELESS shield)
 */ 
#include <SD.h> // libreria para el manejo de SD
const int chipSelect = 4; // especifico para el arduino 1

//----------------------------serial por software----------------------
//#include <SoftwareSerial.h>
//
//SoftwareSerial Sserial(2, 3); // RX, TX
//String dato;  

//--------------------------------tiempo-------------------------------
//#include <Time.h>

// hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

//-------------------------------------------variables---------------------------------------------
String datos = "";
//int sensor = 0;
File archivo;
char leo = 0;

//---------------------------------------programa principal----------------------------------------
void setup(){
  // configuracion serial por software
//  Sserial.begin(9600);
//  
//  // configuracion del tiempo
//  setTime(23,59,50,22,10,12); // hora, minutos, segundos, dia, mes, añó.
//  
  
  Serial.begin(9600);  // activo el puerto serial para enviar datos
  //-------------------------conecto con la SD-------------------------
  Serial.print("Initializing SD card...");
  pinMode(4, OUTPUT);
  // inicializo la SD
  if (!SD.begin(chipSelect)) {  
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}
//--------------------------------programa principal--------------------------------------
void loop(){
  //if (Sserial.available()>0){serialbyte = Sserial.read();}
  
  // abro el archivo  
  archivo = SD.open("DATA.txt", FILE_WRITE);
  //---------------------------cargo los datos-------------------------
  if (Serial.available()>0){
    leo = Serial.read();
    if (leo == 'G'){
    
    }
    
  }
    
  
  //for (int an = 0; an < 3; an++){
  //  sensor = analogRead(an);
  //  datos += String(sensor);
  //  if (an < 2){datos += ",";}
  //}
  
  //----------------preparo el archivo para los datos-------------------
  if (archivo){
    // si el dato es correcto, escribo en él 
    archivo.println(datos);               // escribo    
    //Serial.println(datos);                // imprimo por serial
    datos="";
  }else{
    Serial.println("error opening datalog.txt");
    archivo.close();                      // cierro el archivo
    archivo = SD.open("datalog.txt", FILE_WRITE);
  } 
  delay(1);
  //Serial.println("punto final");
}
