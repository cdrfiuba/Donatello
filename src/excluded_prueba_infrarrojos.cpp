

const int infr_izq_2 = 8;
const int infr_izq_1 = 9;
const int infr_centro = 10;
const int infr_der_1 = 11;
const int infr_der_2 = 12;

const int blanco = HIGH;
const int negro = LOW;


void setup() {

  
  pinMode(infr_izq_2, INPUT);
  pinMode(infr_izq_1, INPUT);
  pinMode(infr_centro, INPUT);
  pinMode(infr_der_1, INPUT);
  pinMode(infr_der_2, INPUT);

  Serial.begin(9600);
}
 
void loop() {

  if(digitalRead(infr_izq_2) == blanco) Serial.println("IZQUIERDA 2");
  if(digitalRead(infr_izq_1) == blanco) Serial.println("IZQUIERDA 1");
  if(digitalRead(infr_centro) == blanco) Serial.println("CENTRO");
  if(digitalRead(infr_der_1) == blanco) Serial.println("DERECHA 1");
  if(digitalRead(infr_der_2) == blanco) Serial.println("DERECHA 2");

}
