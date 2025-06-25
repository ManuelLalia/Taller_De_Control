#include <Servo.h>  // Libreria para controlar el servo
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <NewPing.h>  // Libreria para la comunicación con el sensor de ultrasonido

#define TRIGGER_PIN 6    // Pin de arduino conectado al pin trigger del sensor de ultrasonido.
#define ECHO_PIN 7       // Pin de arduino conectado al pin echo del sensor de ultrasonido.
#define MAX_DISTANCE 40  // Distancia máxima que queremos medir (en cm). El sensor tiene un límite de 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // Se inicializa el sensor con los parametros definidos.

Adafruit_MPU6050 mpu;  // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

Servo myservo;  // create Servo object to control a servo
#define N_CALIB 100
bool calibrando = true;

#define PI 3.14159
#define ALPHA 0.1
#define CORRECCION_SERVO 88  //85
#define CORRECCION_IMU 0     //-2.05 //-0.2128

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens
  }

  // Se inicia la IMU
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  myservo.attach(9);  // Indico el pin PWM de arduino que controla al servo

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  myservo.write(-15 + CORRECCION_SERVO);
  delay(1000);
  myservo.write(-15 + CORRECCION_SERVO);
  delay(1000);
  myservo.write(0 + CORRECCION_SERVO);
  delay(1000);
}

void loop() {
  startTime = micros();

  static float bias_ang = 0;
  static int calib_cont = 0;
  if (calibrando) {
    myservo.write(0 + CORRECCION_SERVO);
    Serial.println("Calibrando...");
    static float suma = 0;
    float medicion[2] = { 0, 0 };
    medir_angulo(medicion);

    Serial.println(medicion[0]);
    suma += medicion[0];
    calib_cont++;

    if (calib_cont >= N_CALIB) {
      bias_ang = suma / N_CALIB;
      calibrando = false;
      Serial.println("Offset calibrado: ");
      Serial.println(bias_ang);
    }
    return;  // todavía calibrando, no hacemos nada más
  }

  // float u = cuadrada(-12, 10, 40, 80);
  // float u = obs(30);
  // float u = 0;
  // comandarServo(u);

  float A[4][4] = { { 1, 0.02, 0, 0 }, { 0, 0.9355, 0.3424, 0 }, { 0, 0, 1, 0.02 }, { 0, 0, -2.882, 0.6546 } };
  float B[4] = { 0, 0, 0, 1.236 };
  // x5
  // float L[4][2] = { {0.5377, -0.0241}, {5.0571, 0.7687}, {1.9976, 0.8539}, {-4.8768, -4.2470} };

  // x3
  float L[4][2] = { { 0.6242, 0.0089 }, { 2.6002, 0.3718 }, { -0.1021, 0.4846 }, { -0.2325, -2.1196 } };

  float medicion[2] = { 0, 0 };
  medir_angulo(medicion);

  float pos = medir_distancia();
  static float pos_ant = pos;
  float vel = (pos - pos_ant) / 0.02;
  pos_ant = pos;

  float ang = medicion[0] - bias_ang;
  float w = medicion[1];

  static float pos_est_ant = 0;
  static float vel_est_ant = 0;
  static float ang_est_ant = 0;
  static float w_est_ant = 0;

  // Realimentación de estados
  float K[4] = {-1.3077, -0.4431, -0.1525, -0.0001};
  float u = K[0] * pos_est_ant + K[1] * vel_est_ant + K[2] * ang_est_ant + K[3] * w_est_ant;
  u = limitar_u(u);
  comandarServo(u);

  // Seguimiento de referencias
  // static float ref[2] = {0., 0.};
  // ref[0] = cuadrada(-9, 9, 250, 500);
  // float K[4] = {-1.3077, -0.4431, -0.1525, -0.0001};
  // float F[2] = {1.3077, 0.};
  // float u = K[0]*pos_est_ant + K[1]*vel_est_ant + K[2]*ang_est_ant + K[3]*w_est_ant + F[0]*ref[0] + F[1]*ref[1];
  // u = limitar_u(u);
  // comandarServo(u);

  // Acción integral
  // static float ref = 0;
  // // ref = cuadrada(-10, 10, 400, 800);

  // static float q_ant = 0;
  // float e = ref - pos;
  // float q = q_ant + 0.02 * e;



  // float K[4] = { -5.6067, -1.1095, -2.2399, -0.2019 };
  // float H = 6.9466;
  // float u = K[0] * pos_est_ant + K[1] * vel_est_ant + K[2] * ang_est_ant + K[3] * w_est_ant + H * q;
  // u = limitar_u(u);
  // comandarServo(u);

  float pos_est = A[0][0] * pos_est_ant + A[0][1] * vel_est_ant + A[0][2] * ang_est_ant + A[0][3] * w_est_ant + L[0][0] * (pos - pos_est_ant) + L[0][1] * (ang - ang_est_ant) + B[0] * u;
  float vel_est = A[1][0] * pos_est_ant + A[1][1] * vel_est_ant + A[1][2] * ang_est_ant + A[1][3] * w_est_ant + L[1][0] * (pos - pos_est_ant) + L[1][1] * (ang - ang_est_ant) + B[1] * u;
  float ang_est = A[2][0] * pos_est_ant + A[2][1] * vel_est_ant + A[2][2] * ang_est_ant + A[2][3] * w_est_ant + L[2][0] * (pos - pos_est_ant) + L[2][1] * (ang - ang_est_ant) + B[2] * u;
  float w_est = A[3][0] * pos_est_ant + A[3][1] * vel_est_ant + A[3][2] * ang_est_ant + A[3][3] * w_est_ant + L[3][0] * (pos - pos_est_ant) + L[3][1] * (ang - ang_est_ant) + B[3] * u;

  pos_est_ant = pos_est;
  vel_est_ant = vel_est;
  ang_est_ant = ang_est;
  w_est_ant = w_est;
  // q_ant = q;
  // Serial.println(ang);
  // Serial.println(u);


  matlab_send(0, pos, pos_est, vel, vel_est, ang, ang_est, w, w_est);
  // matlab_send(ref[0], pos, pos_est, vel, vel_est, ang, ang_est, w, w_est);
  // matlab_send(ref, pos, pos_est, vel, vel_est, ang, ang_est, w, w_est);

  endTime = micros();

  float del = 20 - (endTime - startTime) / 1000.0;
  // Serial.println(del);
  if (del > 0) {
    delay(del);
  }
}

