//por: julian galvez serna
//--------------------------perifericos------------------
const int pul = 12;     // pulsador de inicio de lectura
const int sen = 4;      // puerto donde esta el sensor infrarrojo
const int tam = 1000;   // tiempo de muestreo en ms
const int res = 100;   // tiempo entre lecturas en us
// can se calcula usando 2000*(res/1000) y deve ser entero
const int can = 10000;   // cantidad de mediciones
//--------------------------variables------------------

String datos;           // vector de datos
boolean aux = 0;        // lo uso para iniciar lectura y enviar datos con el boton
boolean epu = 1;        // guardo el estado del pulsador
int i = 0;              // variable de los for
//--------------------------------configuracion inicial--------------------------------------
void setup() {
    Serial.begin(57600);  // activo el puerto serial para enviar datos
    pinMode(pul,INPUT);  // declaro como entrada el pin del pulsador
    pinMode(sen,INPUT);  // declaro como entrada el pin del lector  
}

//--------------------------------programa principal--------------------------------------
void loop() {
  epu = digitalRead(pul); // leo el estado del pulsador
  // pregunto si el pulsador fue presionado, por primera vez
  if (epu == 0 && aux == 0){
    datos = ""; // limpio el vector de datos
    epu = 1; // limpio el estado del pulsador
    // procedo a tomar datos
    Serial.print('I'); // inicio de lectura
    for(i = 0; i < can; i++){
      // leo el estado del pin
      datos = (datos + digitalRead(sen));
      // espero el tiempo indicado
      delayMicroseconds(res);
    } // fin de la toma de datos
    Serial.println('F'); // fin de lectura
    aux = 1; // la uso para saber que el boton fue presionado una vez
  } // fin de accion ejecutada al presionar el boton por primera vez
  // si el pulsador es presionado por segunda vez
  if (epu == 0 && aux == 1){
    // procedo a enviar los datos por serial
    Serial.print('I'); // indicativo de inicio de transmicion
//    for(i = 0; i < can; i++){
      Serial.print(datos); //evio dato por dato
//    } // fin envio de datos
    Serial.println('F'); // indicativo fin de trasmicion y salto de linea
    aux = 0;
  } // fin envio de datos
  delay(100);
}
