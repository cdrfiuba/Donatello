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
#define PIN_SENSORS_MASKB       0x01
#define PIN_SENSORS_MASKD       0xf0
#define PIN_SENSORS_SHIFT       4


uint8_t Read_sensor(){
	return ((PINB & PIN_SENSORS_MASKB) << PIN_SENSORS_SHIFT) | ((PIND & PIN_SENSORS_MASKD) >> PIN_SENSORS_SHIFT);
};
// Casos que no tenemos contemplados, mejor ignorar
#define UNDEFINED   125

#define BOTON_ENCENDIDO 2

#define MOTOR_IZQUIERDO 9
#define MOTOR_DERECHO 10



float sensors_to_error[] = {
    UNDEFINED,  // -----
    +2,         // ----*
	+1,			// ---*-
	+1.5,		// ---**
	0,			// --*--
	UNDEFINED,	// --*-*
	+0.5,		// --**-
	UNDEFINED,	// --***
	-1,			// -*---
	UNDEFINED,	// -*--*
	UNDEFINED,	// -*-*-
	UNDEFINED,	// -*-**
	-0.5,		// -**--
	UNDEFINED,	// -**-*
	UNDEFINED,	// -***-
	UNDEFINED,	// -****
	-2,			// *----
	UNDEFINED,	// *---*
	UNDEFINED,	// *--*-
	UNDEFINED,	// *--**
	UNDEFINED,	// *-*--
	UNDEFINED,	// *-*-*
	UNDEFINED,	// *-**-
	UNDEFINED,	// *-***
	-1.5,		// **---

};

constexpr uint8_t kVelMaxIzq = 150;
constexpr uint8_t kVelMaxDer = 150;
constexpr float kP = 70; //it was 28
constexpr float kD = 70;

#define VEL_ADELANTE	200

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

/*float Read_Sensors(){
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
	else 
		return UNDEFINED;

}
*/
void follow_line_p() {
    float error = sensors_to_error[Read_sensor()];	
    //float error = Read_Sensors();
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
	DDRB = 0x01;
	DDRC = 0xf0;
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
	if(s.error_p==0 && s.error_d==0){
		s.velDer=VEL_ADELANTE;
		s.velIzq=VEL_ADELANTE;
	}
	else{
	s.velDer = min(max(s.velDer + s.error_d * kD + s.error_p * kP, 0), 220);
    s.velIzq = min(max(s.velIzq - s.error_d * kD - s.error_p * kP, 0), 220);

	}
	
	analogWrite(MOTOR_DERECHO, s.velDer);
	analogWrite(MOTOR_IZQUIERDO, s.velIzq);

}