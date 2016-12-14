/*
  SD card datalogger

 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 */
#include <SD.h> // libreria para el manejo de SD

const int chipSelect = 8; // especifico para el arduino 1
String datos = "";
int sensor = 0;
File archivo;

void setup(){
  Serial.begin(9600);  // activo el puerto serial para enviar datos
  //-------------------------conecto con la SD-------------------------
  Serial.print("Initializing SD card...");
  pinMode(8, OUTPUT);
  // inicializo la SD
  if (!SD.begin(chipSelect)) {  
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop(){
  // abro el archivo  
  archivo = SD.open("datalog.txt", FILE_WRITE);
  //---------------------------cargo los datos-------------------------
  for (int an = 0; an < 3; an++){
    sensor = analogRead(an);
    datos += String(sensor);
    if (an < 2){datos += ",";}
  }
  
  //----------------preparo el archivo para los datos-------------------
  if (archivo){
    // si el dato es correcto, escribo en él 
    archivo.println(datos);               // escribo    
    Serial.println(datos);                // imprimo por serial
    datos="";
  }else{
    Serial.println("error opening datalog.txt");
    archivo.close();                      // cierro el archivo
    archivo = SD.open("datalog.txt", FILE_WRITE);
  } 
  archivo.close(); 
  delay(1);
  //Serial.println("punto final");
}
