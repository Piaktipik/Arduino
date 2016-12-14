void Principal() {
  
  // vista del funcionamiento de los encoders
  if(true){
    delay(100);
    Serial.print("Ca1= " + String(ca1));
    Serial.print(" , Cb1= " + String(cb1));
    Serial.print(" , Est_enc1= " + String(est_enc1));
    Serial.print(" , pas_tot1= " + String(pas_tot1));
    Serial.print("\t");
    Serial.print("Ca2= " + String(ca2));
    Serial.print(" , Cb2= " + String(cb2));
    Serial.print(" , Est_enc2= " + String(est_enc2));
    Serial.print(" , pas_tot2= " + String(pas_tot2));
    Serial.print("\t");
    Serial.print("Ca3= " + String(ca3));
    Serial.print(" , Cb3= " + String(cb3));
    Serial.print(" , Est_enc3= " + String(est_enc3));
    Serial.print(" , pas_tot3= " + String(pas_tot3));
    Serial.print("\t");
    Serial.print("Ca4= " + String(ca4));
    Serial.print(" , Cb4= " + String(cb4));
    Serial.print(" , Est_enc4= " + String(est_enc4));
    Serial.println(" , pas_tot4= " + String(pas_tot4));
  }
  //mover1g(90);
  
  // recivo comandos por el xbee serial
  if (Serial.available() > 0) {
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
  
  /*
  if((tiempop+50) < millis()){
    tiempop = millis();
    q1 = (q4T1[i]/pi)*180;
    i = (i+1)%100;
  }
  */
  if(false){
//    Input = pasosgra1*pas_tot1;
//    Setpoint = q1;
//    myPID.Compute();
//    velo_moto_1 = (255 - Output);
//    mantenerq1();
//    Serial.print(String(Input));
//    Serial.print(", " + String(q1));
//    Serial.println(", " + String(Output));
  }

  if(false){
//    Input = pasosgra2*pas_tot2;
//    Setpoint = q2;
//    myPID.Compute();
//    velo_moto_2 = (255 - Output);
//    mantenerq2();
//    Serial.print(String(Input));
//    Serial.print(", " + String(q2));
//    Serial.println(", " + String(Output));
  }
  
  //if(pas_tot1 > set_point+offset){velocidad1((pas_tot1-set_point)*40);atras1();}
  //else if(pas_tot1 < set_point-offset){velocidad1((pas_tot1-set_point)*40);adelante1();}
  //else{parar1();}
}  // fin loop

