#include <NewPing.h> // Libreria para la comunicación con el sensor de ultrasonido

#define TRIGGER_PIN  6    // Pin de arduino conectado al pin trigger del sensor de ultrasonido.
#define ECHO_PIN     7    // Pin de arduino conectado al pin echo del sensor de ultrasonido.
#define MAX_DISTANCE 100  // Distancia máxima que queremos medir (en cm). El sensor tiene un límite de 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Se inicializa el sensor con los parametros definidos.

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;


void setup() {
  Serial.begin(115200);
}

void loop() {
  // Mido el tiempo que tardo en ejecutar todas las instrucciones
  startTime = micros();
  
  Serial.print("Ping: ");
  Serial.print(sonar.ping()/(2*29.287));
  endTime = micros();
  Serial.println(" cm");

  endTime = micros();

  // Calculo el delay necesario para que se ejecute el loop a 50 Hz
  delay(20 - (endTime-startTime)/1000.0);  
}
