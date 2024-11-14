
// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/


//esp 32 SCL -22 SDA -21

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int BUTTON_PIN = 5;
const int INTPin = 14;

int startTime = 0;
int sampleTime = 100;
int sampleCount = 0;

float calibrateAksX = 0;
float calibrateAksY = 0;
float calibrateAksZ = 0;
float calibrateRotX = 0;
float calibrateRotY = 0;
float calibrateRotZ = 0;

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(INTPin, INPUT);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

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

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
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


float* calibrate(int sampels) {
  Serial.println("Calibrating with: " + String(sampels) + "sampels");
  const int sensorAmount = 6;
  const int sampleDelay = 500;
  static float calibrationNumbs[sensorAmount];
  float sumXAks = 0;
  float sumYAks = 0;
  float sumZAks = 0;
  float sumXRot = 0;
  float sumYRot = 0;
  float sumZRot = 0;
  for (int i = 0; i < sampels; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    sumXAks += a.acceleration.x;
    sumYAks += a.acceleration.y;
    sumZAks += a.acceleration.z;

    sumXRot += g.gyro.x;
    sumYRot += g.gyro.y;
    sumZRot += g.gyro.z;
    Serial.println(String(i + 1) + "/" + String(sampels));
    delay(sampleDelay);
  }
  calibrationNumbs[0] = sumXAks / sampels;
  calibrationNumbs[1] = sumYAks / sampels;
  calibrationNumbs[2] = sumZAks / sampels;

  calibrationNumbs[3] = sumXRot / sampels;
  calibrationNumbs[4] = sumYRot / sampels;
  calibrationNumbs[5] = sumZRot / sampels;

  return calibrationNumbs;
}

void loop() {
  int INTPinVal = digitalRead(INTPin);

  float xAks = 0;
  float yAks = 0;
  float zAks = 0;
  float xRot = 0;
  float yRot = 0;
  float zRot = 0;

  int calibrationButton = digitalRead(BUTTON_PIN);
  // Serial.println(calibrationButton);
  // Serial.println(INTPinVal);

  if (calibrationButton == 0) {
    float* calibrationVals = calibrate(10);
    calibrateAksX = calibrationVals[0];
    calibrateAksY = calibrationVals[1];
    calibrateAksZ = calibrationVals[2] - 9.81;

    calibrateRotX = calibrationVals[3];
    calibrateRotY = calibrationVals[4];
    calibrateRotZ = calibrationVals[5];

    Serial.println("------------------");
    Serial.println(String(calibrateAksX) + "/" + String(calibrationVals[0]));
    Serial.println(String(calibrateAksY) + "/" + String(calibrationVals[1]));
    Serial.println(String(calibrateAksZ) + "/" + String(calibrationVals[2]));
    Serial.println(String(calibrateRotX) + "/" + String(calibrationVals[3]));
    Serial.println(String(calibrateRotY) + "/" + String(calibrationVals[4]));
    Serial.println(String(calibrateRotZ) + "/" + String(calibrationVals[5]));
    Serial.println("------------------");
  }

  startTime = millis();
  sampleCount = 0;
  while ((millis()-startTime) < sampleTime) {
    while (INTPinVal != 0) {
      INTPinVal = digitalRead(INTPin);
    }

    // save the last state

    // Serial.println(( millis()));
    // Serial.println(( startTime));
    // Serial.println(( millis()-startTime));
    //   Serial.println(sampleCount);
    //     Serial.println("-------------");


      /* Get new sensor events with the readings */
      sensors_event_t a,
      g, temp;
    mpu.getEvent(&a, &g, &temp);
    xAks += a.acceleration.x - calibrateAksX;
    yAks += a.acceleration.y - calibrateAksY;
    zAks += a.acceleration.z - calibrateAksZ;

    xRot += g.gyro.x - calibrateRotX;
    yRot += g.gyro.y - calibrateRotY;
    zRot += g.gyro.z - calibrateRotZ;

    /* Print out the values */

    sampleCount++;
  }

  xAks /=sampleCount;
  yAks /=sampleCount;
  zAks /=sampleCount;

  xRot /=sampleCount;
  yRot /=sampleCount;
  zRot /=sampleCount;





  // Serial.print("Acceleration X: ");
  // Serial.print(xAks);
  // Serial.print(", Y: ");
  // Serial.print(yAks);
  // Serial.print(", Z: ");
  // Serial.print(zAks);
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(xRot);
  // Serial.print(", Y: ");
  // Serial.print(yRot);
  // Serial.print(", Z: ");
  // Serial.print(zRot);
  // Serial.print(" rad/s");
  // Serial.print(", sampels: ");
  // Serial.print(sampleCount);
  // Serial.println(" pr/måling");
  Serial.print(xAks);
  Serial.print(",");
  Serial.print(yAks);
  Serial.print(",");
  Serial.print(zAks);
  Serial.print(",");
  Serial.print(xRot);
  Serial.print(",");
  Serial.print(yRot);
  Serial.print(",");
  Serial.print(zRot);
  Serial.print(",");
  Serial.print(sampleCount);
  Serial.print(",");
  Serial.print((millis()-startTime));
  Serial.println("");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  // Serial.println(" degC");

  // Serial.println("");
  // delay(100);
}
