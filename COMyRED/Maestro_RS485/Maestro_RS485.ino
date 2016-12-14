
char dir[] = {'A','B','C'}; // vector de modulos en la red
int val[sizeof(dir)];
byte pin_dir_com = 2;  // pin para el control TX/RX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(9600);
  pinMode(pin_dir_com,OUTPUT);
  digitalWrite(pin_dir_com,HIGH); // ponemos la comunicacion en modo transmicion
}

void loop() {
  delay(50);
  // leemos todos los modulos adjuntos
  for(byte i=0;i<sizeof(dir);i++){
    //val[i] = 60+i*10;
    val[i] =lee_mod(i,dir[i]);
    //Serial.println(String(val[i])); // pruebas
  }
  // una vez leidos todos los modulos los imprimimos al computador
  //Serial.print("<");
  String aux_est = String(millis());
  for(byte i=0;i<sizeof(val)/2;i++){
    if(i < sizeof(val)){
      aux_est += ",";
    }
    //Serial.print("."+String(i)+".");
    aux_est += String(val[i]);
  }
  //Serial.println(aux_est + ">");
  Serial.println(aux_est); 
}

int lee_mod(int ind,char dato) {
  //Serial.println("lee_mod"); // pruebas
  int res = -1;
  while(res == -1){
    // enviamos la llamada al esclavo correspondiente
    digitalWrite(pin_dir_com,HIGH); // ponemos la comunicacion en modo escritura
    Serial3.print(dato);
    Serial3.flush(); // esperamos a que el dato sea enviado para pasar a modo escucha
    digitalWrite(pin_dir_com,LOW); // ponemos la comunicacion en modo escucha
    // esperamos los datos enviados por serial
    unsigned long tie_bas = millis();
    bool inicio = 0;
    int res_aux = 0;
    
    while((tie_bas+5) > millis()){
      // si hay respuesta serial
      if (Serial3.available() > 0) {
        //leemos el dato
        char dato = (char)Serial3.read();
        // varificamos el dato
        if(inicio == 0 && dato == '<'){
          inicio = 1; // caracter de inicio de trama detectado
          res_aux = 0; // limpiamos la variable que almacena el valor recivido
          res = -1;
        }else if(inicio == 1 && dato != '>'){
          res_aux = res_aux*10 + (int(dato)-48); // recuperamos el valor de 0-100
          
        }else if(inicio == 1 && res_aux != -1 && dato == '>'  ){
          // guardamos el valor
          if(res_aux >= 0 && res_aux <=100){
            res = res_aux;
            res_aux = 0;
            inicio = 0;
            break;
            //Serial.print("<" + String(res) + ">"); // pruebas
          }
          inicio = 0;
        }   
      } // fin dato serial
    } // fin while espera en milisegundos recepcion
  } // fin while res =! -1
  // Serial.print(res);  // pruebas
  return res;
}
