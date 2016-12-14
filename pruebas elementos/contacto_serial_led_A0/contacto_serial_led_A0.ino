
// --------- variables --------------

int firstSensor = 0;    // sensor analago
byte led = 13 ;         // puerto del led
int inByte = 0;         // byte de llegada por serial

// ----------- configuracones inciales --------------

void setup(){
  Serial.begin(9600);     // inicializo comunicacion serial 9600
  pinMode(led, OUTPUT);   // se configura el puerto del led
  // ------- espero a establecer conexion -------
  establishContact();     // envio la letra A hasta que halla una respuesta 
}

// ------------- programa principal ---------------

void loop(){
  
  
  
  
  //  si llegan datos por serial
  if (Serial.available() > 0) {
    inByte = Serial.read(); // leeo el valor que llego
    
    if(inByte == 'B'){
      digitalWrite(led, HIGH);
    }
    else{
      digitalWrite(led, LOW);
    }
    
    // leeo el ADC A0, y lo escalo a 0 - 255 -> (1024)/4 :
    firstSensor = analogRead(0)/4;
    // envio el valor como byte -> llegara un caracter
    Serial.write(firstSensor);
    // envio un espacio, luego el valor medido como int y salto de linea
    Serial.print('\t');
    Serial.print(firstSensor);
    Serial.println();    
  }
}


void establishContact() {
  // mientras no encunetre respuesta envio una A cada 300ms
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}
