#include <Servo.h>

Servo myservo;  // create Servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object

}

void loop() {
  // put your main code here, to run repeatedly:

  // myservo.writeMicroseconds(1000);
  // delay(2000);
  // myservo.writeMicroseconds(1500);
  // delay(2000);
  // myservo.writeMicroseconds(2000);
  // delay(2000);
  // myservo.writeMicroseconds(1500);
  // delay(2000);

  myservo.write(0);
  delay(2000);
  myservo.write(90);
  delay(2000);
  myservo.write(180);
  delay(2000);
  myservo.write(90);
  delay(2000);

}
