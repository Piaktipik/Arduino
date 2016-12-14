
void datoserial(char dato) {
  
  if (dato == 'a') {
    adelante1();
    Serial.println('a');
  }
  if (dato == 'b') {
    adelante2();
    Serial.println('b');
  }
  if (dato == 'c') {
    adelante3();
    Serial.println('c');
  }
  if (dato == 'd') {
    adelante4();
    Serial.println('d');
  }

  
  if (dato == 'e') {
    atras1();
    Serial.println('e');
  }
  if (dato == 'f') {
    atras2();
    Serial.println('g');
  }
  if (dato == 'h') {
    atras3();
    Serial.println('h');
  }
  if (dato == 'i') {
    atras4();
    Serial.println('i');
  }
  
  
  if (dato == 'j') {
    parar1();
    Serial.println('j');
  }
  if (dato == 'k') {
    parar2();
    Serial.println('k');
  }
  if (dato == 'l') {
    parar3();
    Serial.println('l');
  }
  if (dato == 'm') {
    parar4();
    Serial.println('m');
  }


  if (dato == 'n') {
    abrir();
    Serial.println("abrir");
  }
  if (dato == 'o') {
    cerrar();
    Serial.println("cerrar");
  }


  if (dato == 'p') {
    mover1g(Serial.parseInt()); 
  }
  if (dato == 'q') {
    mover2g(Serial.parseInt()); 
  }
  if (dato == 'r') {
    mover3g(Serial.parseInt()); 
  }
  if (dato == 's') {
    mover4g(Serial.parseInt()); 
  }
  
  if (dato == 't') {
    q1 = Serial.parseInt(); 
  }

  if (dato == 'u') {
    delay(100);
    velo_moto_1 = Serial.read(); // vel izquierda
    velo_moto_1 = Serial.read(); // vel derecha
  }

  if (dato == 'v') {
    delay(100);
    set_point = int(Serial.read()-48)*1000;
    set_point += int(Serial.read()-48)*100;
    set_point += int(Serial.read()-48)*10;
    set_point += int(Serial.read()-48);
  }
  
}

