
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// -------------------- variables fisicas del sensor -------------------- 
const byte sen_linea[] = {2,3,4,5,10,15,14,16};         // pines digitales del sensor en orden
unsigned long tiem_refl[8];                             // varialbe para calculo de la medida de la reflexion
int med[8];                                             // contiene la medida de la relfexion
//int med_h[8] = {0,0,0,0,0,0,0,0};                       // valor maximo del sensor -> negro
int med_l[8]={2000,2000,2000,2000,2000,2000,2000,2000}; // valor minimo del sensor -> blanco
int hist = 1000;
bool calibrado = 0;
int tmax = 2000; 
//float ult_pos_max = 0;                                  // lo uso para saber como vario la linea con respescto al valor anterior

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
// se calibra el color mas reflexivo detectado por los sensores
void calibrar_sensores(){
  // tomamos 50 medidas y obtenemos el valor minimo, ya el maximo lo tomamos como 2000
  for(int i=0;i < 50;i++){
    medirlinea();
    for(int j=0;j<8;j++){
      if(med[j] < med_l[j] && med[j] != 0){med_l[j] = med[j];}  // guardo el valor mas pequeño de cada sensor
    }
    delay(10); // espero un instante entre mediciones
  }
//  motor(0,0.5); // configuracion motores (motor 0-A,1-B, direccion y velocidad valor = {-1,1})
//  motor(1,-0.5);
  calibrado = 1; // ya tengo el vector de calibracion 
}

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz preguntamos si estamos sobre la linea zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
bool hay_linea(){
  for(int j=0;j<8;j++){
    if(med[j] > 1200){return true;}
  }
  return false;
}


// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz se promedian los ultimos 5 valores obtenidos zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz  
void leer_prom(int npro){
  int med_p[8] = {0,0,0,0,0,0,0,0}; 
  // leeo y sumo las proximos npro mediciones  
  for(int i=0;i < npro;i++){
    medirlinea();
    for(int j=0;j<8;j++){
      med_p[j] += med[j];
    }
  }
  for(int j=0;j<8;j++){med[j] = int(float(med_p[j])/npro);} // renicio el vector de promedio   
}



// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz medimos los sensores zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
void medirlinea(){
  // ------------ cargo los condesandores poniendo en alto los pines
  for(int i=0;i<8;i++){
    pinMode(sen_linea[i],OUTPUT);digitalWrite(sen_linea[i],HIGH);
  }

  // ------------ espero minimo 10us a que cargue el capacitor de los sensores
  delayMicroseconds(20);
  // pongo los pines como entrada y espero a la interrupcion
  for(int i=0;i<8;i++){
    pinMode(sen_linea[i],INPUT);
    tiem_refl[i] = micros(); // tomo el tiempo de inicio de la medicion
    med[i] = tmax;           // pongo el valor de medida en maximo
  }
  
  //  ------------ espero a lo sumo 5000 a que  todos los sensores respondan
  unsigned long cont = micros();  // obtengo el tiempo de inicio
  // espero a lo sumo el timepo base + 5000 us
  while(cont + tmax > micros()){
    for(int i=0;i<8;i++){
      // pregunto por el estado de cada puerto, si esta es 0 y no se ha medido
      if(digitalRead(sen_linea[i]) == 0 && med[i] == tmax){
        // capturo como medida el valor actual - el valor desde el incio de la medicion
        med[i] = micros()-tiem_refl[i];
      }
    }
  }
  
  // luego de tener la medicion la calibro en caso de ya tener valores de calibracion
  if (calibrado == 1){
    for(int i=0;i<8;i++){
      int den = tmax - med_l[i];    // calculo el valor para linealizar la medicion
      if(den != 0){
        int x = (float(med[i]-med_l[i])/den)*tmax;   // tomo la medicion y le resto su valor minimo, luego lo aplifico 1000 para que tenga parte entera y lo divido para normalizarlo
        if(x < 0){x = 0;}
        else if(x > tmax){x = tmax;}
        med[i] = x;
      }
    }
  }// fin si esta calibrado
  
} // fin medir linea

// zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz calculamos linea zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
float calcular_linea(){
  // calculamos la posible posicion de la linea a partir de su pocicion en el mapa
  
  // calculamos la sumatoria de todas la mediciones
  float sum = 0;
  for(int i=0;i<8;i++){
    sum += med[i];
  }
  
  // se calcula la posicion de la linea a travez de los pesos de la medicion
  float posicion;
  posicion = (1.0*(med[7]-med[0])+0.7143*(med[6]-med[1])+0.4286*(med[5]-med[2])+0.1429*(med[4]-med[3]))/sum;

  // direccion de movimiento del robot respecto a la linea
  if(hay_linea()){
    if (posicion > 0){izquierda = false;}
    else{izquierda = true;}
  }
  
  return posicion;
}


// muestro la ultima medicion guardada tomada de los sensores
void mostar_linea(){
  for(int i=0;i<8;i++){
    Serial.print(med_l[i]);
    Serial.print(",");
  }
  for(int i=0;i<8;i++){
    Serial.print(med[i]);
    Serial.print(",");
  }
}


