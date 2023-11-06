#ifndef MPU6050_accelerometer_h
#define MPU6050_accelerometer_h

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 

class MPU6050_accelerometer {
public:
    MPU6050_accelerometer();
    void begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL);
    void readSensorData();
    String xAcceleration();
    String yAcceleration();
    String zAcceleration();
    void printAccelerationData();
    String xGyro();
    String yGyro();
    String zGyro();
    void printGyroData();
    void printTemperature();
    void setAccelerometerRange(uint8_t range);
    void setGyroRange(uint16_t range);
    void setFilterBandwidth(uint8_t bandwidth);
    bool fallDetection();
    String totalAcceleration();
    String totalAngularVelocity();
    unsigned long getTime();
    String whatTime();
    String timestamp_String;
    String x_acce;
    String y_acce;
    String z_acce;
    String x_gyro;
    String y_gyro;
    String z_gyro;
    float W;
    float totalAcc;
    String total_acce;
    String total_gyro;
    int timestamp;
    float upperAcceleration = 20.0;             // Threshold for fall detection
    float upperAngularVelocity = 3.5;           // Threshold for fall detection
    unsigned long sendDataPrevMillis = 0;
    unsigned long timerDelay = 180000;

private:
    Adafruit_MPU6050 mpu;
    sensors_event_t a, g, temp;
    TwoWire I2CMPU = TwoWire(0);
    Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;


};

#endif
