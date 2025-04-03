#include <Servo.h>

Servo myservo;  // create Servo object to control a servo

int sensorPin = A0;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

float angulo = 0;

unsigned long startTime = 0;
unsigned long endTime = 0;

// Cambiar para correr el loop principal a distintas frecuencias
// const int periodo = 20;  // ms -> 50Hz
// const int periodo = 100; // ms -> 10Hz
const int periodo = 1000;   // ms ->  1Hz

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(115200);
}

void loop() {
  // Mido el tiempo que tardo en ejecutar todas las acciones
  startTime = micros();

  sensorValue = analogRead(sensorPin);

  // Mapeo toda la barrida del pote a valores entre 0° y 180°
  angulo = sensorValue * (180.0/1023.0);
  
  Serial.println(angulo);
  myservo.write(angulo);

  endTime = micros();

  // Calculo el delay para hacer una vuelta del programa con el período definido
  delay(periodo - (endTime-startTime)/1000.0);
}
