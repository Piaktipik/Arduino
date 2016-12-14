int inByte=0;//t1=0,t2=0,t3=0,t4=0,t5=0,
int medidas[5];
void setup(){
  Serial.begin(9600);  //inicializo el puerto serial a 9600
  //pinMode(2, INPUT);   // digital sensor is on digital pin 2
  establishContact();  // send a byte to establish contact until Processing responds 
}
void loop(){
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    inByte = Serial.read();            // get incoming byte:
    for (int i=0;i < sizeof(medidas)-1;i++){Serial.write(analogRead(i)/4);delay(10);}  // send sensor values:  
  }//fin if
  //else{establishContact();} //descomentar para hacer que funcione en todo momento
}
//---------------------------subprogramas-----------------------------------------
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write('A');   // envia A mienstras no se halla establecido conexion
    delay(300);
  }
}//fin void de primer contacto
