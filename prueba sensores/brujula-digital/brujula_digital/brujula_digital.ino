// por: julian galvez

const int analogInPin = A0;  // pin analogo de un eje
const int analogInPin1 = A1;  // pin analogo de un eje

float valor1 = 0;        // valor promediado
float valor2 = 0;        // valor promediado

// vector para guardar el promedio de las mediciones
const int tval1 = 50;
const int tval2 = 50;

int val1[tval1];
int val2[tval2];

int c1 = 0;
int c2 = 0;

// variables para calculo del centro del circulo del indicador de la brujula
int xma=0, xmi=1024, yma=0, ymi=1024;
int x=0,y=0;
float al=0,pi=3.1416;

void setup() {
  pinMode(8,INPUT);
  // se inicializa el puerto serial
  Serial.begin(9600);
}

void loop() {
  // se leen los valores analogos:
  val1[c1] = analogRead(analogInPin);
  c1=(c1+1)%tval1;
  val2[c2] = analogRead(analogInPin1);
  c2=(c2+1)%tval2;
  
  valor1 = 0;
  for(int i=0;i<tval1;i++){
    valor1 = valor1 + val1[i];
  }
  valor1 = valor1/tval1;
  valor2 = 0;
  for(int i=0;i<tval2;i++){
    valor2 = valor2 + val2[i];
  }
  valor2 = valor2/tval2;
//  Serial.print("sensor = " );
//  Serial.print(valor1);
//  Serial.print("\t sensor1 = " );
//  Serial.print(valor2);
//  Serial.print("\t xma = " );
//  Serial.print(xma);
//  Serial.print("\t xmi = " );
//  Serial.print(xmi);
//  Serial.print("\t valor1-y = " );
//  Serial.print(valor1-y);
//  Serial.print("\t valor2-x = " );
//  Serial.print(valor2-x);
//  Serial.print("\t alfa = " );
if(c1%50 == 0) Serial.println(al);
  
  // fin filtro
  
  // tomo el valor maximo y minimo de cada eje para calcular automaticamente el circulo
  
  if (digitalRead(8) == HIGH){
    if (valor1 > yma) yma = valor1;
    if (valor1 < ymi) ymi = valor1;
    if (valor2 > xma) xma = valor2;
    if (valor2 < xmi) xmi = valor2;
    // calculo el centro del circulo
    y = int((yma-ymi)/2)+ymi;
    x = int((xma-xmi)/2)+xmi;
  } 
  
  al = atan((valor1-y)/(valor2-x));
  if(valor2-x < 0) al = al + pi;
  if(valor2-x > 0 && valor1-y < 0) al = al + 2*pi;
  
  //al = al*180/pi;
  
}
