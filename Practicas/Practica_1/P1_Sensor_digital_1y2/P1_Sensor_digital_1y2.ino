#include <NewPing.h> // Libreria para la comunicación con el sensor de ultrasonido

#define TRIGGER_PIN  6    // Pin de arduino conectado al pin trigger del sensor de ultrasonido.
#define ECHO_PIN     7    // Pin de arduino conectado al pin echo del sensor de ultrasonido.
#define MAX_DISTANCE 100  // Distancia máxima que queremos medir (en cm). El sensor tiene un límite de 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Se inicializa el sensor con los parametros definidos.

// Variables para medir el tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

// Frecuencia de medición
float frec = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Cada onda de sonido emitida tarda unos 30 ms en desvanecerse, si no esperamos lo suficiente entre pings
  // podemos enviar un ping y leer la vuelta de un ping anterior lo que resulta en una distancia medida mucho
  // menor a la real.
  delay(50);

  Serial.print("Ping: ");
  
  // Mido el tiempo que tardo en medir. Para distancias mayores tardo mas ya que debo esperar el ida y vuelta
  // de la onda de sonido.
  startTime = micros();
  // El sonido tarda 29.287 us en viajar un centimetro, si medimos algo a 1 cm de distancia el sonido tarda
  // 2*29.287 us en ir y volver al sensor.
  Serial.print(sonar.ping()/(2*29.287));
  endTime = micros();
  
  Serial.println(" cm");

  // Se ve como la frecuencia de medición varía entre 200 y 400 Hz segun la distancia de medición
  frec = 1000000/(endTime-startTime);

  Serial.print("Frec = ");
  Serial.print(frec);
  Serial.println(" Hz");
}
