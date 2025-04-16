#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

#define PI 3.14159;

float alpha = 0.1;

void setup() {
  // Setup copiado del ejemplo de uso de la libreria

  // Se inicia el serial y se espera a que este listo
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Se inicia la IMU
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }

  // Se definen los fondos de escala (Valores extremos que se va a medir)
  // En este caso, la salida del acelerometro será máxima cuando la aceleración mayor o igual a 8 veces la de la gravedad
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // En este caso, la salida del giroscopio será máxima cuando la velocidad angular sea mayor o igual a 500°/seg
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // Se define el ancho de banda del filtro interno de la IMU.
  // Usamos un valor intermedio para implementar nuestro propio filtro en el controlador
  // Un bajo ancho de banda nos da una medición suave pero podria introducir retardos
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  delay(100);
}

void loop() {
  startTime = micros();

  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!

  static float theta_g = 0;
  theta_g = theta_g + (g.gyro.x * 0.02) * 180/PI;

  float theta_a = ( atan2(a.acceleration.y, a.acceleration.z) ) * 180/PI;


  static float theta_best = 0;
  float theta_g_best = theta_best + (g.gyro.x * 0.02) * 180/PI;
  theta_best = alpha * theta_a + (1-alpha) * theta_g_best;


  matlab_send(theta_g, theta_a, theta_g_best, theta_best);
  // theta_g = theta_(mejor) + g_x * delta_t (0.02)
  // theta_a = f(a_z, a_y) atan2

  // theta_(mejor) = alpha * theta_a + (1-alpha) * theta_g

  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

// Funcion para enviar datos a matlab, se podría pasar un array como parametro en vez de los datos por separado
void matlab_send(float dato1, float dato2, float dato3, float dato4){
  // Encabezado que marca el comienzo de los datos
  Serial.write("abcd");

  // Utilizo un puntero a byte y lo apunto al comienzo de cada dato
  // Un float ocupa 4 Bytes por lo que envio por el canal serial los
  // 4 Bytes empezando desde la posicion del puntero b
  byte * b = (byte *) &dato1;
  Serial.write(b,4);
  
  // Repito para todos los datos
  b = (byte *) &dato2;
  Serial.write(b,4);
  b = (byte *) &dato3;
  Serial.write(b,4);
  b = (byte *) &dato4;
  Serial.write(b,4);
}
