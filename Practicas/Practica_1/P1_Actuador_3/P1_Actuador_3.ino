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
  // put your setup code here, to run once:
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  startTime = micros();

  sensorValue = analogRead(sensorPin);

  // Mapeo toda la barrida del pote a valores entre 0° y 180°
  angulo = sensorValue * (180.0/1023.0);
  
  Serial.println(angulo);
  myservo.write(angulo);

  endTime = micros();

  delay(periodo - (endTime-startTime)/1000.0);
}
