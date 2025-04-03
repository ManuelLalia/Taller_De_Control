
int sensorPin = A0;   // Pin donde se conecta el potenciómetro
int sensorValue = 0;  // Variable para guardar el valor medido

float angulo = 0; // Angulo del potenciometro

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

void setup() {
  // Inicio el serial para imprimir en consola
  Serial.begin(115200);
}

void loop() {
  // Mido el tiempo que tardo en ejecutar todas las acciones
  startTime = micros();
  sensorValue = analogRead(sensorPin);

  angulo = sensorValue * (300.0/1023.0);

  Serial.print("sensor = ");
  Serial.println(sensorValue);

  Serial.print("Angulo = ");
  Serial.println(angulo);

  endTime = micros();

  // Quiero trabajar a 50Hz por lo que cada vuelta del programa debe durar 20ms
  // Mido cuanto tardan las instrucciones de código y calculo el delay necesario
  // El tiempo medido esta en us y el delay va en ms
  delay(20 - (endTime-startTime)/1000.0 );

  // delayMicroseconds() permitiría hacer un delay mas preciso pero tiene un delay
  // máximo permitido de 16383 us por lo que no nos alcanza para nuestros 20 ms 
}
