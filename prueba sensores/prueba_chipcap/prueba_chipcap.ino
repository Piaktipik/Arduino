
// ------------ variables para la lectrura del chipcap ------------
const int tempin = A4;  // puerto analogo señal de temperatura
const int humpin = A5;  // puerto analogo señal de humedad

int tem = 0;            // valor digital de temperatura
int hum = 0;            // valor digital de humedad
float ftem = 0;         // valor en unidades de temperatura
float fhum = 0;         // valor en unidades de humedad

void setup() {
  // se inicia la comunicacion serial a 9600
  Serial.begin(9600);
}

void loop() {
  // leo los valores analogos
  tem = analogRead(tempin);
  hum = analogRead(humpin);
  // lo paso a voltajes
  ftem = float(tem*5)/1023;
  fhum = float(hum*5)/1023;
  
  // modo radiometrico
  //%RH = ((Vout - (0.1 * Vsupply))/(0.8 * Vsupply)) * 100
  //Tc = ((Vout - (0.1 * Vsupply))/(0.8 * Vsupply)) * 200 - 50
  ftem = ((ftem-(0.5))/(4))*200-50;
  fhum = ((fhum-(0.5))/(4))*100;

  Serial.print("temperatura = " );
  Serial.print(ftem);
  Serial.print("\t humedad = ");
  Serial.print(fhum);
  
  
  Serial.println();
  // espero un par de ms
  delay(2);
}
