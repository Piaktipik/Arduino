
// --------------- variables ---------------
const int tiempo = 100;       // timepo en 1 y 0
const int num_pines = 55;    // numero de puertos digitales

// --------------- configuraciones iniciales ---------------
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // initialize the digital pin as an output.
  for(int i=0;i <= num_pines;i++){pinMode(i, OUTPUT);}
}

// --------------- ciclo infinito ---------------
void loop() {
  // apago todos los puertos
  for(int i=0;i <= num_pines;i++){digitalWrite(i,LOW);Serial.println(i);delay(tiempo);}
  // prendo todos los puertos
  for(int i=0;i <= num_pines;i++){digitalWrite(i,HIGH);Serial.println(i);delay(tiempo);}
}
