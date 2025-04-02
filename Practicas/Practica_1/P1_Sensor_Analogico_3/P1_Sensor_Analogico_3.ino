
int sensorPin = A0;   // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

float angulo = 0;

unsigned long startTime = 0;
unsigned long endTime = 0;

void setup() {
  Serial.begin(115200);

}

void loop() {  
  // Mido el tiempo total
  startTime = micros();
  sensorValue = analogRead(sensorPin);

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  angulo = sensorValue * (300.0/1023.0);

  Serial.print("Angulo = ");
  Serial.println(angulo);

  endTime = micros();

  // Quiero trabajar a 50Hz por lo que cada vuelta del programa debe durar 20ms
  // Mido cuanto tardan las instrucciones de código y calculo el delay necesario
  // El tiempo medido esta en us y el delay va en ms
  delay(20 - (endTime-startTime)/1000.0 );
}
