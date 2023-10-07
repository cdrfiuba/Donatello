//#include <Arduino.h>

// Seguidor de linea blanca con sensor TCRT5000
 
//const int AIA = A0;
const int motor_derecho = 9;  
//const int BIA = A2;  
const int motor_izquierdo = 10; 

const int infr_izq_2 = 4;
const int infr_izq_1 = 5;
const int infr_centro =6;
const int infr_der_1 = 7;
const int infr_der_2 = 8;

const int pulsador = 2;
const int pin_led = A5;

const int blanco = HIGH;
const int negro = LOW;

//0.1 y 255 -- 54seg

float multiplicador_der = 1; // multiplica la velocidad de la rueda derecha
float multiplicador_izq = 1; // multiplica la velocidad de la rueda iquierda
float paso_multiplicador = 0.02; // paso con el que se van variando los multiplicadores en cada iteración
float multiplicador_extremos = 5;
/*
1    1
1    1
0.01 0.02
4    5
54s  52s
    baterias completamente cargada
*/

int velocidad_max = 255; //puede variar entre 0 y 255

void centrar(){ //Reestablece los multiplicadores para que avance recto
  multiplicador_der = 1;
  multiplicador_izq = 1;
}

void doblar_iquierda(float val){ //recibe un valor entre 0 y 1 que modifica el multiplicador adecuado

  if(multiplicador_der + val < 1){
    multiplicador_der = multiplicador_der + val;
  }

  else if(multiplicador_der + val > 1 && multiplicador_der != 1){
    multiplicador_der = 1;
  }
  
  else if(multiplicador_der == 1 && multiplicador_izq - val > 0){
    multiplicador_izq = multiplicador_izq - val;
  }

  else if(multiplicador_der == 1 && multiplicador_izq - val < 0){
    multiplicador_izq = 0;
  }
}

void doblar_derecha(float val){ //recibe un valor entre 0 y 1 que modifica el multiplicador adecuado

  if(multiplicador_izq + val < 1){
    multiplicador_izq = multiplicador_izq + val;
  }

  else if(multiplicador_izq + val > 1 && multiplicador_izq != 1){
    multiplicador_izq = 1;
  }
  
  else if(multiplicador_izq == 1 && multiplicador_der - val > 0){
    multiplicador_der = multiplicador_der - val;
  }

  else if(multiplicador_izq == 1 && multiplicador_der - val < 0){
    multiplicador_der = 0;
  }
}

void setup() {
  pinMode(motor_derecho, OUTPUT);
  pinMode(motor_izquierdo, OUTPUT);
//  pinMode(BIA, OUTPUT);
//  pinMode(BIB, OUTPUT);
  
  pinMode(infr_izq_2, INPUT);
  pinMode(infr_izq_1, INPUT);
  pinMode(infr_centro, INPUT);
  pinMode(infr_der_1, INPUT);
  pinMode(infr_der_2, INPUT);

  pinMode(pulsador, INPUT);
  pinMode(pin_led, OUTPUT);

  while(!digitalRead(pulsador)); //Espera a que se accione el pulsador para comenzar a correr el código
  digitalWrite(pin_led, HIGH);
}
 
void loop() {

  if(digitalRead(infr_izq_2) == blanco) doblar_iquierda(multiplicador_extremos * paso_multiplicador);
  if(digitalRead(infr_izq_1) == blanco) doblar_iquierda(paso_multiplicador);
  if(digitalRead(infr_centro) == blanco) centrar();
  if(digitalRead(infr_der_1) == blanco) doblar_derecha(paso_multiplicador);
  if(digitalRead(infr_der_2) == blanco) doblar_derecha(multiplicador_extremos * paso_multiplicador);
  
  //analogWrite(AIA, 0);
  analogWrite(motor_derecho, velocidad_max * multiplicador_der);
  ///analogWrite(BIA, 0);
  analogWrite(motor_izquierdo, velocidad_max * multiplicador_izq);

  //delay(50);
}
