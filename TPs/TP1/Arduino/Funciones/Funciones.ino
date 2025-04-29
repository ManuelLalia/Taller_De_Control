#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN  6    // Pin de arduino conectado al Trigger.
#define ECHO_PIN     7    // Pin de arduino conectado al Echo.
#define MAX_DISTANCE 100  // Distancia máxima que queremos medir [cm]
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Se inicializa el sensor con los parametros definidos.

#define CORRECCION_SERVO 85
Servo myservo;  // create Servo object to control a servo

int sensorPin = A0;   // select the input pin for the potentiometer

float angulo = 0;

unsigned long startTime = 0;
unsigned long endTime = 0;

// Cambiar para correr el loop principal a distintas frecuencias
// const int periodo = 20;  // ms -> 50Hz
// const int periodo = 100; // ms -> 10Hz
const int periodo = 1000;   // ms ->  1Hz

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(115200);
}

void loop() {
  startTime = micros()
  
  static int contador = 0;

  float referencia = tomarReferencia();
  float distancia = medirDistancia();

  if(contador < 10){
    comandarServo(0);
    contador = ++;
  }
  if(contador > 10){
    comandarServo(20);
    contador ++;
  }
  if(contador == 20){
    contador = 0;
  }

  Serial.print("Referencia potenciometro = ");
  Serial.print(referencia);
  Serial.print(" cm")

  Serial.print("Distancia medida = ");
  Serial.print(distancia);
  Serial.print(" cm")


  endTime = micros()
  delay(20 - (endTime-startTime)/1000.0);
}

// La distancia maxima es 34.8 cm, mapeo entre 0 y 32.8 y le sumo 2 para evitar
// valores menores al minimo que puede medir el sensor de distancia
float tomarReferencia(){
  int sensorValue = analog.read(sensorPin);
  return 2.0 + sensorValue * (32.8/1023.0);
}

void comandarServo(float angulo){
  if(angulo < -30){
    angulo = -30;
  } else if (angulo > 30){
    angulo = 30;
  }
  
  Serial.print("Angulo comandado = ");
  Serial.println(angulo);

  myservo.write(angulo + CORRECCION_SERVO);
  return;
}

float medirDistancia(){
  int sensorValue = sonar.ping(); // us
  return sensorValue / (2 * 29.287); // el tarda 29.287 us en viajar 1cm
}
