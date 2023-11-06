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
    void sendDataToDatabase(String timestamp, String x_acce, String y_acce, String z_acce, String x_gyro, String y_gyro, String z_gyro, String totalAcc, String W);
    String x_acce;
    String y_acce;
    String z_acce;
    String x_gyro;
    String y_gyro;
    String z_gyro;
    String W;
    String totalAcc;
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
    String totalAccPath = "/total_acceleration";
    String WPath = "/total_angular_velocity";
    String fallMessage = "Possible fall detected";
    FirebaseJson json;
    const char* ntpServer = "pool.ntp.org";

private:

};

#endif
