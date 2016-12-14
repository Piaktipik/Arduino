//por: Julian Galvez Serna, laura olaya, francis osorio

////////////////////////////////////////  LIBRERIA EEPROM ////////////////////////////////////// 
#include <EEPROM.h>      // se usara para guardar valores de configuracion

////////////////////////////////////////  LIBRERIA LCD ////////////////////////////////////// 
#include <LiquidCrystal.h>               // cargo la libreria
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);    // monto el display
/* LCD RS pin to digital pin 4
 * LCD Enable pein to digital pin 5
 * LCD D4 pin to digital pin 6
 * LCD D5 pin to digital pin 7
 * LCD D6 pin to digital pin 8
 * LCD D7 pin to digital pin 9
 * LCD R/W pin to ground, VO pin to ground (pin 3)
 */

//////////////////////////////////////// VALORES MEDICION //////////////////////////////////////// 

float VALOR_DHT1_T = 0;  // variable para guardar valor sensor 1 DE TEMPERATURA
float VALOR_DHT1_H = 0;  // variable para guardar valor sensor 1 DE HUMEDAD

float VALOR_DHT2_T = 0;  // variable para guardar valor sensor 2 DE TEMPERATURA
float VALOR_DHT2_H = 0;  // variable para guardar valor sensor 2 DE HUMEDAD

float VALOR_LM35_1 = 0;  // variable para guardar valor sensor 3 DE TEMPERATURA
float VALOR_LM35_2 = 0;  // variable para guardar valor sensor 4 DE TEMPERATURA

//////////////////////////////////////// VALORES DE CALIBRACION ////////////////////////////////////////
byte CAL_DHT1_T = 100;    // valor de calibracion sensor 1 Temperatura
byte CAL_DHT1_H = 100;    // valor de calibracion senosr 1 Humedad

byte CAL_DHT2_T = 100;    // valor de calibracion sensor 2 Temperatura
byte CAL_DHT2_H = 100;    // valor de calibracion senosr 2 Humedad

// VALORES DE CALIBRACION
byte CAL_LM35_1 = 100;    // valor de calibracion sensor 1
byte CAL_LM35_2 = 100;    // valor de calibracion senosr 2

//////////////////////////////////////// VARIABLES ////////////////////////////////////////
unsigned long tiempo = 0;         // PARA LA MEDICION PERIODICA DE LOS SENSORES

unsigned long tiempo_lcd = 0;     // PARA el envio de informacion al lcd

unsigned long tiempo_eeprom = 0;  // PARA el envio de informacion al lcd
// variables activacion alarmas
bool LED_ROJO_ON = true ;    // variabLe activaccion alarma ROJA
bool LED_VERDE_ON = true ;   // variabLe activaccion alarma VERDE
bool BUZZER_ON = true ;      // variabLe activaccion alarma BUZZER

// variables de configuracion temperaturas
int T_MAX_CRIT = 1 ;         // variable de temperatura antes de que se biodegraden las vacunas
int T_MAX_ESTA = 1 ;         // variable de temperatura antes requerir accion de enfriamiento
int T_MIN_ESTA = 1 ;         // variable de temperatura antes requerir accion de calentamiento
int T_MIN_CRIT = 1 ;         // variable de temperatura antes de que se congelen las vacunas

byte T_MAX = 0;              // temperatura maxima registrada
byte T_MIN = 255;              // temperatura minima registrada

// auxiliares menu de opciones
unsigned long tiempo_menu = 0;    // lo uso para espera run tiempo para salir del menu
byte menu = 0;                     // variable que me permite detectar en que capa del menu estoy
int pos_menu = 0;
int pos_sub_menu = 0;
int sub_menu_max = 0;

// variable para los botones
unsigned long tiempo_bot = 0; // lo uso para configurar el tiempo de muestreo de los botones
byte boton = 0;               // variable que  me indica que boton ha sido presionado

// uxiliares para mostrar datos en el lcd
bool aux1 = true, aux2 = true, aux3 = true, aux4 = true;
float valor = 0;


