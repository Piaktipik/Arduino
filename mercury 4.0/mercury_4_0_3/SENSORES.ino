
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz infrarrojos de pista zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// -------------------- variables fisicas del sensor -------------------- 
const byte sen_linea[] = {2,3,4,5,10,15,14,16};         // pines digitales del sensor en orden
unsigned long tiem_refl[8];                             // varialbe para calculo de la medida de la reflexion
int med[8];                                             // contiene la medida de la relfexion
int med_h[8] = {0,0,0,0,0,0,0,0};                       // valor maximo del sensor -> negro
int med_l[8]={5000,5000,5000,5000,5000,5000,5000,5000}; // valor minimo del sensor -> blanco
int hist = 500;
// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz funciones zzzzzzzzzzzzzzzzzzzzzzzzzzzzz 
// se calibra el color mas reflexivo detectado por los sensores
void calibrar_linea(){
  for(int i=0;i < 50;i++){
    medirlinea();
    for(int j=0;j<8;j++){
      if(med[j] < med_l[j] && med[j] != 0){med_l[j] = med[j];}
    }
  }
}

// -------------------- medicion de la linea -------------------- 
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
    med[i] = 5000;           // pongo el valor de medida en maximo
  }
  
  //  ------------ espero a lo sumo 5000 a que  todos los sensores respondan
  unsigned long cont = micros();  // obtengo el tiempo de inicio
  // espero a lo sumo el timepo base + 5000 us
  while(cont + 5000 > micros()){
    for(int i=0;i<8;i++){
      // pregunto por el estado de cada puerto, si esta es 0 y no se ha medido
      if(digitalRead(sen_linea[i]) == 0 && med[i] == 5000){
        // capturo como medida el valor actual - el valor desde el incio de la medicion
        med[i] = micros()-tiem_refl[i];
      }
    }
  }
}


float calcular_linea(){
  float posicion;
  // calculamos la posible posicion de la linea a partir de su poacicion en el mapa
  // usamos el promedio de los pesos como valor real de la linea
  float sum = 0;
  for(int i=0;i<8;i++){
    // med[i] = 5000 - med[i];  // negacion de los valores
    // med[i] = med[i] - med_l[i];  // utilizo a posicion como auxiliar
    //if (posicion > 0)
    sum += med[i];
  }
    
  //posicion = (1.0*(med[7]-med[0])+0.75*(med[6]-med[1])+0.5*(med[5]-med[2])+0.25*(med[4]-med[3]))/sum;
  //posicion = (2*(med[7]-med[0])+1.5*(med[6]-med[1])+1*(med[5]-med[2])+0.50*(med[4]-med[3]))/sum;
  
  posicion = (1.0*(med[7]-med[0])+0.7143*(med[6]-med[1])+0.4286*(med[5]-med[2])+0.1429*(med[4]-med[3]))/sum;
  
  // si tenemos la linea saliendo por un extremo encontramos cual
  if (med[0] > (med[7] + hist) && abs(posicion) > 0.4){ ult_i = true;ult_d = false;}  // izquierda
  if (med[7] > (med[0] + hist) && abs(posicion) > 0.4){ ult_d = true;ult_i = false;}  // derecha

  // funcion para detectar por donde se midio linea por ultima vez antes de salirce de la pista
  // si el robot vuelve a la linea se desactiva la bandera de salida por los lados
  if ((med[5] > (med[7] + hist)||med[4] > (med[7] + hist)||med[6] > (med[7] + hist)) && abs(posicion) < 0.4){ ult_i = false; ult_d = false;}  // izquierdo
  if ((med[1] > (med[0] + hist)||med[2] > (med[0] + hist)||med[3] > (med[0] + hist)) && abs(posicion) < 0.4){ ult_i = false; ult_d = false;}  // derecha
  
  return posicion;
}


void mostar_linea(){
  for(int i=0;i<8;i++){
    Serial.print(med[i]);
    Serial.print(",");
  }
}

