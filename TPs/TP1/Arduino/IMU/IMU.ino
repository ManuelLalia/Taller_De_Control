#include <Servo.h> // Libreria para controlar el servo
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long IMUStartTime = 0;
unsigned long IMUEndTime = 0;
unsigned long AngleStartTime = 0;
unsigned long AngleEndTime = 0;

unsigned long FloatStartTime = 0;
unsigned long FloatEndTime = 0;

Servo myservo;  // create Servo object to control a servo

#define PI 3.14159
#define ALPHA 0.08
#define CORRECCION_SERVO 85
#define CORRECCION_IMU 1.56

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

  int angulo_servo = 0;
  // myservo.write(angulo_servo + CORRECCION_SERVO);
  // myservo.writeMicroseconds(1500);
  
  AngleStartTime = micros();
  float medicion = medir_angulo();
  AngleEndTime = micros();
  
  float TiempoAngle = AngleEndTime - AngleStartTime;
  // Serial.print("Tiempo ángulo: ");
  // Serial.print(TiempoAngle);
  // Serial.println(" us");
  float angulo = medicion + CORRECCION_IMU;

  static float aux = 0;
  static float contador = 0.0;
  if (contador < 100.0) {
    aux += TiempoAngle;
    contador++;
  }

  else if (contador < 200.0) {
    float promedio = aux / contador;
    // Serial.print("Promedio = ");
    // Serial.print(promedio);
    // Serial.println(" us");
  }

  static float tiempo = 0;

  FloatStartTime = micros();
  for(int i=0; i<100; i++){
    // matlab_send(angulo);
    Serial.print(angulo);
  }
  FloatEndTime = micros();

  // matlab_send(FloatEndTime - FloatStartTime);

  Serial.print("\nTiempo float = ");
  Serial.println( FloatEndTime - FloatStartTime);

  // theta_g = theta_(mejor) + g_x * delta_t (0.02)
  // theta_a = f(a_z, a_y) atan2

  // theta_(mejor) = alpha * theta_a + (1-alpha) * theta_g

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

float medir_angulo(){
  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  
  IMUStartTime = micros();
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!
  IMUEndTime = micros();
  
  float TiempoIMU = IMUEndTime - IMUStartTime;
  // Serial.print("Tiempo IMU: ");
  // Serial.print(TiempoIMU);
  // Serial.println(" us");

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
  // b = (byte *) &dato2;
  // Serial.write(b,4);
}