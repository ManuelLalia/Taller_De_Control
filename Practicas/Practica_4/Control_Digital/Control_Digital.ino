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

  static float error = 0;
  static float error_ant = 0;
  static float salida = 0;
  static float salida_ant = 0;
  static float angulo_servo = 0;
  static float angulo_servo_ant = 0;

  float ref = 0;

  salida_ant = salida;
  salida = medir_angulo();
  
  error_ant = error;
  error = ref - salida;

  angulo_servo_ant = angulo_servo;
  
  // Bilineal 1: 0.15136; 2: 0.085114
  // angulo_servo = 0.085114 * (error + error_ant) + angulo_servo_ant;

  // Forward
  angulo_servo = 0.30271 * error_ant + angulo_servo_ant;

  // Backwards
  // angulo_servo = 0.30271 * error + angulo_servo_ant;

  myservo.write(angulo_servo + CORRECCION_SERVO);


  // matlab_send(angulo, angulo_servo);

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

  return theta_best + CORRECCION_IMU;
}

void matlab_send(float dato1, float dato2){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte * b = (byte *) &dato1;
  Serial.write(b,4);
  b = (byte *) &dato2;
  Serial.write(b,4);
}
