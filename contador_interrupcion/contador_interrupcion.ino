unsigned long time = 0 ;
bool conting = 0;
byte pin_int = 12;

void setup() {
  Serial.begin(9600);
  pinMode(pin_int,INPUT);
}

void loop() {
  if(!conting && digitalRead(pin_int) == 0){
    time = micros();
    conting = true;
  }
  
  if(conting && digitalRead(pin_int) == 0){
    time = micros()-time;
    Serial.println(time);
  }

}
