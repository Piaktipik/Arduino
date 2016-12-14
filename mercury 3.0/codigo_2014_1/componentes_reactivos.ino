  
  // -objetivos: -buscar velas - evadir obstaculos -> (paredes, linea)
  // -variables de entrada 
  // ultrasonidos -> (medi_dist_ultr_dere,medi_dist_ultr_fron,medi_dist_ultr_izqu) der - fre -izq
  // infrarrojos
  //   vela ->     senvela[0 <-> 20] atraz - izq - dereha
  //   pista ->    medi_bool_line[0 <-> 15] derecho - izquierdo
  // - codigo: 
  
  //definir_dir(0); // funcion que uso para definir la direccion de movimiento

// ------------------------------------ detecto lineas ------------------------------------ 
bool detecto_lineas(){ 
  bool veo_line_fren = false;    // supongo que no estoy viendo linea de frente
  bool veo_line_late = false;    // supongo que no estoy viendo linea de lado
  
  byte cont_line_dere = 0;     // variable conteo sensores de linea derechos detectando linea
  byte cont_line_izqu = 0;     // variable conteo sensores de linea izquierdos detectando linea
  
  // cuento las detecciones de el lado derecho
  for(int i=0;i<8;i++){
    if(medi_bool_line[i] == HIGH){cont_line_dere++;}
  }
  // cuento las detecciones de el lado izquierdo
  for(int i=8;i<15;i++){
    if(medi_bool_line[i] == HIGH){cont_line_izqu++;}
  }
  
  if(cont_line_dere > 0 && cont_line_izqu > 0){veo_line_fren = true;}        // si hay linea en frente
  if(cont_line_dere > 1 || cont_line_izqu > 1){veo_line_late = true;}        // si veo con dos sensores linea de lado
  return veo_line_fren,veo_line_late;
}

// ------------------------------------ apago la vela ------------------------------------ 
void apago_vela(){ 
  // si veo vela la apago, y la veo si: estoy a 20cm de un obstaculo y veo una linea de frente y estoy en frente de algo caliente
  if((medi_dist_ultr_fron < 20 && veo_line_fren) && apag_vela){
    atras();delayy(10);                    // freno rapidamente el motor
    parar();fan(1);delayy(tiem_apag);      // me detengo prendo el ventilador y espero el timepo de apagado
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo; // configuro velocidades
    fan(0);atras();delayy(200);            // apago el ventilador marcho atraz por 400 ms
    // voy al lugar de mayor distancia
    
    if(!(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela)){escapar();delayy(200);} // giro solo si la vela esta apagada
  }
}

// ------------------------------------ evado obstaculos ---------------------------------
bool evado_obstaculo(){
  veo_obst = false;    // supongo que no estoy viendo obstaculos
  if(apun_vela){dist_mini_late = 5;}else{dist_mini_late = 15;}      // ajusto la distancia de evacion
  // derecho
  if(medi_dist_ultr_dere < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{izquierda();}veo_obst = true;}
  // izquierdo
  if(medi_dist_ultr_izqu < dist_mini_late) {if(apun_vela){atras();delayy(200);}else{derecha();}veo_obst = true;}
    // frontal
  if(medi_dist_ultr_fron < dist_mini_fron) {atras();delayy(200);escapar();delayy(200);veo_obst = true;}
  
  if (veo_obst && !apag_vela && !veo_line_late){return true;}  // si hay obstaculos y no estoy apagando una vela y no estoy evadiendo linea -> no reviso la vela y evado obstaculo
  return false;
}

// ---------------- evito salirme de la pista o entrar a la vela apagada -----------------
void pista_vela(){
  // retrocedo y giro hacia donde alla detectado menos lineas
  if (veo_line_fren && !apag_vela){
    velo_moto_izqu = velo_esta;velo_moto_dere = velo_esta + valo_ajus_velo;  // configuro velocidad de motores
    atras();delayy(500);          // retrocedo ya que la linea esta de frente
    if(cont_line_dere > cont_line_izqu){izquierda();}else{derecha();}
    delayy(150);      // escapo para el lado con al cual tenga menos sensores en alto 
  }
  
  // giro levemente
  else if(veo_line_late && !apag_vela && !apun_vela){
    velo_moto_izqu = velo_esta;velo_moto_dere = velo_esta + valo_ajus_velo;  // configuro velocidad de motores
    atras();delay(500);          // retrocedo ya que la linea esta de frente
    if(cont_line_dere > cont_line_izqu){izquierda();}else{derecha();}
    delayy(150);      // escapo para el lado con al cual tenga menos sensores en alto 
  }
}


