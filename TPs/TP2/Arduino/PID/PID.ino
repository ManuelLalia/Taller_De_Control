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

#define Ts 0.02

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
  // static float salida_ant = 0;
  static float angulo_servo = 0;
  // static float angulo_servo_ant = 0;

  float ref = 16;
  static int contador = 0;
 
  // if(contador > 2000){
  //   ref = 16;
  //   contador = 0;
  // }else if(contador > 1000){
  //     ref = 26;
  // }
  contador++;


  salida = medir_distancia();
  error = ref - salida;
  if(abs(error) < 0.4){
    error = 0;
  }

  static float Kp = 1.8; // 3
  static float Kd = 0;
  static float Ki = 0.2; //0.75

  static float Ik_ant = 0; 
  float Ik = Ik_ant + Ts/2 * (error + error_ant);

  static float Dk_ant = 0;
  static float Dk = 0;
  if(contador > 100){ 
    Dk = 2 * (error - error_ant)/Ts - Dk_ant;
  }
  // Serial.println(Dk);
  // delay(10000);
  // if(Dk > 10)
  //   Dk = 10;
  // else if(Dk < -10)
  //   Dk = -10;

  angulo_servo = Kp * error + Ki * Ik + Kd * Dk;
  comandarServo(angulo_servo);

  error_ant = error;
  Ik_ant = Ik;
  Dk_ant = Dk;

  matlab_send(angulo_servo, salida - 16, error, Ik, Dk);
  // matlab_send(Ik, Dk, error);
  // Serial.println(error);

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

float medir_distancia(){
  return sonar.ping()/(2*29.287);
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

void matlab_send(float dato1, float dato2, float dato3, float dato4, float dato5)
{
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
