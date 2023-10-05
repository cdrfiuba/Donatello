#include <Arduino.h>

#define PIN_BTN    2
#define PIN_MOTOR_LEFT 9
#define PIN_MOTOR_RIGHT 10

#define PIN_SENSOR_IZQUIERDOO   8
#define PIN_SENSOR_IZQUIERDO    7
#define PIN_SENSOR_CENTRO       6
#define PIN_SENSOR_DERECHO      5
#define PIN_SENSOR_DERECHOO     4
#define PIN_SENSORS_SHIFT       1


typedef enum {
  WAITING_BUTTON,
  STATE_CENTER,
  STATE_CENTER_LEFT,
  STATE_LEFT,
  STATE_CENTER_RIGHT,
  STATE_RIGHT,
  STATE_ZERO,
  STATE_RIGHT_RIGHT,
  STATE_LEFT_LEFT,
  STATE_LEFTT,
  STATE_RIGHTT
} state_t;


struct State {
  bool button_pressed;
  state_t program_state;
  state_t previous_state;
  long int dt;
};
State state;

byte speed=150;

typedef enum{
  ZERO,
  DELTA_PLUS_PLUS,
  DELTA_PLUS,
  DELTA,
  DELTA_MINUS,
  DELTA_MINUS_MINUS
}delta_t;

signed char deltas[6][2] ={
  {0, 0}, // ZERO - avanza derecho
  {100, -120}, // DELTA_PLUS_PLUS - giro fuerte a la derecha
  {0, -50}, // DELTA_PLUS - giro suave a la derecha 
  {0 , -25}, // DELTA - giro mas suave a la derecha
  {25 , 0}, // DELTA_MINUS - corrijo inercia a la derecha
  {15 , 0} // DELTA_MINUS_MINUS - corrijo inercia mas suave a la derecha
}; 

void setup() {
  Serial.begin(9600);   //iniciar puerto serie
  pinMode(PIN_BTN, INPUT);  //definir pin como entrada
  pinMode(PIN_MOTOR_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT, OUTPUT);
  pinMode(PIN_SENSOR_IZQUIERDO, INPUT);
  pinMode(PIN_SENSOR_CENTRO, INPUT);
  pinMode(PIN_SENSOR_DERECHO, INPUT);
  pinMode(PIN_SENSOR_DERECHOO, INPUT);
  pinMode(PIN_SENSOR_IZQUIERDOO, INPUT);
  state.program_state = WAITING_BUTTON;
}


void forward(delta_t d, bool dir) {
 analogWrite(PIN_MOTOR_LEFT, speed+deltas[d][!dir]);
 analogWrite(PIN_MOTOR_RIGHT, speed+deltas[d][dir]);
}


typedef void (procesar_state_t)();

//lectura de sensores
void read_sensors(){
    int sensores[] = {
      digitalRead(PIN_SENSOR_IZQUIERDOO),
      digitalRead(PIN_SENSOR_IZQUIERDO),
      digitalRead(PIN_SENSOR_CENTRO),
      digitalRead(PIN_SENSOR_DERECHO),
      digitalRead(PIN_SENSOR_DERECHOO)
      };
    

    if(sensores[2]){
      state.previous_state = state.program_state;
      state.program_state = STATE_CENTER;
    }
    else if(sensores[2] && sensores[1]){
      state.previous_state = state.program_state;
      state.program_state = STATE_CENTER_LEFT;
    }
     else if(sensores[1]){
      state.previous_state = state.program_state;
      state.program_state = STATE_LEFT;
    }
    else if(sensores[2] && sensores[3]){
      state.previous_state = state.program_state;
      state.program_state = STATE_CENTER_RIGHT;
    }
     else if(sensores[3]){
      state.previous_state = state.program_state;
      state.program_state = STATE_RIGHT;
    }
    else if(sensores[4] && sensores[3]){
      state.previous_state = state.program_state;
      state.program_state = STATE_RIGHT_RIGHT;
    }
    else if(sensores[4]){
      state.previous_state = state.program_state;
      state.program_state = STATE_RIGHTT;
    }
    else if(sensores[0] && sensores[1]){
      state.previous_state = state.program_state;
      state.program_state = STATE_LEFT_LEFT;
    }
    else if(sensores[0]){
      state.previous_state = state.program_state;
      state.program_state = STATE_LEFTT; 
    }

}


