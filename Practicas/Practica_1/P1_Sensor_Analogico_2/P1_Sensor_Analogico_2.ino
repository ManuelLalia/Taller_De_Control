
int sensorPin = A0;   // Pin donde se conecta el potenciómetro
int sensorValue = 0;  // Variable para guardar el valor medido

float angulo = 0; // Angulo del potenciometro

void setup() {
  // Inicio el serial para imprimir en consola
  Serial.begin(115200);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  // El valor del potenciómetro se representa entre 0 y 1023
  // El pote. no da toda la vuelta, barre aprox 300°
  angulo = sensorValue * (300.0/1023.0);

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  Serial.print("Angulo = ");
  Serial.println(angulo);

  delay(1000);
}
