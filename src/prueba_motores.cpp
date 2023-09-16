#include <Arduino.h>

#define AIA 3
#define AIB 5
#define BIA 10
#define BIB 9
#define BOTON_ENCENDIDO 6

void setup(){
	pinMode(AIA, OUTPUT);
	pinMode(AIB, OUTPUT);
	pinMode(BIA, OUTPUT);
	pinMode(BIB, OUTPUT);

	pinMode(BOTON_ENCENDIDO, INPUT_PULLUP);

    while(digitalRead(BOTON_ENCENDIDO));
}

void loop(){
	for(int i=0; i<=260; i+=10){
		analogWrite(AIA, 0);
		analogWrite(AIB, i);
		analogWrite(BIA, i);
		analogWrite(BIB, 0);
		delay(3000);
		}
}