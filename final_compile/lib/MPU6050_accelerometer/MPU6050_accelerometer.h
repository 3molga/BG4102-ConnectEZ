#ifndef MPU6050_accelerometer_h
#define MPU6050_accelerometer_h

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
// #include <firebase.h>
// #include <Firebase_ESP_Client.h>

class MPU6050_accelerometer {
public:
    MPU6050_accelerometer();
    void begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL);
    void readSensorData();
    void printAccelerationData();
    void printGyroData();
    void printTemperature();
    void setAccelerometerRange(uint8_t range);
    void setGyroRange(uint16_t range);
    void setFilterBandwidth(uint8_t bandwidth);
    bool fallDetection();
    unsigned long getTime();
    float x_acce;
    float y_acce;
    float z_acce;
    float x_gyro;
    float y_gyro;
    float z_gyro;
    float W;
    float totalAcc;

private:
    Adafruit_MPU6050 mpu;
    sensors_event_t a, g, temp;
    TwoWire I2CMPU = TwoWire(0);
    Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;
    int timestamp;
    float upperAcceleration = 50.0;
    float upperAngularVelocity = 2.0;
    unsigned long sendDataPrevMillis = 0;
    unsigned long timerDelay = 180000;

};

#endif
