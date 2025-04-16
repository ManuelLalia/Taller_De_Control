#include <Servo.h> // Libreria para controlar el servo
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

Servo myservo;  // create Servo object to control a servo

#define PI 3.14159
#define ALPHA 0.15
#define CORRECCION_SERVO 85
#define CORRECCION_IMU 0.8

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

  myservo.write(0 + CORRECCION_SERVO);

  delay(100);
}

void loop() {
  startTime = micros();

  static int contador = 0;
  if(contador == 0)
    myservo.write(0 + CORRECCION_SERVO);
  
  if(contador==400)
    myservo.write(30 + CORRECCION_SERVO);

  contador++;

  if(contador==800)
    contador = 0;



  float medicion = medir_angulo();
  float angulo = medicion + CORRECCION_IMU;


  matlab_send(angulo);
  // theta_g = theta_(mejor) + g_x * delta_t (0.02)
  // theta_a = f(a_z, a_y) atan2

  // theta_(mejor) = alpha * theta_a + (1-alpha) * theta_g

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

float medir_angulo(){
  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!

  static float theta_g = 0;
  theta_g = theta_g + (g.gyro.x * 0.02) * 180/PI;

  float theta_a = ( atan2(a.acceleration.y, a.acceleration.z) ) * 180/PI;


  static float theta_best = 0;
  float theta_g_best = theta_best + (g.gyro.x * 0.02) * 180/PI;
  theta_best = ALPHA * theta_a + (1-ALPHA) * theta_g_best;

  return theta_best;
}

void matlab_send(float dato1){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte * b = (byte *) &dato1;
  Serial.write(b,4);
}
