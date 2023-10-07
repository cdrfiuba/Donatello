#include <Arduino.h>

#define PIN_BTN 2
#define PIN_MOTOR_LEFT 10
#define PIN_MOTOR_RIGHT 9
#define PIN_LED A5

#define PIN_SENSOR_IZQUIERDOO 4
#define PIN_SENSOR_IZQUIERDO 5
#define PIN_SENSOR_CENTRO 6
#define PIN_SENSOR_DERECHO 7
#define PIN_SENSOR_DERECHOO 8
#define PIN_SENSORS_SHIFT 1

#define DER 0
#define IZQ 1

typedef enum {
  WAITING_BUTTON,
  STATE_CENTER,
  STATE_LEFT,
  STATE_RIGHT,
  STATE_LEFTT,
  STATE_RIGHTT,
  STATE_ZERO
} state_t;


struct State {
  bool button_pressed;
  state_t program_state;
  state_t previous_state;
  long int dt;
};
State state;

byte speed = 230;
byte speed2 = speed * 0.05;

typedef enum {
  ZERO,
  DELTA_PLUS_PLUS,
  DELTA_PLUS,
  DELTA,
  DELTA_MINUS,
  DELTA_MINUS_MINUS
} delta_t;

signed int deltas[6][2] = {
  { 0, 0 },                  // ZERO - avanza derecho
  { -50, speed },  // DELTA_PLUS_PLUS - giro fuerte a la derecha
  { 0, speed },             // DELTA_PLUS - giro suave a la derecha
  { 0, speed },             // DELTA - giro mas suave a la derecha
  { 0, speed },             // DELTA_MINUS - corrijo inercia a la derecha
  { 0, speed }              // DELTA_MINUS_MINUS - corrijo inercia mas suave a la derecha
};

void setup() {
  //Serial.begin(9600);       //iniciar puerto serie
  pinMode(PIN_BTN, INPUT);  //definir pin como entrada
  pinMode(PIN_MOTOR_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT, OUTPUT);
  pinMode(PIN_SENSOR_IZQUIERDO, INPUT);
  pinMode(PIN_SENSOR_CENTRO, INPUT);
  pinMode(PIN_SENSOR_DERECHO, INPUT);
  pinMode(PIN_SENSOR_DERECHOO, INPUT);
  pinMode(PIN_SENSOR_IZQUIERDOO, INPUT);
  pinMode(PIN_LED, OUTPUT);

  while (!digitalRead(PIN_BTN))
    ;
  state.previous_state = state.program_state = STATE_CENTER;
  digitalWrite(PIN_LED, HIGH);
  delay(100);

  /*for(int i=0; i<100; i++){
    delay(1000);
    analogWrite(PIN_MOTOR_RIGHT,255);
    analogWrite(PIN_MOTOR_LEFT,253);
    delay(2000);
    analogWrite(PIN_MOTOR_RIGHT,0);
    analogWrite(PIN_MOTOR_LEFT,0);
    while (!digitalRead(PIN_BTN));


    }
    while(1);*/
}


void forward(delta_t delta, bool dir) {
  //analogWrite(PIN_MOTOR_RIGHT, constrain( speed - deltas[delta][!dir] ,0,255));
  //analogWrite(PIN_MOTOR_LEFT, constrain( speed - deltas[delta][dir]-2 ,0,255));
}


typedef void(procesar_state_t)();

//lectura de sensores
void read_sensors() {
  int sensores[] = {
    digitalRead(PIN_SENSOR_IZQUIERDOO),
    digitalRead(PIN_SENSOR_IZQUIERDO),
    digitalRead(PIN_SENSOR_CENTRO),
    digitalRead(PIN_SENSOR_DERECHO),
    digitalRead(PIN_SENSOR_DERECHOO)
  };


  if (sensores[2]) {
    state.previous_state = state.program_state;
    state.program_state = STATE_CENTER;
  } else if (sensores[1]) {
    state.previous_state = state.program_state;
    state.program_state = STATE_LEFT;
  } else if (sensores[3]) {
    state.previous_state = state.program_state;
    state.program_state = STATE_RIGHT;
  } else if (sensores[4]) {
    state.previous_state = state.program_state;
    state.program_state = STATE_RIGHTT;
  } else if (sensores[0]) {
    state.previous_state = state.program_state;
    state.program_state = STATE_LEFTT;
  }
}

//no usado
void waiting_button() {
  if (!digitalRead(PIN_BTN)) {
    state.program_state = state.previous_state = STATE_CENTER;
  }
}

void state_center() {
  analogWrite(PIN_MOTOR_RIGHT, speed);
  analogWrite(PIN_MOTOR_LEFT, speed);
}

void state_right() {
  analogWrite(PIN_MOTOR_RIGHT, speed2);
  analogWrite(PIN_MOTOR_LEFT, speed);
}

void state_left() {
  analogWrite(PIN_MOTOR_RIGHT, speed);
  analogWrite(PIN_MOTOR_LEFT, speed2);
}

void state_zero() {
}

void state_rightt() {
  while (!digitalRead(PIN_SENSOR_CENTRO)) {
    analogWrite(PIN_MOTOR_RIGHT, 0);
    analogWrite(PIN_MOTOR_LEFT, speed);
  }
}

void state_leftt() {
  while (!digitalRead(PIN_SENSOR_CENTRO)) {
    analogWrite(PIN_MOTOR_RIGHT, speed);
    analogWrite(PIN_MOTOR_LEFT, 0);
  }
}

/*
  WAITING_BUTTON,
  STATE_CENTER,
  STATE_LEFT,
  STATE_RIGHT,
  STATE_LEFTT,
  STATE_RIGHTT,
  STATE_ZERO
*/
procesar_state_t* dispatch_table[] = {
  waiting_button,
  state_center,
  state_left,
  state_right,
  state_leftt,
  state_rightt,
  state_zero
};


void loop() {

  read_sensors();
  dispatch_table[state.program_state]();
  //Serial.println(state.program_state);
  //delay(10);
}
