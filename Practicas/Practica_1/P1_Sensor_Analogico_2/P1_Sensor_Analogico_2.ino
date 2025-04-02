
int sensorPin = A0;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

float angulo = 0;


void setup() {
  Serial.begin(115200);

}

void loop() {
  sensorValue = analogRead(sensorPin);

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  // El valor del potenciómetro se representa entre 0 1023
  // El pote. no da toda la vuelta, barre aprox 300°
  angulo = sensorValue * (300.0/1023.0);

  Serial.print("Angulo = ");
  Serial.println(angulo);

  delay(1000);
}
