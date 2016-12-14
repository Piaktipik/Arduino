
// 
byte sendCommand(byte command){
  if (command == SELECT_MIFARE_CARD){
    Serial.write(0xBA);
    Serial.write(0x02);
    Serial.write(command);
    Serial.write(0xB9);
  }
  delay(500);
  pinMode(13,OUTPUT);
  while(Serial.available()>0){
    Serial.read();
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500);
  }
  
}

// imprime el tipo de tarjeta encontrada
void tag_type(byte tag) {
  
  switch (tag) {
  case MIFARE_1K_4B:
    Serial.println("MIFARE_1K_4B");
    break;

  case MIFARE_1K_7B:
    Serial.println("MIFARE_1K_7B");
    break;

  case MIFARE_ULTRALIGHT_7B:
    Serial.println("MIFARE_ULTRALIGHT_7B");
    break;

  case MIFARE_4K_4B:
    Serial.println("MIFARE_4K_4B");
    break;

  case MIFARE_4K_7B:
    Serial.println("MIFARE_4K_7B");
    break;

  case MIFARE_DESFIRE_7B:
    Serial.println("MIFARE_DESFIRE_7B");
    break;
    
  case OTHER:
    Serial.println("OTHER");
    break;
    
  default:
    Serial.println("OTHER");
    break;
  }
  
}
// calulo de checksum -> XOR
byte CHECKSUM_XOR (byte *values, int length) {
  byte result = values[0];
  for (int i = 1; i < length; i++)
    result ^= values[i]; // operacion XOR
  return result;
}





// ---------------------- functions ---------------------- 
//void loginToSectorTest() {
//   
//  if (mifare.loginToSector(2, keyA, USE_KEY_A) == LOGIN_SUCCEED)
//    Serial.println("Login OK");
//  else
//    Serial.println("Login Failed");
//
//  delay(500);  
//}


//void readDatablockTest() {
//  int i;
//  datablock *data;
//
//  if (mifare.loginToSector(2, keyA, USE_KEY_A) == LOGIN_SUCCEED) {
//    data = mifare.readDatablock(8);
//    
//    if (data->status == 0x00) {
//      for (i = 0; i < 16; i++)
//        Serial.println(data->data[i], HEX);
//        
//      delay(5000);      
//    }    
//  }
//  
//  delay(500);
//}
