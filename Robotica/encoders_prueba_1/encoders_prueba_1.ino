int pinL = 13;
int pinA = 2;
int pinB = 3;
int ca = 0;
int cb = 0;
int est_enc = 0;
long pas_tot = 0;

volatile int state = LOW;

void setup() {
    pinMode(pinL, OUTPUT);
    activarEncoder(pinA,pinB);
    Serial.begin(115200);
}

void loop() {
    digitalWrite(pinL, state);
    //delay(100);
    //int est_enc = obt_est(pinA,pinB);
    Serial.print("Ca = " + String(ca));
    Serial.print(" , Cb = " + String(cb));
    Serial.print(" , Est_enc = " + String(est_enc));
    Serial.println(" , pas_tot = " + String(pas_tot));
}

