
char dir = 'C'; // letra que activa la respuesta
byte pin_dir_com = 3;  // pin para el control TX/RX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin_dir_com,OUTPUT);
  digitalWrite(pin_dir_com,LOW); // ponemos la comunicacion en modo escucha
}

void loop() {
  
  if (Serial.available() > 0) {
    datoserial(Serial.read());  // 
  }
}

void datoserial(char dato) {
  //if (dato == dir || dato == 'B') {
  if (dato == dir) {
    unsigned int med = 0;
    med = analogRead(0);
    med = int(100*(float(med)/1024));
    digitalWrite(pin_dir_com,HIGH); // ponemos la comunicacion en modo escritura
    //if (dato == 'B'){med-=1;}
    Serial.print("<");
    Serial.print(med);
    Serial.print(">");
    Serial.flush(); // esperamos a que el dato sea enviado para pasar a modo escucha
    digitalWrite(pin_dir_com,LOW); // ponemos la comunicacion en modo escucha
  }
}
