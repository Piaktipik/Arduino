// #
// # Editor    : Jiang from DFRobot
// # Data      : 19.11.2012
// # E-Mail    : jianghao0716@gmail.com

// # Product name  : Capacitive touch pad 
// # Version  : 0.4
// # Power supply: 3.3v
// # Connection:
// #       SDA (MPR121) -> PIN A4 (Arduino)
// #       SCL (MPR121) -> PIN A5 (Arduino)
// #       IRQ (MPR121) -> PIN A2 (Arduino) 

#include <Wire.h>
#include <mpr121.h>

int X ;           // X-coordinate
int Y ;           // Y-coordinate

// =========  setup  =========
void setup()
{ 
  //  initialize function
  Serial.begin(19200);
  Wire.begin();
  CapaTouch.begin();

  delay(500);
  Serial.println("START"); 
}


// =========  loop  =========
void loop()
{   
  X=CapaTouch.getX();              // Get X position.
  Y=CapaTouch.getY();              // Get Y position.
  if(X>=1&&X<=9&&Y>=1&&Y<=13)
   {                                 // Determine whether in the range.If not,do nothing.
      Serial.print("X=");
      Serial.print(X);
      Serial.print("  Y=");
      Serial.println(Y); 
   }
  delay(200);
}








