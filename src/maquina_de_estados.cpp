#include <Arduino.h>

#define PIN_BTN    2

typedef enum {
  STATE_INIT_A,
  STATE_A,
  STATE_ELAPSED_A,
  STATE_INIT_B,
  STATE_B,
  STATE_ELAPSED_B
} state_t;

const int sensorPin = 9;
const int AIA = 10; //pines motores
const int AIB =11;

byte speed=255;

void setup() {
  Serial.begin(9600);   //iniciar puerto serie
  pinMode(PIN_BTN, INPUT);  //definir pin como entrada
  pinMode(sensorPin, INPUT);  //definir pin como entrada
  pinMode(AIA, OUTPUT);  //definir pin como salida
  pinMode(AIB, OUTPUT);  //definir pin como salida
}

void backward() {
 analogWrite(AIA, 0);
 analogWrite(AIB, speed);
}

void forward() {
 analogWrite(AIA, speed);
 analogWrite(AIB, 0);
}

void stop() {
  analogWrite(AIA, 0);
  analogWrite(AIB, 0);
}

struct State {
  bool button_pressed;
  state_t program_state;
  long int dt;
};
State state;

typedef void (procesar_state_t)();


void stateInitA() {
  forward();
  state.dt = 0;
  state.program_state = STATE_A;
}

void stateA() {
  long int start_time = micros();

  if(state.dt > 2000000) {
    stop();
    state.program_state = STATE_ELAPSED_A;
  }

  state.dt += micros() - start_time;
}

void elapsedA() {
  if(state.button_pressed){
    state.program_state = STATE_INIT_B;
  }
}

void stateInitB() {
  backward();
  state.dt = 0;
  state.program_state = STATE_B;
}

void stateB() {
  if(state.dt > 2000000) {
    stop();
    state.program_state = STATE_ELAPSED_B;
  }

}

void elapsedB(){
  if(state.button_pressed){
    state.program_state = STATE_INIT_A;
  }
}

procesar_state_t* dispatch_table[] = {
  stateInitA,
  stateA,
  elapsedA,
  stateInitB,
  stateB,
  elapsedB
};


void loop(){
  long int start_time = micros();

  state.button_pressed = digitalRead(PIN_BTN);

  dispatch_table[state.program_state]();

  state.dt += micros() - start_time;
}