void waiting_button() {
  if(digitalRead(PIN_BTN)){
    state.program_state = state.previous_state = STATE_CENTER;
  }
}

void state_center(){
  switch (state.previous_state)
  {
  case STATE_CENTER:
    analogWrite(PIN_MOTOR_RIGHT, 200);
	  analogWrite(PIN_MOTOR_LEFT, 200);	
    break;
  
  case STATE_CENTER_RIGHT:
    analogWrite(PIN_MOTOR_RIGHT, 230);
	  analogWrite(PIN_MOTOR_LEFT, 170);	
  break;

  case STATE_RIGHT:
    analogWrite(PIN_MOTOR_RIGHT, 170);
	  analogWrite(PIN_MOTOR_LEFT, 90);	
  break;

  case STATE_CENTER_LEFT:
    analogWrite(PIN_MOTOR_RIGHT, 170);
	  analogWrite(PIN_MOTOR_LEFT, 230);	
  break;

  case STATE_LEFT:
    analogWrite(PIN_MOTOR_RIGHT, 90);
	  analogWrite(PIN_MOTOR_LEFT, 170);	
  break;

  case STATE_LEFT_LEFT:
    analogWrite(PIN_MOTOR_RIGHT, 30);
	  analogWrite(PIN_MOTOR_LEFT, 150);	
  break;

  case STATE_LEFTT:
    analogWrite(PIN_MOTOR_RIGHT, 30);
	  analogWrite(PIN_MOTOR_LEFT, 170);	
  break;
  }

}

void state_center_left(){
  switch (state.previous_state)
  {
  case STATE_CENTER:
    forward(DELTA, 1);
  break;

  case STATE_CENTER_LEFT:
    forward(DELTA_MINUS_MINUS, 1); //
  break;

  case STATE_LEFT:
    forward(DELTA_MINUS, 0); //
  break;
  }
}



void state_center_right(){
  switch (state.previous_state)
  {
  case STATE_CENTER:
    forward(DELTA, 0);
  break;

  case STATE_CENTER_RIGHT:
    forward(DELTA_MINUS_MINUS, 0); //
  break;

  case STATE_RIGHT:
    forward(DELTA_MINUS, 1); //
  break;
  }
}

void state_right(){
  switch (state.previous_state)
  {
  case STATE_CENTER:
    forward(DELTA_PLUS_PLUS,0);
    break;
  
  case STATE_CENTER_RIGHT:
    forward(DELTA_PLUS, 0); //
  break;

  case STATE_RIGHT:
    forward(DELTA_MINUS, 0); //
  break;

  case STATE_ZERO:
  break; //
  }
}

void state_left(){
  switch (state.previous_state)
  {
  case STATE_CENTER:
    forward(DELTA_PLUS_PLUS,1);
    break;
  
  case STATE_CENTER_RIGHT:
    forward(DELTA_PLUS, 1); //
  break;

  case STATE_RIGHT:
    forward(DELTA_MINUS, 1); //
  break;

  case STATE_ZERO:
  break; //
  }
}

void state_zero(){
  switch (state.previous_state)
  {
  case STATE_RIGHT:
  case STATE_CENTER_RIGHT:
  forward(DELTA_PLUS_PLUS, 0);
  break;

  case STATE_LEFT:
  case STATE_CENTER_LEFT:
  forward(DELTA_PLUS_PLUS, 1);
  break;

  default:
    break;
  }
}


procesar_state_t* dispatch_table[] = {
  waiting_button,
  state_center,
  state_center_left,
  state_left,
  state_center_right,
  state_right,
  state_zero
};


void loop(){

  read_sensors();
  dispatch_table[state.program_state]();

  delay(10); 

}


