#include <Servo.h>
#include <BMI160Gen.h>
#include "Kalman.h"

//Servo motorFL;
//Servo motorFR;
//Servo motorBL;
//Servo motorBR;

Kalman kalmanX;
Kalman kalmanY;

int gxRaw, gyRaw, gzRaw; 
int axRaw, ayRaw, azRaw; 

double accX, accY, accZ;
double gyroX, gyroY, gyroZ;

double gyroXangle, gyroYangle;
double compAngleX, compAngleY; //complementary filter
double kalAngleX, kalAngleY; //kalman filter

uint32_t timer;

//TODO: make calibration routine

void setup() {
 Serial.begin(9600);
 while(!Serial);

 Serial.println("Initializing IMU device...");
 if (!BMI160.begin(BMI160GenClass::SPI_MODE, /*SS pin# = */2))
 {
  Serial.println("failed");
  return;
 }
 
 uint8_t dev_id = BMI160.getDeviceID();
 Serial.print("DEVICE ID: ");
 Serial.println(dev_id, HEX);

 //Set the accelerometer range to 250 degrees/second
 BMI160.setGyroRange(250);

 BMI160.readAccelerometer(axRaw, ayRaw, azRaw);
 accX = convertRawGyro(axRaw);
 accY = convertRawGyro(ayRaw);
 accZ = convertRawGyro(azRaw);

 double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
 double pitch = atan2(-accX, accZ) * RAD_TO_DEG;

 kalmanX.setAngle(roll);
 kalmanY.setAngle(pitch);

 gyroXangle = roll;
 gyroYangle = pitch;
 compAngleX = roll;
 compAngleY = pitch;
 
 Serial.println("Initializing IMU device...done.");

 //motorFL.attach(8);
 //motorFR.attach(9);
 //motorHL.attach(10);
 //motorHR.attach(11);

 timer = micros();
}


void loop() {
 //read raw gyro measurements from device
 BMI160.readGyro(gxRaw, gyRaw, gzRaw);
 BMI160.readAccelerometer(axRaw, ayRaw, azRaw);

 //convert the raw gyro data to degrees/second
 gyroX = convertRawGyro(gxRaw);
 gyroY = convertRawGyro(gyRaw);
 gyroZ = convertRawGyro(gzRaw);

 accX = convertRawGyro(axRaw);
 accY = convertRawGyro(ayRaw);
 accZ = convertRawGyro(azRaw);

 double dt = (double)(micros() -timer) / 1000000; //delta time
 timer = micros();

 double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
 double pitch = atan2(-accX, accZ) * RAD_TO_DEG;

//WTF?
 double gyroXrate = gyroX /131.0; //convert to deg/s
 double gyroYrate = gyroY /131.0;

 //this fixes transition problem from -180 to 180
 if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90))
 {
  kalmanY.setAngle(pitch);
  compAngleY = pitch;
  kalAngleY = pitch;
  gyroYangle = pitch;
 }
 else
 {
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
 }

 if (abs(kalAngleY) > 90)
 {
  gyroXrate = -gyroXrate;
 }

 kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);

 gyroXangle += gyroXrate * dt;
 gyroYangle += gyroYrate * dt;

 compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll;
 compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

 //reset gyro angle when it has drifted too much
 if (gyroXangle < -180 || gyroXangle > 180)
  gyroXangle = kalAngleX;
 if (gyroYangle < -180 || gyroYangle > 180)
  gyroYangle = kalAngleY;


  //print data
  Serial.println(roll);
  Serial.println(gyroXangle); 
  Serial.println(compAngleX);
  Serial.println(kalAngleX);

  Serial.println();
  Serial.println(pitch);
  Serial.println(gyroYangle);
  Serial.println(compAngleY);
  Serial.println(kalAngleY);

  delay(2);
}

float convertRawGyro(int gRaw)
{
  //since we are using 250 degrees/second range
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

