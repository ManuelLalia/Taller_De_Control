#include <Servo.h> // Libreria para controlar el servo
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <NewPing.h> // Libreria para la comunicación con el sensor de ultrasonido

#define TRIGGER_PIN  6    // Pin de arduino conectado al pin trigger del sensor de ultrasonido.
#define ECHO_PIN     7    // Pin de arduino conectado al pin echo del sensor de ultrasonido.
#define MAX_DISTANCE 100  // Distancia máxima que queremos medir (en cm). El sensor tiene un límite de 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Se inicializa el sensor con los parametros definidos.

Adafruit_MPU6050 mpu; // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

Servo myservo;  // create Servo object to control a servo

#define PI 3.14159
#define ALPHA 0.1
#define CORRECCION_SERVO 85
#define CORRECCION_IMU -0.2128
#define PUNTO_0_BARRA 0

void setup() {
  Serial.begin(115200);

  while (!Serial){
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Se inicia la IMU
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }
  myservo.attach(9);  // Indico el pin PWM de arduino que controla al servo

  while (!Serial){
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Se inicia la IMU
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  // myservo.write(0 + CORRECCION_SERVO);

  delay(100);
}

void loop() {
  startTime = micros();

  comandarServo(20);
  float distancia = medir_distancia();
  float angulo = medir_angulo();

  matlab_send(distancia, angulo);

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}



float medir_angulo(){
  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!

  static float theta_g = 0;
  theta_g = theta_g + ((g.gyro.x + 0.1138) * 0.02) * 180/PI;

  float theta_a = ( atan2(a.acceleration.y + 0.1840, a.acceleration.z - 2.1528) ) * 180/PI;


  static float theta_best = 0;
  float theta_g_best = theta_best + ((g.gyro.x + 0.1138) * 0.02) * 180/PI;
  theta_best = ALPHA * theta_a + (1-ALPHA) * theta_g_best;

  return theta_best + CORRECCION_IMU;
}

float medir_distancia(){
  return sonar.ping()/(2*29.287) - PUNTO_0_BARRA;
}

void comandarServo(float angulo){
  if(angulo < -30){
    angulo = -30;
  } else if (angulo > 30){
    angulo = 30;
  }
  
  myservo.write(angulo + CORRECCION_SERVO);
  return;
}

void matlab_send(float dato1, float dato2){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte * b = (byte *) &dato1;
  Serial.write(b,4);
  b = (byte *) &dato2;
  Serial.write(b,4);
}
