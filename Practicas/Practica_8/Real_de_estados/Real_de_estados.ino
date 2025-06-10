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
#define ALPHA 0.1
#define CORRECCION_SERVO 85
#define CORRECCION_IMU -0.2128

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

  float ref = cuadrada(0, 10);
  static float u = 0;
  static float u_ant = 0;

  float A [3][3] = { {1 ,0.02, 0}, {-2.882, 0.6546, 0}, {0, 0, 1} };
  float L[3][2] = { {0.5834, 0.0033}, {-2.8476, -0.1451}, {-0.2459, 0.9315} };
  float B[3] = {0, 1.236, 0} ;

  
  float medicion[2] = {0, 0};
  medir_angulo(medicion);
  
  float angulo = medicion[0] + CORRECCION_IMU;
  float velocidad = medicion[1];

  static float angulo_est_ant = 0; 
  static float velocidad_est_ant = 0;

  float K[2] = {-1.7136, -0.2869};
  float F = 4.0453;
  
  u = F * ref + K[0] * angulo_est_ant + K[1] * velocidad_est_ant;

  // Serial.println(u);
  comandarServo(u);

  static float bias_est_ant = 0;
  float angulo_est = A[0][0] * angulo_est_ant + A[0][1] * velocidad_est_ant + A[0][2] * bias_est_ant + L[0][0] * (angulo - angulo_est_ant) + L[0][1] * (velocidad - velocidad_est_ant - bias_est_ant) + B[0] * u;
  float velocidad_est = A[1][0] * angulo_est_ant + A[1][1] * velocidad_est_ant + A[1][2] * bias_est_ant + L[1][0] * (angulo - angulo_est_ant) + L[1][1] * (velocidad - velocidad_est_ant - bias_est_ant) + B[1] * u;
  float bias_est = A[2][0] * angulo_est_ant + A[2][1] * velocidad_est_ant + A[2][2] * bias_est_ant + L[2][0] * (angulo - angulo_est_ant) + L[2][1] * (velocidad - velocidad_est_ant - bias_est_ant) + B[2] * u;

  angulo_est_ant = angulo_est;
  velocidad_est_ant = velocidad_est;
  bias_est_ant = bias_est;


  matlab_send(ref, angulo, velocidad, angulo_est, velocidad_est);
  // matlab_send(u, 0., velocidad-bias_est, bias_est, velocidad_est);
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
  theta_g = theta_g + ((g.gyro.x + 0.1138) * 0.02) * 180/PI; // Bias = 0.1138

  float theta_a = ( atan2(a.acceleration.y + 0.1840, a.acceleration.z - 2.1528) ) * 180/PI;


  static float theta_best = 0;
  float theta_g_best = theta_best + ((g.gyro.x + 0.1138) * 0.02) * 180/PI; // Bias = 0.1138
  theta_best = ALPHA * theta_a + (1-ALPHA) * theta_g_best;

  med[0] = theta_best;
  med[1] = (g.gyro.x + 0.1138) * 180/PI; // Bias = 0.1138
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
