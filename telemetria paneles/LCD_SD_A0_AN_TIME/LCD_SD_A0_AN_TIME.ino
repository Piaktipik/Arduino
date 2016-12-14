//por: Julian Galvez Serna

// ----------- libreria display lcd ----------- 
#include <LiquidCrystal.h>               // cargo la libreria
LiquidCrystal lcd(10, 9, 4, 5, 2, 3);    // monto el display
/* LCD RS pin to digital pin 10
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 2
 * LCD D7 pin to digital pin 3
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

// ----------- libreria sd ----------- 
#include <SD.h> // libreria para el manejo de SD
 /* SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 8
 */
const int chipSelect = 8; // especifico para el arduino 1 y sd shield spark_fun
String datos = "";
int sensor = 0;
File archivo;

// ----------- libreria tiempo -----------
 #include <Time.h>
 // hour();, minute();, second();, weekday();, day();, monthShortStr(month());, year();.

// ----------- pulsadores de control -----------  
const int bot1 = 6;     // seleccionar
const int bot2 = 7;     // aumentar

boolean bot1e;     // estado seleccionar
boolean bot2e;     // estado aumentar

// ----------- varialbles menu -----------
//  # archivo / # de canales / ajustar reloj / iniciar captura 
int menu = 0;  // (0-3) variable de estado de menu
int numa = 0;  // (0-x) numero de data_# dondse se guardan los datos
int numc = 0;  // (0-5) numero de canales a muestrear

int ho = 0;   // hora
int mi = 0;   // minuto
int di = 0;   // dia
int me = 0;   // mes
int an = 0;  // año
int con = 0;  // contador de ingreso de datos
////////////////////////////////////// configuraciones iniciales ////////////////////////////////////// 

void setup(){
  //--------------configuro el tiempo-------------------------
    
    setTime(0,0,0,1,1,14);                          // hora, minutos, segundos, dia, mes, añó.
    
  // inicializo diplay lcd 16x2
  lcd.begin(16, 2);
  lcd.print("   iniciando SD... ");
  lcd.setCursor(0, 1);
  delay(500);  
  //---------------conecto con la SD-------------------------
  pinMode(8, OUTPUT);
  // inicializo la SD
  if (!SD.begin(chipSelect)) {
    lcd.print("  SD mala/sin  ");
    delay(500);  
    setup();
  }
  lcd.print("  SD iniciada  ");
  delay(500);
  lcd.clear(); 
}
////////////////////////////////////// programa principal ////////////////////////////////////// 
void loop(){
  switch (menu) {
   case 0:
      lcd.setCursor(0, 0);
      lcd.print("   # archivo    ");
   break;
   case 1:
      lcd.setCursor(0, 0);
      lcd.print("  # de canales  ");
   break;
   case 2:
      lcd.setCursor(0, 0);
      lcd.print("  ajustar reloj ");
   break;
   default:
      lcd.setCursor(0, 0);
      lcd.print(" iniciar captura");
}
  // leo los pulsadores
  bot1e = digitalRead(bot1);
  bot2e = digitalRead(bot2);
  // muestro el menu de toma de datos
  if (!bot1e){
    lcd.clear();
    bot1e = 1; // aseguro que se ingrese al while
    delay(500);
    switch (menu) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print("   # archivo    ");
        lcd.setCursor(0, 1);
        lcd.print("#: "+ String(numa));
        // mientras que no se presione de nuevo seleccion
        while(bot1e){
          // espero que se presione aumentar
          bot2e = digitalRead(bot2);
          if (!bot2e){
            numa = numa + 1;
            lcd.setCursor(0, 1);
            lcd.print("#: "+ String(numa));
            delay(200);
          }
          bot1e = digitalRead(bot1);
        }
        lcd.clear();
        delay(500); // espero antirrebote
      break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print("  # de canales  ");
        lcd.setCursor(0, 1);
        lcd.print("#: "+ String(numc));
        // mientras que no se presione de nuevo seleccion
        while(bot1e){
          // espero que se presione aumentar
          bot2e = digitalRead(bot2);
          if (!bot2e){
            numc = (numc+1)%6;
            lcd.setCursor(0, 1);
            lcd.print("#: "+ String(numc));
            delay(200);
          }
          bot1e = digitalRead(bot1);
        }
        lcd.clear();
        delay(500); // espero antirrebote
      break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print("ajustar reloj: ");
        lcd.setCursor(0, 1);
        lcd.print(String(day())+"/"+String(month())+"/"+String(year())+" - "+String(hour())+":"+String(minute()));
        // mientras que no se presione de nuevo seleccion
        con = 0;
        while(con < 5){
          // espero que se presione aumentar
          bot2e = digitalRead(bot2);
          if (!bot2e){
            lcd.setCursor(14,0);
            switch (con) {
               case 0:  di = (di)%31+1;
               lcd.print("di");
               break;
               case 1:  me = (me)%12+1;
               lcd.print("me");
               break;
               case 2:  an = (an+1)%7;
               lcd.print("an");
               break;
               case 3:  ho = (ho+1)%24;
               lcd.print("ho");
               break;
               default: mi = (mi+1)%60;
               lcd.print("mi");
            }
            // se pone el tiempo actual
            setTime(ho,mi,0,di,me,an+14);
            lcd.setCursor(0, 1);
            lcd.print(String(day())+"/"+String(month())+"/"+String(year())+" - "+String(hour())+":"+String(minute())+"     ");
            delay(200);  
          }
          bot1e = digitalRead(bot1);
          // cuento el numero de veces que se presiona seleccionar 
          if (!bot1e){
            con = con+1;
            delay(200);
          }
        } // fin while
        lcd.clear();
        delay(500); // espero antirrebote
      break;
      default:
        // while programa principal
        while(bot1e){
          lcd.setCursor(0, 0);
          lcd.print(" "+String(day())+"/"+String(month())+"/"+String(year())+" - "+String(hour())+":"+String(minute())+":"+String(second()));
          lcd.setCursor(0, 1);
          lcd.print("tomando datos...");
          
          // abro el archivo  
          String narchi = "datos_";
          narchi.concat(numa);
          narchi.concat(".txt");
          char nombre[narchi.length()+1];
          narchi.toCharArray(nombre, sizeof(nombre));
          archivo = SD.open(nombre, FILE_WRITE);
          
          //---------------------------cargo los datos-------------------------
          datos = (String(day())+"/"+String(month())+"/"+String(year())+" - "+String(hour())+":"+String(minute())+":"+String(second())+ "\t");
          for (int ana = 0; ana <= numc; ana++){
            sensor = analogRead(ana);
            datos += String(sensor);
            if (ana < numc){datos += ",";}
          }
          
          //----------------preparo el archivo para los datos-------------------
          if (archivo){
            // si el dato es correcto, escribo en él 
            archivo.println(datos);               // escribo    
            datos = "";
            lcd.setCursor(0, 1);
            lcd.print(" guardando datos...");
          }else{
            archivo.close();                      // cierro el archivo
            archivo = SD.open(nombre, FILE_WRITE);
          } 
          archivo.close(); 
          delay(100);
          lcd.setCursor(0, 1);
          lcd.print(" guardando datos . ");
          // fin guardado en sd
        } // fin while lectura de datos
        lcd.clear();
        delay(500); // espero antirrebote
      }
}
  // me muevo a traves del menu
  if (!bot2e){
    menu = (menu+1)%4;
    delay(500);
  }
  
}// fin void loop
