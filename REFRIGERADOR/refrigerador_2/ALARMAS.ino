
//////////////////////////////////////// puertos alarmas y botones ////////////////////////////////////// 
const byte PIN_LED_VERDE = 11; // PIN DEL LED VERDE
const byte PIN_LED_ROJO = 10;  // PIN DEL LED ROJO
const byte PIN_BUZZER = 2;     // PIN DEL BUZZER

unsigned long t_led_verde = 0;    // para emitir las alarmas sin detener el programa
bool e_led_verde = 0;             // para la intermitencia de la alarma

unsigned long t_led_rojo = 0;    // para emitir las alarmas sin detener el programa
bool e_led_rojo = 0;             // para la intermitencia de la alarma

void inciar_alarmas(){
  // declaro los puertos como salidas
  pinMode(PIN_LED_VERDE,OUTPUT);
  pinMode(PIN_LED_ROJO,OUTPUT);
  pinMode(PIN_BUZZER,OUTPUT);
  // prendo rapidamente y apago los puertos
  digitalWrite(PIN_LED_VERDE,HIGH);
  digitalWrite(PIN_LED_ROJO,HIGH);
  digitalWrite(PIN_BUZZER,HIGH);
  delay(100);
  digitalWrite(PIN_LED_VERDE,LOW);
  digitalWrite(PIN_LED_ROJO,LOW);
  digitalWrite(PIN_BUZZER,LOW);
  
  leer_alarmas();
}


void leer_alarmas(){
  (EEPROM.read(6) > 0) ? LED_ROJO_ON=true : LED_ROJO_ON=false;
  (EEPROM.read(7) > 0) ? LED_VERDE_ON=true : LED_VERDE_ON=false;
  (EEPROM.read(8) > 0) ? BUZZER_ON=true : BUZZER_ON=false;
}

void escribir_alarmas(bool led_verde, bool led_rojo, bool buzzer){
  if (EEPROM.read(6) != led_rojo) EEPROM.write(6,led_rojo);
  if (EEPROM.read(7) != led_verde) EEPROM.write(7,led_verde);
  if (EEPROM.read(8) != buzzer) EEPROM.write(8,buzzer);
  
  leer_alarmas();
}

void apagar_alarmas(){
  digitalWrite(PIN_LED_VERDE,LOW);
  digitalWrite(PIN_LED_ROJO,LOW);
  digitalWrite(PIN_BUZZER,LOW);
}

// alarma de que todo esta bien
void alarma_OK(){
  if(!LED_VERDE_ON){return;digitalWrite(PIN_LED_VERDE,LOW);}
  if (millis()>(t_led_verde+1000)){
    digitalWrite(PIN_LED_VERDE,e_led_verde);
    e_led_verde = !e_led_verde;
    t_led_verde = millis();
    digitalWrite(PIN_LED_ROJO,LOW);
    digitalWrite(PIN_BUZZER,LOW);
  }
}

// alarma de que todo esta bien
void alarma_BOM(){
  if (millis()>(t_led_rojo+1000)){
    if(LED_ROJO_ON){digitalWrite(PIN_LED_ROJO,e_led_rojo);}
    if(BUZZER_ON){beep(10);}
    e_led_rojo = !e_led_rojo;
    t_led_rojo = millis();
    digitalWrite(PIN_LED_VERDE,LOW);
  }
}

void alarma_CRI(){
  if (millis()>(t_led_rojo+1000)){
    if(LED_ROJO_ON){digitalWrite(PIN_LED_ROJO,e_led_rojo);}
    if(BUZZER_ON){digitalWrite(PIN_BUZZER,e_led_rojo);}
    e_led_rojo = !e_led_rojo;
    t_led_rojo = millis();
    digitalWrite(PIN_LED_VERDE,LOW);
  }
}

void beep(int TIEMP){
  digitalWrite(PIN_BUZZER,HIGH);
  delay(TIEMP);
  digitalWrite(PIN_BUZZER,LOW);
}
