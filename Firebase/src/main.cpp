#include <Arduino.h>
#include <googlefirebase.h>
#include <MPU6050_accelerometer.h>

#define WIFI_SSID "Zane’s iPhone"                           // Insert your network credentials
#define WIFI_PASSWORD "zane1234"                            // Insert your network password
#define API_KEY "AIzaSyC9IZqH7onhkDCxWYMoz4Hb_ZWK1eQJ7YM"   // Insert Firebase project API Key
#define USER_EMAIL "zaneyong00@gmail.com"// Insert Authorized Email and Corresponding Password
#define USER_PASSWORD "zane1234"
#define DATABASE_URL "https://connectez-87c05-default-rtdb.asia-southeast1.firebasedatabase.app/" // Insert RTDB URLefine the RTDB URL

// MPU6050_accelerometer mpu;
#define I2C_SDA 8 
#define I2C_SCL 9 

googlefirebase fb;
MPU6050_accelerometer mpu;

void setup() {
    fb.initWiFi(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connected to %s Wifi", WIFI_SSID);
    fb.setup(API_KEY, USER_EMAIL, USER_PASSWORD, DATABASE_URL);
    Serial.printf("Connected to Google Firebase at %s", DATABASE_URL);
    mpu.begin(I2C_SDA, I2C_SCL);
    Serial.printf("Accelerometer Initialized");
}

void loop() {
    mpu.readSensorData();
    // Serial.println(mpu.totalAcceleration().c_str());
    // Serial.println(mpu.totalAngularVelocity().c_str());
    // Serial.println(mpu.whatTime().c_str());
    Serial.print("Acceleration on x: ");
    Serial.print(mpu.xAcceleration().c_str());
    Serial.println("m/s2");
    Serial.print("Acceleration on y: ");
    Serial.print(mpu.yAcceleration().c_str());
    Serial.println("m/s2");
    Serial.print("Acceleration on z: ");
    Serial.print(mpu.zAcceleration().c_str());
    Serial.println("m/s2");
    // Serial.println(mpu.xGyro().c_str());
    // Serial.println(mpu.yGyro().c_str());
    // Serial.println(mpu.zGyro().c_str());
    // fb.fallDetection();
    if (mpu.fallDetection() == 1){
        fb.sendDataToDatabase(mpu.whatTime(), mpu.xAcceleration(), mpu.yAcceleration(), mpu.zAcceleration(), mpu.xGyro(), mpu.yGyro(), mpu.zGyro(), mpu.totalAcceleration(), mpu.totalAngularVelocity());
    }
    delay(1000);
}
