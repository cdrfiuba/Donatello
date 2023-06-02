/*#include <Arduino.h>
#define PIN_BTN    2

typedef enum {
  INICIO,
  CARRERA,
  PID,
  FANCYFANCY,
  FINISH
} caso_t;

caso_t caso =INICIO;

const int sensorPin = 9;
const int PinM1 = 10; //pines motores
const int PinM2 =11;


void setup() {
  Serial.begin(9600);   //iniciar puerto serie
  pinMode(PIN_BTN, INPUT);  //definir pin como entrada
  pinMode(sensorPin, INPUT);  //definir pin como entrada
  pinMode(PinM1, OUTPUT);  //definir pin como salida
  pinMode(PinM2, OUTPUT);  //definir pin como salida
}
 

 void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(200);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(200);                      // wait for a second
}

void loop(){

  


  switch (caso) {
    case INICIO:
      if(digitalRead(PIN_BTN) == 1){      
        Serial.print("Cambio al caso carrera\n");
        caso = CARRERA;
      }
    break;

    case CARRERA:
      if (digitalRead(PIN_BTN) == 1){
        Serial.print("Cambio al caso PID\n");
        caso = PID;
      }
    break;

    case PID:
    if (digitalRead(PIN_BTN) == 1){
    Serial.print("Caso INICIO\n");
      caso = INICIO;
    }
    break;

  default:
    caso = CARRERA;
  break;

  }

}*/
