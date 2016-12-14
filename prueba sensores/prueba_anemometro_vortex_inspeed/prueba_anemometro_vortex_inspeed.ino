


// ------------ variables para la lectrura del anemometro ------------
unsigned long tanemometro = 0;    // variable anemometro (tiempo entre pulsos)
float anemometro = 0;             // velocidad anemometro en m/s
unsigned long tantes = 0;         // tiempo a el pulso anterior

// -------- programa principal -------- 
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);                              // configuro como entrada el pin del anemometro
  attachInterrupt(0, medir_tanemometro, RISING);  // abilito interrupcion en pin 2 - acciono la funcion:  medir_tanemometro
}


void loop() {
  
  /* convierto a velocidad
    1 pulso/segundo = 2.5 milla/hora 
    1 milla/hora -> 1.60934 kilometros/hora
    1 kilometro/hora -> 0.277778 metros/segundo 
  */
  anemometro = (1000000.0/tanemometro)*1,117597;
  Serial.print("velocidad = "); 
  Serial.print(anemometro);
  Serial.print(" m/s ");
  Serial.println();
  delay(100);
  
  if((tantes + 10000000) < micros()){
    tanemometro = 1000000000;
    tantes = micros();
  }
}
  
  
// -------- cuenta el tiempo entre pulsos -----------
void  medir_tanemometro(){
  tanemometro = micros() - tantes;    // calculo el tiempo pasado entre la ultima medicion y la actual
  tantes = micros();                  // guardo el tiempo de la ultima medicion
}
  
