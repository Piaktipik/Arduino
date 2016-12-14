void Setup() {

  // ----------------- comunicaciones seriales -----------------
  Serial.begin(115200);       // comunicacion con el pc - pruebas
  
  activarEncoder1(pinA1,pinB1);
  activarEncoder2(pinA2,pinB2);
  activarEncoder3(pinA3,pinB3);
  activarEncoder4(pinA4,pinB4);
  
  // ----------------- motores -----------------
  // se declaran como salidas
  pinMode(pin_dire_moto_1, OUTPUT);
  pinMode(pin_velo_moto_1, OUTPUT);
  pinMode(pin_dire_moto_2, OUTPUT);
  pinMode(pin_velo_moto_2, OUTPUT);
  pinMode(pin_dire_moto_3, OUTPUT);
  pinMode(pin_velo_moto_3, OUTPUT);
  pinMode(pin_dire_moto_4, OUTPUT);
  pinMode(pin_velo_moto_4, OUTPUT);
  
  pinza.attach(38);
  abrir();

  parar1(); // inicializo el robot con los motores quietos
  parar2(); // inicializo el robot con los motores quietos
  parar3(); // inicializo el robot con los motores quietos
  parar4(); // inicializo el robot con los motores quietos
  
  //myPID.SetOutputLimits(0, 255);
  //myPID.SetMode(AUTOMATIC);
  
}
