//por: Julian Galvez Serna
//para: sensor ss49e (linear hall-efect sensor)
//----------------------------------------------------------------
int sensorPin = A0;    // select the input pin for the potentiometer
//int ledPin = 13;      // select the pin for the LED
float hall = 0;  // variable to store the value coming from the sensor
int inByte = 0;         // incoming serial byte

void setup() {
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);  
}

void loop() {
  // read the value from the sensor:
  hall = (analogRead(sensorPin)-517)*3.5;
  //hall = (hall*1000/1.4);
  //1.4mv/G
  Serial.println(hall);                      
}
