#include <Servo.h> // Libreria para controlar el servo

Servo myservo;  // create Servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // Indico el pin PWM de arduino que controla al servo
}

void loop() {
  // Ejercicio 1: Se controla el ancho del pulso PWM que se envía al servo

  // myservo.writeMicroseconds(1000);
  // delay(2000);
  // myservo.writeMicroseconds(1500);
  // delay(2000);
  // myservo.writeMicroseconds(2000);
  // delay(2000);
  // myservo.writeMicroseconds(1500);
  // delay(2000);

/*-----------------------------------------------------------------------------------------*/

  // Ejercicio 2: Se controla la posición del servo pasando un angulo entre 0° y 180°

  myservo.write(0);
  delay(2000);
  myservo.write(90);
  delay(2000);
  myservo.write(180);
  delay(2000);
  myservo.write(90);
  delay(2000);

}
