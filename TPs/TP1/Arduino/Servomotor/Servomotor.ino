#include <Servo.h>

Servo myservo;  // create Servo object to control a servo

int SensorPin = A0;   // Pin del potenciometro
int SensorValue = 0;  // Almacenar el valor del Sensor

float angulo = 0;  // Almacenar el ángulo

// Variables para medir tiempos
unsigned long StartTime = 0;
unsigned long EndTime = 0;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(115200);
}

void ejercicio3() {
  // Mapeo toda la barrida del pote a valores entre 60° y 120°
  SensorValue = analogRead(SensorPin);
  angulo = SensorValue * (60.0 / 1023.0) + 60.0;
  Serial.print("angulo = ");
  Serial.print(angulo);
  Serial.println("°");

  // Se lo paso al servo
  myservo.write(angulo);
}

void loop() {

  StartTime = micros();

  // myservo.writeMicroseconds(1000); // 1 ms, la funcion recibe en us, se le debe pasar 1ms, 1.5ms y 2ms
  // myservo.write(60); // le paso 60° porque esta en la planta del TP (no da fisicamente de 0° a 180°)
  // ejercicio3();

  EndTime = micros();

  delay(20 - (EndTime - StartTime) / 1000.0);  // para que me de exactamente 50 Hz (micros esta en us)
}
