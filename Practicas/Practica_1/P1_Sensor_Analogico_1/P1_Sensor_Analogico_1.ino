
int sensorPin = A0;   // Pin donde se conecta el potenciómetro
int sensorValue = 0;  // Variable para guardar el valor medido

// Variables para medir tiempos
unsigned long startTime = 0;
unsigned long endTime = 0;

// Variables globales
float frec = 0;

/*------------------------------------------------*/
void setup() {
  // Inicio el serial para imprimir en consola
  Serial.begin(115200);

}

void loop() {
  // Mido el tiempo que se tarda en medir la entrada analógica
  startTime = micros();
  sensorValue = analogRead(sensorPin);
  endTime = micros();

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  // Calculo la frecuencia de medición, el tiempo esta en microsegundos
  frec = 1000000.0/(endTime - startTime);

  Serial.print("Frec. de lectura = ");
  Serial.println(frec);

  delay(1000); // Espero 1s
}
