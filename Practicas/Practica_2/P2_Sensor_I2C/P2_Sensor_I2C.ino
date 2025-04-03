// Librerias para controlar la IMU
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; // Se define el objeto de la IMU

// Variables para medir tiempo
unsigned long startTime = 0;
unsigned long endTime = 0;

void setup() {
  // Setup copiado del ejemplo de uso de la libreria

  // Se inicia el serial y se espera a que este listo
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Se inicia la IMU
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");


  // Se definen los fondos de escala (Valores extremos que se va a medir)
  // En este caso, la salida del acelerometro será máxima cuando la aceleración mayor o igual a 8 veces la de la gravedad
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // En este caso, la salida del giroscopio será máxima cuando la velocidad angular sea mayor o igual a 500°/seg
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  // Se define el ancho de banda del filtro interno de la IMU.
  // Usamos un valor intermedio para implementar nuestro propio filtro en el controlador
  // Un bajo ancho de banda nos da una medición suave pero podria introducir retardos
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  // Mido el tiempo que tardo en ejecutar todas las instrucciones
  startTime = micros();

  sensors_event_t a, g, temp;   // Defino las variables para leer los sensores
  mpu.getEvent(&a, &g, &temp);  // Leo los sensores. ¡¡ El valor de la velocidad angular está en radianes por segundo !!
  
  // Envío los valores medidos a simulink
  matlab_send(a.acceleration.x, a.acceleration.y, a.acceleration.z, g.gyro.x, g.gyro.y, g.gyro.z);
  endTime = micros();
  
  delay(20 - (endTime-startTime)/1000.0);
}

// Funcion para enviar datos a matlab, se podría pasar un array como parametro en vez de los datos por separado
void matlab_send(float dato1, float dato2, float dato3, float dato4, float dato5, float dato6){
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
  b = (byte *) &dato5;
  Serial.write(b,4);
  b = (byte *) &dato6;
  Serial.write(b,4);
}
