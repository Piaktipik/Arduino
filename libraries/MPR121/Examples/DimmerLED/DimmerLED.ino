/*
 TouchWheel.pde
 MPR121 WhellPad Example Code
 
 by:Waiman Zhao
 Mail:Binpower@foxmail.com
 created on: 11/2/14
 license: CC-SA 3.0
 
 Hardware: 3.3V Arduino Pro Mini
           SDA -> A4
           SCL -> A5
           IRQ -> D2
*/
#include <Wire.h>
#include <mpr121.h>


int LED = 13;

int VCCpin=12;
int Rpin=11;
int Gpin=10;
int Bpin=9;
int Modepin=5;

volatile int state = LOW;
int key = 0;
int lastkey = 0;

int rgbIndex=0;
uint8_t pinlevels[3]={255,255,255};
uint8_t R,G,B;

uint8_t X=0;
int gap=0;


// =========  setup  =========
void setup()
{ 
	//  initialize function
  Serial.begin(19200);
  Wire.begin();
  CapaTouch.begin();
  
  // initialize RGB LED IO
  pinMode(LED, OUTPUT);
  pinMode(Rpin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(Bpin, OUTPUT);
  pinMode(VCCpin, OUTPUT);
 
   // RGB LED PWM output
  analogWrite(Rpin,255);
  analogWrite(Gpin,255);
  analogWrite(Bpin,255);
  digitalWrite(VCCpin, HIGH); 
  
  pinMode(4, OUTPUT);	// GND
  pinMode(Modepin, INPUT);	// mode choose
  digitalWrite(4,LOW);
  digitalWrite(Modepin,HIGH);
  
  // Use interrupt read MPR121, reducing CPU load
  attachInterrupt(0, blink, FALLING);
  delay(500);
  Serial.println("START"); 
}

// =========  loop  =========
void loop()
{
  int mode = digitalRead(Modepin);
  digitalWrite(LED, state);  
  if(state==HIGH){
     if(mode==HIGH)  wheelFun();
     else keyFun();
    state = LOW;
  }
}

// --------- Interrupt handling 
void blink()
{
  state =HIGH;
}

// ---------  wheel pad handler
void wheelFun(){
    key=CapaTouch.wheelKey();
    Serial.print("wheelKey:\t");
    Serial.println(key,DEC); 
    if(key>0){
      if(key==20){
            if(rgbIndex==2) rgbIndex=0;
	    else rgbIndex++;
	    Serial.print("Toning: \t");
            if(rgbIndex==0) Serial.println("*R*");
            if(rgbIndex==1) Serial.println("*G*");
            if(rgbIndex==2) Serial.println("*B*");
      }else{
			gap=lastkey-key;
			if(gap!=0){	
				
				// 边缘处理
				if(lastkey<3 && key>14){	//增加
					gap = 16-key+lastkey;
					channgeRGB(gap);
				}else if(lastkey>14 && key<3){	//减少		
					gap = (16-lastkey+key)*-1;
					channgeRGB(gap);
				}else{
				// 正常处理
					if(gap<0){	//减少
						channgeRGB(gap);
					}else if(gap>0){	//增加
						channgeRGB(gap);
					}
					// lastkey==i 不处理
				}
			}
			lastkey=key;
      } //key
    }// -1
}

// --------- key pad handler
void keyFun(){
  int key2 =CapaTouch.keyPad();
      switch(key2)
      {
		case 1:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,0);
	                analogWrite(Gpin,255);
	                analogWrite(Bpin,255);
		break;
		case 2:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,0);
	                analogWrite(Gpin,0);
	                analogWrite(Bpin,255);
		break;
		case 3:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,255);
	                analogWrite(Gpin,0);
	                analogWrite(Bpin,255);
		break;
		case 4:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,255);
	                analogWrite(Gpin,0);
	                analogWrite(Bpin,0);
		break;
		case 5:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,255);
	                analogWrite(Gpin,255);
	                analogWrite(Bpin,0);
		break;
		case 6:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,0);
	                analogWrite(Gpin,255);
	                analogWrite(Bpin,0);
		break;
		case 7:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,255);
	                analogWrite(Gpin,100);
	                analogWrite(Bpin,100);

		break;
		case 8:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,100);
	                analogWrite(Gpin,255);
	                analogWrite(Bpin,100);
		break;
		case 9:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,100);
	                analogWrite(Gpin,100);
	                analogWrite(Bpin,255);
		break;
		case 0:
			Serial.println(key2,DEC);
	                analogWrite(Rpin,100);
	                analogWrite(Gpin,100);
	                analogWrite(Bpin,100);
		break;
		case 11:
			Serial.println("*");
	                analogWrite(Rpin,0);
	                analogWrite(Gpin,0);
	                analogWrite(Bpin,0);
		break;
		case 12:
			Serial.println("#");
	                analogWrite(Rpin,255);
	                analogWrite(Gpin,255);
	                analogWrite(Bpin,255);
		break;
		default:
		break;
      }
}


// --------- RGB PWM output process
void channgeRGB(int8_t increment){
  if(abs(increment)<4){
    //increment = increment*-1;
    Serial.print("D:");
    Serial.print(increment,DEC); 
    Serial.print("\t");


    uint8_t tmp;
    if(increment>0){
      tmp = 10*increment;
      if((pinlevels[rgbIndex]+tmp)<256)	pinlevels[rgbIndex]+=tmp;
    }else{
      tmp = -10*increment;
      if((pinlevels[rgbIndex]-tmp)>=0)	pinlevels[rgbIndex]-=tmp;
    }

    if(rgbIndex==0){	
      analogWrite(Rpin, pinlevels[rgbIndex]);
      Serial.print ("*R*");
    };
    if(rgbIndex==1){
      analogWrite(Gpin, pinlevels[rgbIndex]);
      Serial.print ("*G*");
    }
    if(rgbIndex==2){
      Serial.print ("*B*");
      analogWrite(Bpin, pinlevels[rgbIndex]);
    }

    Serial.print("\tR:");
    Serial.print(pinlevels[0],DEC); 
    Serial.print("\tG:");
    Serial.print(pinlevels[1],DEC);
    Serial.print("\tB:");
    Serial.println(pinlevels[2],DEC);
  }
}
//WHILE  R:125	G:5	B:5
