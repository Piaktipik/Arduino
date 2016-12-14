
xbee_1__boya_ca

const int ledPin = 13;       // pin del led
const int buttonPin = 2;     // the number of the pushbutton pin

int buttonState = 0;         // variable for reading the pushbutton status
byte potenciometro = 100;         // variable for reading the pushbutton status

void setup(){
  
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600); 
  pinMode(ledPin, OUTPUT);   // digital sensor is on digital pin 2
  pinMode(buttonPin, INPUT);
}

void loop(){
  //--------------------------para el boton---------------------
  // leo el boton
  buttonState = digitalRead(buttonPin);
  // evio 1/0 segun el estado del boton
  if (buttonState == HIGH) {     
    // turn LED on:    
    Serial.print("1");  
  } 
  else {
    // turn LED off:
    Serial.print("0"); 
  }
  //--------------para el la llegada del potenciometro---------
  if (Serial.available() > 0) {
    // get incoming byte:
    potenciometro = Serial.read();
    //Serial.println(potenciometro);
  } 
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);  
  // stop the program for <sensorValue> milliseconds:
  delay(potenciometro);          
  // turn the ledPin off:        
  digitalWrite(ledPin, LOW);   
  // stop the program for <sensorValue> milliseconds:
  delay(potenciometro);
}
