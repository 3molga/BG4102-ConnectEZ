#ifndef googlefirebase_h
#define googlefirebase_h

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 


class googlefirebase {
public:
    googlefirebase();
    void initWiFi(String WIFI_SSID, String WIFI_PASSWORD);
    void setup(String API_KEY, String USER_EMAIL, String USER_PASSWORD, String DATABASE_URL);
    void sendDataToDatabase(int timestamp, float x_acce, float y_acce, float z_acce, float x_gyro, float y_gyro, float z_gyro, float totalAcc, float W);
    void begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL);
    void readSensorData();
    void printAccelerationData();
    void printGyroData();
    void printTemperature();
    void setAccelerometerRange(uint8_t range);
    void setGyroRange(uint16_t range);
    void setFilterBandwidth(uint8_t bandwidth);
    unsigned long getTime();
    void fallDetection();
    float x_acce;
    float y_acce;
    float z_acce;
    float x_gyro;
    float y_gyro;
    float z_gyro;
    float W;
    float totalAcc;
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    String uid;
    String databasePath;
    String parentPath;
    String xPath = "/x_axis";
    String yPath = "/y_axis";
    String zPath = "/z_axis";
    String xGyroPath = "/x_gyro";
    String yGyroPath = "/y_gyro";
    String zGyroPath = "/z_gyro";
    String timePath = "/timestamp";
    String fallMessage = "Possible fall detected";
    FirebaseJson json;
    const char* ntpServer = "pool.ntp.org";

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
