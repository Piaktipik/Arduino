//Variable declaration
const int pirPin = 2; //Variable that will receive the digital signal from the sensor 

const int aviso = 13;
const int rec = 3;

unsigned long tieGra = 2000;
unsigned long auxtiem = 0;

void setup(){

 Serial.begin(9600);      // set the baud rate serial with 9600 

 pinMode(pirPin, INPUT);  // declares variable pirPin as an input
 pinMode(rec, INPUT);
 pinMode(aviso, OUTPUT);
}

void loop(){

 int pirVal = digitalRead(pirPin); // Leemos el sensor pir
 Serial.print(pirVal + ":"); // mostramos la medicion del sensor pir
 
  int recVal = digitalRead(rec); // Leemos el sensor pir
  Serial.println(recVal);
  
  // verificamos que no se este grabando un mensaje
  if(recVal != 1){
    // iniciamos la gravacion
    digitalWrite (aviso, LOW);
    auxtiem = millis();
    delay(500);
    while(recVal != 1){
      recVal = digitalRead(rec); // Leemos el sensor pir
      delay(10);
    }
    tieGra = millis()-auxtiem;
    digitalWrite (aviso, HIGH); // desactivamos el aviso
  }
  
  else if(pirVal != 1){ // Low value means that a motion was detected
    Serial.println("Motion Detected"); // print at the serial monitor the words: Motion Detected 
    digitalWrite (aviso, LOW);
    delay(tieGra);
   }
   if(recVal != 0){
    digitalWrite (aviso, HIGH); // desactivamos el aviso
   }
  delay(100); // wait for 2 seconds before compare again

}

