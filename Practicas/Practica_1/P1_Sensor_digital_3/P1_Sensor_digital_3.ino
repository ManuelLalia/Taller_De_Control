#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long startTime = 0;
unsigned long endTime = 0;

float frec = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  // put your main code here, to run repeatedly:
  startTime = micros();
  
  Serial.print("Ping: ");
  Serial.print(sonar.ping()/(2*29.287));
  endTime = micros();
  Serial.println(" cm");

  endTime = micros();

  delay(20 - (endTime-startTime)/1000.0);
  
}
