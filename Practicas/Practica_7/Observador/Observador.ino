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
#define ALPHA 0.08
#define CORRECCION_SERVO 85
#define CORRECCION_IMU 2.2

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

  float u = cuadrada(0, 30);

  float A [2][2] = { {1 ,0.02}, {-0.3474, -3.038} } ;
  float L[2] = {-2.06, 465.69};
  float B[2] = {0, 1.4676} ;
  
  float medicion[2] = {0, 0};
  medir_angulo(medicion);
  
  float angulo = medicion[0] + CORRECCION_IMU;
  float velocidad = medicion[1];

  static float angulo_est_ant = 0; 
  static float velocidad_est_ant = 0;

  float angulo_est = A[0][0] * angulo_est_ant + A[0][1] * velocidad_est_ant + L[0] * (angulo - angulo_est_ant) + B[0] * u;
  float velocidad_est = A[1][0] * angulo_est_ant + A[1][1] * velocidad_est_ant + L[1] * (angulo - angulo_est_ant) + B[1] * u;

  angulo_est_ant = angulo_est;
  velocidad_est_ant = velocidad_est;

  matlab_send(u, angulo, velocidad, angulo_est, velocidad_est);
  // theta_g = theta_(mejor) + g_x * delta_t (0.02)
  // theta_a = f(a_z, a_y) atan2

  // theta_(mejor) = alpha * theta_a + (1-alpha) * theta_g

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

float cuadrada(float inicial, float final){
  static int angulo_servo = 0;
  static int contador = 0;
  if(contador == 0){
    angulo_servo = inicial;
    myservo.write(angulo_servo + CORRECCION_SERVO);
  }
  
  if(contador==200){
    angulo_servo = final;
    myservo.write(angulo_servo + CORRECCION_SERVO);
  }

  contador++;
  if(contador==400)
    contador = 0;
  
  return angulo_servo;
}

void medir_angulo(float *med){
  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!

  static float theta_g = 0;
  theta_g = theta_g + (g.gyro.x * 0.02) * 180/PI;

  float theta_a = ( atan2(a.acceleration.y, a.acceleration.z) ) * 180/PI;


  static float theta_best = 0;
  float theta_g_best = theta_best + (g.gyro.x * 0.02) * 180/PI;
  theta_best = ALPHA * theta_a + (1-ALPHA) * theta_g_best;

  med[0] = theta_best;
  med[1] = g.gyro.x * 180/PI;
}

void matlab_send(float dato1, float dato2, float dato3, float dato4, float dato5){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte * b = (byte *) &dato1;
  Serial.write(b,4);
  b = (byte *) &dato2;
  Serial.write(b,4);
  b = (byte *) &dato3;
  Serial.write(b,4);
  b = (byte *) &dato4;
  Serial.write(b,4);
  b = (byte *) &dato5;
  Serial.write(b,4);
}