float obs(int t1) {
  int valores[4] = { -15, 0, 15, 0 };
  static int i = 0;
  static int j = 0;
  int angulo;
  if (i == 0 * t1) {
    angulo = valores[j];
  }
  if (i == 1 * t1) {
    j++;
    angulo = valores[j];
  }
  if (i == 2 * t1) {
    j++;
    angulo = valores[j];
  }
  if (i == 3 * t1) {
    j++;
    angulo = valores[j];
  }
  i++;
  if (i == 4 * t1) {
    i = 0;
    j = 0;
  }
  return angulo;
}

float cuadrada(float inicial, float final, float t1, float t2) {
  static int angulo_servo = inicial;
  static int contador = 0;
  if (contador == 0) {
    angulo_servo = inicial;
  }

  if (contador == t1) {
    angulo_servo = final;
  }

  contador++;
  if (contador == t2)
    contador = 0;

  return angulo_servo;
}
// 16.3
float medir_distancia() {
  return sonar.ping() / (2 * 29.287) - 16.3;
}

void medir_angulo(float *med) {
  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!

  static float theta_g = 0;
  theta_g = theta_g + ((g.gyro.x + 0.1138) * 0.02) * 180 / PI;  // Bias = 0.1138

  float theta_a = (atan2((a.acceleration.y - 0.11) * 0.9944, (a.acceleration.z - 1.375) * 0.9284)) * 180 / PI;

  static float theta_best = 0;
  float theta_g_best = theta_best + ((g.gyro.x + 0.1138) * 0.02) * 180 / PI;  // Bias = 0.1138
  theta_best = ALPHA * theta_a + (1 - ALPHA) * theta_g_best;

  med[0] = theta_best + CORRECCION_IMU;
  med[1] = (g.gyro.x + 0.1138) * 180 / PI;  // Bias = 0.1138
}

float limitar_u(float u) {
  float u_lim;
  if (u >= -35 && u <= 35) {
    u_lim = u;
  } else if (u < -35) {
    u_lim = -35;
  } else if (u > 35) {
    u_lim = 35;
  } else {
    u_lim = u;
  }
  return u_lim;
}

void comandarServo(float angulo) {
  if (angulo >= -35 && angulo <= 35) {
    myservo.write(angulo + CORRECCION_SERVO);
  }
  return;
}

void matlab_send(float dato1, float dato2, float dato3, float dato4, float dato5, float dato6, float dato7, float dato8, float dato9) {
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  byte *b = (byte *)&dato1;
  Serial.write(b, 4);
  b = (byte *)&dato2;
  Serial.write(b, 4);
  b = (byte *)&dato3;
  Serial.write(b, 4);
  b = (byte *)&dato4;
  Serial.write(b, 4);
  b = (byte *)&dato5;
  Serial.write(b, 4);
  b = (byte *)&dato6;
  Serial.write(b, 4);
  b = (byte *)&dato7;
  Serial.write(b, 4);
  b = (byte *)&dato8;
  Serial.write(b, 4);
  b = (byte *)&dato9;
  Serial.write(b, 4);
}
