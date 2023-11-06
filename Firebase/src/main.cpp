#include <Arduino.h>
#include <googlefirebase.h>
#include <MPU6050_accelerometer.h>
#include <MAX30102_heartratesensor.h>

#define WIFI_SSID "Zane’s iPhone"                           // Insert your network credentials
#define WIFI_PASSWORD "zane1234"                            // Insert your network password
#define API_KEY "AIzaSyC9IZqH7onhkDCxWYMoz4Hb_ZWK1eQJ7YM"   // Insert Firebase project API Key
#define USER_EMAIL "zaneyong00@gmail.com"// Insert Authorized Email and Corresponding Password
#define USER_PASSWORD "zane1234"
#define DATABASE_URL "https://connectez-87c05-default-rtdb.asia-southeast1.firebasedatabase.app/" // Insert RTDB URLefine the RTDB URL

// MPU6050_accelerometer pins
#define I2C_SDA_MPU 8 
#define I2C_SCL_MPU 9 

// MAX30102_heartratesensor pins
#define I2C_SDA_HBR 10
#define I2C_SCL_HBR 11

googlefirebase fb;
MPU6050_accelerometer mpu;
MAX30102_heartratesensor hbr;

void setup() {  
    mpu.begin(I2C_SDA_MPU, I2C_SCL_MPU);
    Serial.println("MPU6050 Accelerometer and Gyrometer Initialized");

    hbr.begin(I2C_SDA_HBR, I2C_SCL_HBR);
    Serial.println("MAX30102 Heart Rate Sensor Initialized");

    fb.initWiFi(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("Connected to %s Wifi", WIFI_SSID);
    Serial.println("");

    fb.setup(API_KEY, USER_EMAIL, USER_PASSWORD, DATABASE_URL);
    Serial.printf("Connected to Google Firebase at %s", DATABASE_URL);
    Serial.println("");
}

void loop() {
    // Fall detection
    mpu.readSensorData();
    if (mpu.fallDetection() == 1){
        fb.sendDataToDatabase(mpu.whatTime(), mpu.xAcceleration(), mpu.yAcceleration(), mpu.zAcceleration(), mpu.xGyro(), mpu.yGyro(), mpu.zGyro(), mpu.totalAcceleration(), mpu.totalAngularVelocity());
    }

    // Heart rate detection
    hbr.checkforHeartBeat();
    // delay(1000);
}
