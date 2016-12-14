
const byte trigger = 11;
const byte eco = 12;


unsigned int tiempo = 0;
float distancia = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(trigger,OUTPUT);
  pinMode(eco,INPUT);
  // put your setup code here, to run once:
}

void loop() {
  medir_srf_04();
  Serial.println(distancia);
  delay(50);
  // put your main code here, to run repeatedly:
}

void medir_srf_04(){
  // enviamos trigger
  digitalWrite(trigger,LOW);
  delay(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  
  // medir el tiempo en alto
  tiempo = 0;
  tiempo = pulseIn(eco,HIGH,50000);
  if (tiempo != 0){
    distancia = tiempo/58.3;
  }  
}

