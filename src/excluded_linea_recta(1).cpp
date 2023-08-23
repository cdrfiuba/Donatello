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

int velocidad_max_der = 230; //puede variar entre 0 y 255
int velocidad_max_izq = 170;

typedef enum {
    WAIT_BUTTON,
    PROGRAM
} state_t;

struct State {
  bool button_pressed = false;
  state_t program_state = WAIT_BUTTON;
  long int dt = 0;
};
State state;


typedef void (procesar_state_t)();

void centrar(){
    analogWrite(AIA, 0);
    analogWrite(AIB, velocidad_max_der);
    analogWrite(BIA, 0);
    analogWrite(BIB, velocidad_max_izq);
}

void doblar_derecha_1(){
    analogWrite(AIA, 0);
    analogWrite(AIB, velocidad_max_der);
    analogWrite(BIA, 0);
    analogWrite(BIB, (multiplicador_izq + paso_multiplicador) *  velocidad_max_izq);
}

void doblar_derecha_2(){
    analogWrite(AIA, 0);
    analogWrite(AIB, velocidad_max_der);
    analogWrite(BIA, 0);
    analogWrite(BIB, (multiplicador_izq + 2*paso_multiplicador) *  velocidad_max_izq);
}

void doblar_izquierda_1(){
    analogWrite(AIA, 0);
    analogWrite(AIB, (multiplicador_der + paso_multiplicador) * velocidad_max_der);
    analogWrite(BIA, 0);
    analogWrite(BIB, velocidad_max_izq);
}

void doblar_izquierda_2(){
    analogWrite(AIA, 0);
    analogWrite(AIB, (multiplicador_der + 2*paso_multiplicador) * velocidad_max_der);
    analogWrite(BIA, 0);
    analogWrite(BIB, velocidad_max_izq);
}

void parar(){
    analogWrite(AIA, 0);
    analogWrite(AIB, 0);
    analogWrite(BIA, 0);
    analogWrite(BIB, 0);
}

void wait_button(){
    if(state.button_pressed){
        state.program_state = PROGRAM;
    }
}

void program(){
    if((digitalRead(infr_centro) == blanco) && (digitalRead(infr_izq_1) == blanco) && (digitalRead(infr_der_1) == blanco)){
        centrar();
    }

    else if((digitalRead(infr_izq_1) == negro) && (digitalRead(infr_centro) == blanco) && (digitalRead(infr_der_1) == blanco)){
        doblar_derecha_1();
    }

    else if((digitalRead(infr_izq_1) == negro) && (digitalRead(infr_centro) == negro) && (digitalRead(infr_der_1) == blanco)){
        doblar_derecha_2();
    }

    else if((digitalRead(infr_izq_1) == blanco) && (digitalRead(infr_centro) == blanco) && (digitalRead(infr_der_1) == negro)){
        doblar_izquierda_1();
    }

    else if((digitalRead(infr_izq_1) == blanco) && (digitalRead(infr_centro) == negro) && (digitalRead(infr_der_1) == negro)){
        doblar_izquierda_2();
    }

    else{
        parar();
        state.program_state = WAIT_BUTTON;
    }
}

procesar_state_t* dispatch_table[] = {
    wait_button,
    program
};

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
}


void loop() {
    state.button_pressed = digitalRead(pulsador);

    dispatch_table[state.program_state]();

}