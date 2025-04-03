
int sensorPin = A0;   // Pin donde se conecta el potenciómetro
int sensorValue = 0;  // Variable para guardar el valor medido

// Variables para medir tiempos
unsigned long startTime = 0;
unsigned long endTime = 0;

float frec = 0; // Frecuencia de medición

/*------------------------------------------------*/
void setup() {
  // Inicio el serial para imprimir en consola (como el LOGGER de embebidos)
  Serial.begin(115200);

}

void loop() {
  // Mido el tiempo que se tarda en obrener el valor de ADC
  startTime = micros();
  sensorValue = analogRead(sensorPin);
  endTime = micros();

  // Calculo la frecuencia de medición, el tiempo esta en microsegundos
  // Se observa una frecuenci de medición de unos 8 kHz
  frec = 1000000.0/(endTime - startTime);

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  Serial.print("Frec. de lectura = ");
  Serial.println(frec);

  delay(1000); // Espero 1s
}
