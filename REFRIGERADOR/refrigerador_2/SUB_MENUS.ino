

void sub_calibrar_sensores(int POSICION){
  lcd.setCursor(0, 0);
  switch (POSICION) {
    case 0:
      lcd.print(" Sensor Temp Int 1: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_DHT1_T)+"       ");
        CAL_DHT1_T = CAL_DHT1_T + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_DHT1_T)+"       ");
      } 
    break;
    case 1:
      lcd.print("  Sensor Hum Int :  ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_DHT1_H)+"       ");
        CAL_DHT1_H = CAL_DHT1_H + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_DHT1_H)+"        ");
      }
     break;
     case 2:
      lcd.print(" Sensor Temp Int 2: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_LM35_1)+"       ");
        CAL_LM35_1 = CAL_LM35_1 + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_LM35_1)+"       ");
      }
     break;
     case 3:
      lcd.print(" Sensor Temp Int 3: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_LM35_2)+"       ");
        CAL_LM35_2 = CAL_LM35_2 + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_LM35_2)+"       ");
      }
     break;
     case 4:
      lcd.print("  Sensor Temp Ext : ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_DHT2_T)+"       ");
        CAL_DHT2_T = CAL_DHT2_T + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_DHT2_T)+"       ");
      } 
     break;
     case 5:
      lcd.print("  Sensor Hum Ext :  ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(VALOR_DHT2_H)+"       ");
        CAL_DHT2_H = CAL_DHT2_H + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(VALOR_DHT2_H)+"       ");
      } 
     break;
     default:
      lcd.print("      Regresar:     ");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      //if (menu == 3){
        if (boton == 1){boton=0;beep(5); menu = 1;} // guardo y regreso al sub menu
      //}
  }
  
  if (menu == 3){
    if (boton == 2){boton=0;beep(5); valor = -1;}
    if (boton == 3){boton=0;beep(5); valor = 1;}
    if (boton == 1){boton=0;beep(5); menu = 2;} //regreso al sub menu
  }
  
}


void sub_calibrar_rangos(int POSICION){
  lcd.setCursor(0, 0);
  switch (POSICION) {
    case 0:
      lcd.print(" Valor Max Critico: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(T_MAX_CRIT)+"       ");
        T_MAX_CRIT = T_MAX_CRIT + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(T_MAX_CRIT)+"       ");
      } 
    break;
    case 1:
      lcd.print(" Valor Max Estable: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(T_MAX_ESTA)+"       ");
        T_MAX_ESTA = T_MAX_ESTA + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(T_MAX_ESTA)+"        ");
      }
     break;
     case 2:
      lcd.print(" Valor Min Estable: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(T_MIN_ESTA)+"       ");
        T_MIN_ESTA = T_MIN_ESTA + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(T_MIN_ESTA)+"       ");
      }
     break;
     case 3:
      lcd.print(" Valor Min Critico: ");
      lcd.setCursor(0, 1);
      if(menu == 3){
        lcd.print("Editar:" + String(T_MIN_CRIT)+"       ");
        T_MIN_CRIT = T_MIN_CRIT + valor;
        valor = 0;
      }else{
        lcd.print("   " + String(T_MIN_CRIT)+"       ");
      }
     break;
     default:
      lcd.print("      Regresar:     ");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      //if (menu == 3){
        if (boton == 1){boton=0;beep(5); menu = 1;} // guardo y regreso al sub menu
      //}
  }
  
  if (menu == 3){
    if (boton == 2){boton=0;beep(5); valor = -1;apagar_alarmas();}
    if (boton == 3){boton=0;beep(5); valor = 1;apagar_alarmas();}
    if (boton == 1){boton=0;beep(5); menu = 2;} // regreso al sub menu
  }
  
}

void sub_alarmas(int POSICION){
  lcd.setCursor(0, 0);
  switch (POSICION) {
    case 0:
      lcd.print("   Alarma Sonora :   ");
      lcd.setCursor(0, 1);
      if (BUZZER_ON){
        lcd.print("      Activada     ");
      }else{
        lcd.print("     Desactivada   ");
      }
      if(menu == 3){lcd.setCursor(0, 1);lcd.print("Ed: ");}
      if (valor != 0){BUZZER_ON = !BUZZER_ON;apagar_alarmas();}
      valor = 0;
    break;
    case 1:
      lcd.print(" Alarma Led Verde : ");
      lcd.setCursor(0, 1);
      if (LED_VERDE_ON){
        lcd.print("      Activada     ");
      }else{
        lcd.print("     Desactivada    ");
      }
      if(menu == 3){lcd.setCursor(0, 1);lcd.print("Ed: ");}
      if (valor != 0){LED_VERDE_ON = !LED_VERDE_ON;apagar_alarmas();}
      valor = 0;
     break;
     case 2:
      lcd.print(" Alarma Led Rojo :  ");
      lcd.setCursor(0, 1);
      if (LED_ROJO_ON){
        lcd.print("      Activada     ");
      }else{
        lcd.print("     Desactivada    ");
      }
      if(menu == 3){lcd.setCursor(0, 1);lcd.print("Ed: ");}
      if (valor != 0){LED_ROJO_ON = !LED_ROJO_ON;apagar_alarmas();}
      valor = 0;
     break;
     default:
      lcd.print("      Regresar:     ");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      //if (menu == 3){
        if (boton == 1){boton=0;beep(5); menu = 1;} // guardo y regreso al sub menu
      //}
  }
  
  if (menu == 3){
    if (boton == 2){boton=0;beep(5); valor = -1;}
    if (boton == 3){boton=0;beep(5); valor = 1;}
    if (boton == 1){boton=0;beep(5); menu = 2;} // regreso al sub menu
  }
  
}

void sub_valores_limites(int POSICION){
  lcd.setCursor(0, 0);
  switch (POSICION) {
    case 0:
      lcd.print(" Temp Maxima Medida: ");
      lcd.setCursor(0, 1);
        lcd.print("   " + String(T_MAX)+"       ");
    break;
    case 1:
      lcd.print(" Temp Minima Medida: ");
      lcd.setCursor(0, 1);
        lcd.print("   " + String(T_MIN)+"       ");
     break;
     case 2:
      lcd.print(" Reiniciar Valores: ");
      if (menu == 2){
        lcd.setCursor(0, 1);
        lcd.print("                    ");
      }
      if (menu == 3){
        if (boton == 1){
          boton = 0; menu = 2;
          T_MAX=0; T_MIN = 255; // guardo y regreso al sub menu
          //escribir_temp_lim(T_MAX,T_MIN);
          lcd.setCursor(0, 1);
          lcd.print("Valores Reiniciados");
        }
      }
     break;
     default:
      lcd.print("      Regresar:     ");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      //if (menu == 3){
        if (boton == 1){boton=0;beep(5); menu = 1;} // guardo y regreso al sub menu
      //}
  }
  
  if (menu == 3){
    if (boton == 2){boton=0;beep(5); valor = -1;}
    if (boton == 3){boton=0;beep(5); valor = 1;}
    if (boton == 1){boton=0;beep(5); menu = 2;} // regreso al sub menu
  }
  
}

