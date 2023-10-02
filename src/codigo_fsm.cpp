#include <Arduino.h>

#define ELAPSED() (micros() - s.t0)

// Puente H
#define AIA A0
#define AIB A1  
#define BIA A2  
#define BIB A3

// Infrarrojos
#define PIN_SENSOR_IZQUIERDOO   8
#define PIN_SENSOR_IZQUIERDO    7
#define PIN_SENSOR_CENTRO       6
#define PIN_SENSOR_DERECHO      5
#define PIN_SENSOR_DERECHOO     4
#define PIN_SENSORS_MASK        0b00001110
#define PIN_SENSORS_SHIFT       1

//#define READ_SENSORS()  ((PINB & PIN_SENSORS_MASK) >> PIN_SENSORS_SHIFT)

// Casos que no tenemos contemplados, mejor ignorar
#define UNDEFINED   125

#define BOTON_ENCENDIDO 2

constexpr float sensors_to_error[] = {
    UNDEFINED,  // ---
    +2,         // --*
     0,         // -*-
    +1,         // -**
    -2,         // *--
    UNDEFINED,  // *-*
    -1,         // **-
    UNDEFINED   // ***
};

constexpr uint8_t kVelMaxIzq = 155;
constexpr uint8_t kVelMaxDer = 155;
constexpr float kP = 130; //it was 28
constexpr float kD = 60;

struct ProgramData {
    int16_t last_error =  0;
    unsigned long t0;
    float dt;
    float error_p;
    float error_d;
    int16_t velDer = kVelMaxDer;
    int16_t velIzq = kVelMaxIzq;
};
ProgramData s{};

float Read_Sensors(){
    int centroo = digitalRead(PIN_SENSOR_CENTRO);
    int izquierdo = digitalRead(PIN_SENSOR_IZQUIERDO);
    int derecho = digitalRead(PIN_SENSOR_DERECHO);
    int derechoo = digitalRead(PIN_SENSOR_DERECHOO);
    int izquierdoo = digitalRead(PIN_SENSOR_IZQUIERDOO);

    if(centroo==1){
        return 0;
    }
    else if(centroo==1 && izquierdo==1){
        return -0.5;
    }
     else if(centroo==1 && derecho==1){
        return 0.5;
    }
    else if(derecho==1){
        return 1;
    }
    else if(izquierdo==1){
        return -1;
    }
    else if(izquierdoo==1 && izquierdo ==1){
        return -1.5;
    }
    else if(derecho==1 && derechoo ==1){
        return 1.5;
    }
    else if(izquierdoo==1){
        return -2;
    }else if(derechoo==1){
        return 2;
    }
    else{
        return 4;
    }
}

void follow_line_p() {
    float error = Read_Sensors();
    // Better safe than sorry
    if (error == UNDEFINED) {
        return;
    }
    s.error_p = error;
}

void follow_line_d() {
    s.error_d = (s.error_p - s.last_error) / s.dt;
    s.last_error = s.error_d;
}

void setup() {
    // Puente H

   // Serial.begin(9600);
    pinMode(PIN_SENSOR_IZQUIERDO, INPUT);
    pinMode(PIN_SENSOR_CENTRO, INPUT);
    pinMode(PIN_SENSOR_DERECHO, INPUT);
    pinMode(PIN_SENSOR_DERECHOO, INPUT);
    pinMode(PIN_SENSOR_IZQUIERDOO, INPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    pinMode(BOTON_ENCENDIDO, INPUT);
    while (!digitalRead(BOTON_ENCENDIDO));

    s.t0 = millis();
}
 
void loop() {
    follow_line_p();
    if (100 < (s.dt = millis() - s.t0)) {
        s.t0 = millis(); 
        follow_line_d();
    }

    s.velDer = min(max(kVelMaxDer  + s.error_d * kD + s.error_p * kP, 0), 230);
    s.velIzq = min(max(kVelMaxIzq  - s.error_d * kD - s.error_p * kP, 0), 230);

    //s.velDer = min(s.velDer, 235);
    //s.velIzq = min(s.velIzq, 235);
/*
    if(s.error_p > 0){
        s.velDer = max(kVelMaxDer  + s.error_d * kD + s.error_p * kP, 0);
        s.velIzq = max(kVelMaxIzq  - s.error_d * kD - s.error_p * kP, 0);
    } else if (s.error_p < 0) {
        s.velDer = max(kVelMaxDer  + s.error_d * kD + s.error_p * kP, 0);
        s.velIzq = max(kVelMaxIzq  - s.error_d * kD - s.error_p * kP, 0);
    }else{
        s.velDer = kVelMaxDer;
        s.velIzq = kVelMaxIzq;
    }
*/
	analogWrite(10, s.velDer);
	analogWrite(9, s.velIzq);	

}