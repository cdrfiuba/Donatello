#include <Arduino.h>

// Seguidor de linea blanca con sensor TCRT5000
 
const int AIA = A0;
const int AIB = A1;  
const int BIA = A2;  
const int BIB = A3; 

const int infr_izq_2 = 8;
const int infr_izq_1 = 9;
const int infr_centro = 10;
const int infr_der_1 = 11;
const int infr_der_2 = 12;

const int pulsador = 7;

const int blanco = HIGH;
const int negro = LOW;

float multiplicador_der = 1; // multiplica la velocidad de la rueda derecha
float multiplicador_izq = 1; // multiplica la velocidad de la rueda iquierda
float paso_multiplicador = 0.05; // paso con el que se van variando los multiplicadores en cada iteración

int velocidad_max = 150; //puede variar entre 0 y 255


void setup() {
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  
  pinMode(infr_izq_2, INPUT);
  pinMode(infr_izq_1, INPUT);
  pinMode(infr_centro, INPUT);
  pinMode(infr_der_1, INPUT);
  pinMode(infr_der_2, INPUT);

  pinMode(pulsador, INPUT);

  while(digitalRead(pulsador)); //Espera a que se accione el pulsador para comenzar a correr el código
}
 
void loop() {

  if(digitalRead(infr_izq_2) == blanco) doblar_iquierda(2 * paso_multiplicador);
  if(digitalRead(infr_izq_1) == blanco) doblar_iquierda(paso_multiplicador);
  if(digitalRead(infr_centro) == blanco) centrar();
  if(digitalRead(infr_der_1) == blanco) doblar_derecha(paso_multiplicador);
  if(digitalRead(infr_der_2) == blanco) doblar_derecha(2 * paso_multiplicador);
  
  analogWrite(AIA, 0);
  analogWrite(AIB, velocidad_max * multiplicador_der);
  analogWrite(BIA, 0);
  analogWrite(BIB, velocidad_max * multiplicador_izq);

  //delay(50);
}



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