// ------------------------------------- busco velas ------------------------------------- 
void busco_vela(){
  // deteccion de vela
  // aumentamos la velocidad de el robot, ya que cuando esta apuntando la reduce - cuidado con que no vea ni la linea o la vela
  if(!apag_vela && !apun_vela){
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;
  }
  // me desplazo hacia adelante en caso de que no este pasando nada
  if(!apag_vela && !apun_vela && !veo_obst && !veo_line_late){
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;
    adelante();
  }
  
  // pregunto si estoy apuntando a vela, para en cazo de ver mas de una vela a la vez no quedar oscilando
  
  // si pierdo de vista la vela durante mas de 500 ms la doy por perdida
  if (ulti_vist_vela + 500 < millis()){
    apun_vela = false;          // suponemos que no estamos girando hacia la vela
    apag_vela = false;          // no hay vela en frente
  }
  
  maxi_valo_vela = 0; // reinicio el maximo valor de vela detectado
  cont_vela = 0;      // reinicio el contador de velas 
  
  // si he visto vela y he girado a la derecha sigo girando a la derecha
  if(apun_vela && dire_busq_vela){
    goto giro_derecha;
  }
  // analizo sensores de la izquierda
  for(int i=0;i<10;i++){
    if(senvela[i] > ruid_vela){
      cont_vela++;
      aux_velo_giro = (velo_esta-((velo_esta/10)*i)); // lo uso para girar despacion
      // si el valor de vela detectado es el mayor de todos giro en esa direccion
      if (senvela[i] > maxi_valo_vela){
        maxi_valo_vela = senvela[i];     // guardo el maximo valor
        dire_busq_vela = false;          //izquierda();
        // ajusto la velocidad
        velo_moto_izqu = aux_velo_giro;
        velo_moto_dere = aux_velo_giro + valo_ajus_velo;
        ulti_vist_vela = millis();  // actualizo la ultima vista de la vela
      }
    } // fin si veo vela
    
  }
  
  // si he visto vela y he girado a la izquierda sigo girando a la izquierda
  if (apun_vela && !dire_busq_vela){
    goto giro_izquierda;
  }
  giro_derecha:          // salto giro a la izquierda si estaba girando a la derecha
  
  // analizo sensores de la derecha
  for(int i=19;i>10;i--){
    if(senvela[i]>ruid_vela){  // revisar error de velocidad
      cont_vela++;
      aux_velo_giro = (velo_esta-((velo_esta/10)*(19-i))); // lo uso para girar despacion
      // si el valor de vela detectado es el mayor de todos giro en esa direccion
      if (senvela[i] > maxi_valo_vela){
        maxi_valo_vela = senvela[i];     // guardo el maximo valor
        dire_busq_vela = true;//derecha();
        // ajusto la velocidad
        velo_moto_izqu = aux_velo_giro;
        velo_moto_dere = aux_velo_giro + valo_ajus_velo;
        ulti_vist_vela = millis();  // actualizo la ultima vista de la vela
      } 
    }// fin si veo vela 
  } 
  giro_izquierda:      // salto  giro a la derecha si estaba girando a la izquierda
  
  if (cont_vela > 0){
    apun_vela = true;
    // configuro la posicion
    if(dire_busq_vela){derecha();}else{izquierda();} 
  } // si veo la vela con un sensor pongo apuntar vela en true

  //}
    //dist_mini_fron = 30;dist_mini_late = 30; ruid_vela=10;} 
  
  // ----------- porbar ajuste dinamico de sensivilidad de la vela -------------
  // disminullo la sensibilidad cuando encuentro velas
//  if (cont_vela > 0){
//    if(maxi_valo_vela - defe_ruid_vela > ruid_vela){   // osea si obtengo un valor de ruido mayor - menos sensibilidad ante el ruido
//      ruid_vela = maxi_valo_vela - defe_ruid_vela;     // uso el nuevo ruido
//    }
//  }
//  else{
//    ruid_vela = defe_ruid_vela;
//  }   

  //if(senvela[10] > ruid_vela){buz(1);}else{buz(0);}
} 

// --------------------------------- me acerco a la vela --------------------------------- 
void acercar_vela(){
  // si la vela enta en frente me acerco a ella
  if(senvela[9]>ruid_vela || senvela[10]>ruid_vela || senvela[11]>ruid_vela){apag_vela = true;apun_vela = false;}
   
  // si la vela esta en todo el frente
  if(senvela[10] > ruid_vela){
    // ajusto la velocidades al maximo
    velo_moto_izqu = velo_esta;
    velo_moto_dere = velo_esta + valo_ajus_velo;  
    // me acerco al robot
    adelante();                          
  } 
}


// --------------------- funcion que revisa si el robot se ha boqueada -------------------
void evito_bloqueo(){
  // --------- evito que el robot se atranque ---------> minimo cada 60ms (tiempo entre medidas ultrasonidos) 
  if((aux_tiem_atra + 100) < millis()){
    if(medi_dist_ultr_dere == valo_pasa_1 && medi_dist_ultr_fron == valo_pasa_2 && medi_dist_ultr_izqu == valo_pasa_3){
      // configuro las velocidades
      velo_moto_izqu = velo_esta;
      velo_moto_dere = velo_esta + valo_ajus_velo;
      atras();buz(1);
      delayy(500);
      buz(0);
      if(dire_busq_vela){derecha();}else{izquierda();}delayy(200);  // tomo decicion de nuevo rumbo en la direccion contraria al giro que el robot hizo hacia la vela
      adelante();
      delayy(400);
    }
    valo_pasa_1 = medi_dist_ultr_dere;
    valo_pasa_2 = medi_dist_ultr_fron;
    valo_pasa_3 = medi_dist_ultr_izqu;
    
    aux_tiem_atra = millis(); // actualizo el contador
  }
}