//////////////////////////////////////// SETUP ////////////////////////////////////////
void setup() {
  Serial.begin(9600); 
  iniciar_dht(); // incio los dht y cargo de la eeprom sus valores de calibracion
  
  inciar_alarmas(); // configuro los puertos de las alarmas y leeo sus estados de la eeprom
  
  // inicializo diplay lcd 16x2
  lcd.begin(16, 2);
  
  
  lcd.setCursor(0, 0);
  lcd.print("    iniciando ... ");
  delay(200); 
  ////////////////// configuracion de los valores en eeprom //////////////////
  // cargo los valores de calibracion de los sensores  
  //escribir_cal_lm35(100,100);
  //escribir_cal_dht(100,100,100,100);
  // activo las alarmas por defecto
  //escribir_alarmas(1,1,1);
  // escribo los rangos de temperatura por defecto
  //escribir_cal_temp(8,7,3,2);
  // escrivo los valores maximos y minimos
  //escribir_temp_lim(0,255);
  
  // leo de la eeprom los valores limites guardados y los rangos de temperatura dados
  leer_temp_lim();
  leer_cal_temp();
  // leo de la eeprom los valores de calibracion de los lm35
  leer_cal_lm35();
}
//////////////////////////////////////// LOOP ////////////////////////////////////////
void loop() {
  //mostrar_dht();
  //mostrar_lm35();
  //mostrar_cal_temp();
  //mostrar_temp_lim();
  //mostrar_med_botones();
  
  
  //////////////// obtengo temperatura maxima y minima interna  ////////////////
  if(VALOR_DHT1_T > T_MAX){T_MAX = VALOR_DHT1_T;}
  if(VALOR_LM35_1 > T_MAX){T_MAX = VALOR_LM35_1;}
  if(VALOR_LM35_2 > T_MAX){T_MAX = VALOR_LM35_2;}
  
  if(VALOR_DHT1_T < T_MIN){T_MIN = VALOR_DHT1_T;}
  if(VALOR_LM35_1 < T_MIN){T_MIN = VALOR_LM35_1;}
  if(VALOR_LM35_2 < T_MIN){T_MIN = VALOR_LM35_2;}

  //////////////// activo alarmas ////////////////
  float valor = (VALOR_DHT1_T+VALOR_LM35_1+VALOR_LM35_2)/3;
  if (valor > T_MIN_ESTA && valor < T_MAX_ESTA)
    alarma_OK();
  else if (valor >= T_MIN_CRIT && valor <= T_MAX_CRIT)
    alarma_BOM();
  if (valor < T_MIN_CRIT || valor > T_MAX_CRIT)
    alarma_CRI(); 
  
  //////////////// guardo valore en EEPROM cada cierto tiempo ////////////////
  // guardo valores cada minuto
  if (millis()>(tiempo_eeprom+60000)){
    // cargo los valores de calibracion de los sensores  
    escribir_cal_lm35(CAL_LM35_1,CAL_LM35_1);
    escribir_cal_dht(CAL_DHT1_T,CAL_DHT1_H,CAL_DHT2_T,CAL_DHT2_H);
    // activo las alarmas por defecto
    escribir_alarmas(LED_VERDE_ON,LED_ROJO_ON,BUZZER_ON);
    // escribo los rangos de temperatura por defecto
    escribir_cal_temp(T_MAX_CRIT,T_MAX_ESTA,T_MIN_ESTA,T_MIN_CRIT);
    // escrivo los valores maximos y minimos
    escribir_temp_lim(T_MAX,T_MIN);
    tiempo_eeprom = millis();
  }
  
  //////////////// menu y configuraciones ////////////////
  // inicializo los vectores de string para los menus
  
  if (menu > 0){
    // si estoy en la capa 1 imprimo el menu principal
    if (menu == 1){
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 0);
      switch (pos_menu) {
       case 0:
          lcd.print(" Calibrar Sensores  ");
       break;
       case 1:
          lcd.print("  Calibrar Rangos   ");
       break;
       case 2:
          lcd.print("       Alarmas      ");
       break;
       default:
          lcd.print("  Valores Limites   ");
      }
      
      if (boton == 2){boton=0;(pos_menu>0)?(pos_menu = pos_menu-1)%4:pos_menu = 3;}
      if (boton == 3){boton=0;pos_menu = (pos_menu+1)%4;}
      // salto a los sub menus
      if (boton == 1){boton=0; menu = 2; pos_sub_menu = 0;}
      
    } // fin impresion menu principal
    
    // entro al sub menu respectivo
    if (menu > 1){
      switch (pos_menu) {
       case 0: // Calibrar Sensores
          sub_menu_max = 7;
          sub_calibrar_sensores(pos_sub_menu); // 7 sub menus
       break;
       case 1: // Calibrar Rangos
          sub_menu_max = 5;
          sub_calibrar_rangos(pos_sub_menu); // 5 sub menus
       break;
       case 2: // Alarmas 
          sub_menu_max = 5;
          sub_alarmas(pos_sub_menu);  // 5 sub menus
       break;
       default: // Valores Limites
          sub_menu_max = 4;
          sub_valores_limites(pos_sub_menu);   // 4 sub menus
      }
      delay(50);
      // navego en el sub menu
      if (menu == 2){
        if (boton == 2){boton=0;(pos_sub_menu>0)?(pos_sub_menu = pos_sub_menu-1)%sub_menu_max:pos_sub_menu = sub_menu_max-1;}
        if (boton == 3){boton=0;pos_sub_menu = (pos_sub_menu+1)%sub_menu_max;}
        
        if (boton == 1){boton=0; menu = 3; valor = 0;} // activo la edicion del sub menu, inicio valor de calibracion en 0
      }
    }
    
  } // valores menu
  
  // ------------------------------------------------------------------------------------------------------------------- 
   
  Serial.println(); // IMPRIMO SALTO DE LINEA PARA VISULIZAR FACILMENTE
  
  
  //////////////// reviso el estado de los pulsadores ////////////////
  if (millis()>(tiempo_bot+300)){
    revisar_botones();
    if (boton > 0){if(menu == 0){menu = 1;boton=0;}tiempo_menu = millis();beep(5);}
    tiempo_bot = millis();
  }
  
  // espero un tiempo luego de que se presiona un boton, antes de salirme del menu 
  if (millis()>(tiempo_menu+5000)){
    menu = 0;  // salgo del menu  
    pos_menu = 0;
  }
  
  //////////////// muestro valores de sensores en el display solo si no estoy en el menu ////////////////
  if (millis()>(tiempo_lcd+2000) && menu == 0){
    // mostrar datos tempertatura
    if(aux1){
      lcd.setCursor(0, 0);
      lcd.print("   Estado Interno   ");
      aux1 = false; aux2 = true; aux3 = false;
    }
   
    if(aux3){
      lcd.setCursor(0, 0);
      lcd.print("   Estado Externo   ");
      lcd.setCursor(0, 1);
      lcd.print("T:" + String(VALOR_DHT2_T) + " H:" + String(VALOR_DHT2_H));
      aux3 = false;
      aux1 = true;
    } 
    
    if(aux2 && !aux1){
      lcd.setCursor(0, 1);
      lcd.print("T1:" + String(VALOR_DHT1_T) + " H:" + String(VALOR_DHT1_H));
      aux2 = false;
    }else if (!aux1){
      lcd.setCursor(0, 1);
      lcd.print("T2:" + String(VALOR_LM35_1) + " T3:" + String(VALOR_LM35_2));
      aux3 = true;
    }
    tiempo_lcd = millis(); // actualizo el ultimo tiempo de medicion  
  } // fin muestro lcd
  
  // FIN CODIGO --- PROCEDO A MEDIR LOS SENSORES
  delay(10);
  // mido sensores cada 2 segundos
  if (millis()>(tiempo+2000)){
    medir_dht11();
    medir_lm35();
    tiempo = millis(); // actualizo el ultimo tiempo de medicion  
  }
}  //////////////////////////////////////// FIN LOOP ////////////////////////////////////////
