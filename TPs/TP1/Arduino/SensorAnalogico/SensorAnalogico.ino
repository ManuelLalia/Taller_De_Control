int SensorPin = A0;   // Pin donde se conecta el potenciómetro
int SensorValue = 0;  // Variable para guardar el valor medido

float angulo = 0; // Angulo del potenciometro

// Variables para medir tiempos
unsigned long StartTimeSensor = 0;
unsigned long EndTimeSensor = 0;
unsigned long StartTime = 0;
unsigned long EndTime = 0;

float frec = 0; // Frecuencia de medición

void setup() {
  // Inicio el serial para imprimir en consola
  Serial.begin(115200);
}

void loop() {
  StartTime = micros();
  
  // calculo el tiempo que se tarda en leer para tener la frecuencia
  StartTimeSensor = micros();
  SensorValue = analogRead(SensorPin);
  EndTimeSensor = micros();

  float TiempoSensor = EndTimeSensor - StartTimeSensor; 
  // valor de la medición
  Serial.print("sensor = ");
  Serial.println(SensorValue);

  // frecuencia de medición (aprox 8.6k - 8.9k)
  frec = 1000000.0 / TiempoSensor;
  Serial.print("Frec. de lectura = ");
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
    Serial.print("promedio = ");
    Serial.print(promedio);
    Serial.println(" us");
  }
  
  EndTime = micros();

  delay(20 - (EndTime-StartTime) / 1000.0); // para que me de exactamente 50 Hz (micros esta en us)
}