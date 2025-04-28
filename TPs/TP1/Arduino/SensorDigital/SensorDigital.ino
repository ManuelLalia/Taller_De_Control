#include <NewPing.h>

#define TRIGGER_PIN  6    // Pin de arduino conectado al Trigger.
#define ECHO_PIN     7    // Pin de arduino conectado al Echo.
#define MAX_DISTANCE 100  // Distancia máxima que queremos medir [cm]

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Se inicializa el sensor con los parametros definidos.

// Variables para medir el tiempo
unsigned long StartTimeSensor = 0;
unsigned long EndTimeSensor = 0;
unsigned long StartTime = 0;
unsigned long EndTime = 0;

float DistanciaMedida = 0; // Distancia que se mide
float TiempoMedido = 0;
float frec = 0; // Frecuencia de medición

void setup() {
  Serial.begin(115200);
}

void loop() {
  StartTime = micros();

  // calculo el tiempo que se tarda en medir para tener la frecuencia
  StartTimeSensor = micros();
  TiempoMedido = sonar.ping() * 1.0; 
  EndTimeSensor = micros();

  float TiempoSensor = EndTimeSensor - StartTimeSensor;

  DistanciaMedida = TiempoMedido * 1 / (2 * 29.287); // 29.287us --> 1cm y el sensor mide ida y vuelta

  // valor de la medición
  Serial.print("Distancia: ");
  Serial.print(DistanciaMedida);
  Serial.println(" cm");

  // frecuencia de medición
  frec = 1000000.0 / TiempoSensor;
  Serial.print("Frec = ");
  Serial.print(frec);
  Serial.println(" Hz");

  Serial.print("Tiempo = ");
  Serial.print(TiempoSensor);
  Serial.println(" us");
  
  static float aux = 0;
  static float contador = 0.0;
  if (contador < 500.0) {
    aux += TiempoSensor;
    contador++;
  }

  if (contador == 500.0) {
    float promedio = aux / contador;
    Serial.print("Promedio = ");
    Serial.print(promedio);
    Serial.println(" us");
  }

  matlab_send(DistanciaMedida, TiempoMedido);
  EndTime = micros();

  delay(20 - (EndTime - StartTime) / 1000.0); // para que me de exactamente 50 Hz (micros esta en us)
}


void matlab_send(float dato1, float dato2){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte * b = (byte *) &dato1;
  Serial.write(b, 4);
  b = (byte *) &dato2;
  Serial.write(b, 4);
}