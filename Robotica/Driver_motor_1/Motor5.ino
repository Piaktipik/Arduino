
void abrir() {
  pinza.write(160);
  Serial.println("abrir");
  delay(10);
}
void cerrar() {
  pinza.write(20);
  Serial.println("cerrar");
  delay(10);
}

