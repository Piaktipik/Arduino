#include <Wire.h>

#include <L3G4200D.h>
#include <LSM303.h>

L3G4200D gyro;
LSM303 compass;

void setup() {
  Serial.begin(57600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  gyro.enableDefault();
  iniciar_motores();
}

void loop() {
  compass.read();
  gyro.read();
  Serial.print(" | A ");
  Serial.print("X: ");
  Serial.print((int)compass.a.x);if(compass.a.x<100 && compass.a.x >-10)Serial.print("\t");
  Serial.print(" Y: ");
  Serial.print((int)compass.a.y);if(compass.a.y<100 && compass.a.y >-10)Serial.print("\t");
  Serial.print(" Z: ");
  Serial.print((int)compass.a.z);if(compass.a.z<100 && compass.a.z >-10)Serial.print("\t");

  Serial.print(" | M ");  
  Serial.print("X: ");
  Serial.print((int)compass.m.x);if(compass.m.x<100 && compass.m.x >-10)Serial.print("\t");
  Serial.print(" Y: ");
  Serial.print((int)compass.m.y);if(compass.m.y<100 && compass.m.y >-10)Serial.print("\t");
  Serial.print(" Z: ");
  Serial.print((int)compass.m.z);if(compass.m.z<100 && compass.m.z >-10)Serial.print("\t");
  
  Serial.print(" | G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x);if(gyro.g.x<100 && gyro.g.x >-10)Serial.print("\t");
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);if(gyro.g.y<100 && gyro.g.y >-10)Serial.print("\t");
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);

  delay(20);
  
  if(Serial.available() > 0){
    datoserial(Serial.read());// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
  if(gyro.g.y > 200){
    atras();
  }
  else if(gyro.g.y < -200){
    adelante();
    
  }
  else{parar();}
}
