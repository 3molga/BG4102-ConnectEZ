#include <MPU6050_accelerometer.h>
#include <Wire.h>
#include <string>

TwoWire I2CMPU = TwoWire(0);

MPU6050_accelerometer::MPU6050_accelerometer() {
}

void MPU6050_accelerometer::begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL) {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);  // Will pause Zero, Leonardo, etc., until the serial console opens
    } 
    Serial.println("MPU6050 Accelerometer and Gyrometer test");

    I2CMPU.begin(MPU6050_accelerometer_SDA, MPU6050_accelerometer_SCL, 400000);
    bool status;
    status = mpu.begin(0x69, &I2CMPU); // 0x68 (low) or 0x69 (high)

    if (!status) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    // Serial.println("MPU6050-Accelerometer and Gyrometer Found");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

    delay(100);
}

void MPU6050_accelerometer::readSensorData() {
    mpu.getEvent(&a, &g, &temp);
}

String MPU6050_accelerometer::xAcceleration(){
    String x_acce = String(a.acceleration.x);
    return x_acce;
}

String MPU6050_accelerometer::yAcceleration(){
    String y_acce = String(a.acceleration.y);
    return y_acce;
}

String MPU6050_accelerometer::zAcceleration(){
    String z_acce = String(a.acceleration.z);
    return z_acce;
}

void MPU6050_accelerometer::printAccelerationData() {
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");
}

String MPU6050_accelerometer::xGyro(){
    String x_gyro = String(g.gyro.x);
    return x_gyro;
}

String MPU6050_accelerometer::yGyro(){
    String y_gyro = String(g.gyro.y);
    return y_gyro;
}

String MPU6050_accelerometer::zGyro(){
    String z_gyro = String(g.gyro.z);
    return z_gyro;
}

void MPU6050_accelerometer::printGyroData() {
    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");
}

void MPU6050_accelerometer::printTemperature() {
    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");
}

unsigned long MPU6050_accelerometer::getTime() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
    }
    time(&now);
    return now;
}

String MPU6050_accelerometer::whatTime(){
    String timestamp_String = String(getTime());
    return timestamp_String;
}

bool MPU6050_accelerometer::fallDetection(){
    // readSensorData();
    sendDataPrevMillis = millis();
    
    // Calculate total acceleration vector magnitude
    float totalAcc = sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z);

    // Check if total acceleration is greater than the upper threshold
    if (totalAcc > upperAcceleration) {
        timestamp = getTime();
        Serial.print ("time: ");
        Serial.println (timestamp);
        Serial.print("Acceleration exceed: ");
        Serial.print(totalAcc);
        Serial.println("m/s^2");
        float W = sqrt(g.gyro.x*g.gyro.x + g.gyro.y*g.gyro.y + g.gyro.z*g.gyro.z);

        // Check if angular velocity is greater than the upper threshold
        if (W > upperAngularVelocity) {
            // Fall detected
            Serial.println("Fall detected!");
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        // Serial.println("No Fall");
        return 0;
    }
}

String MPU6050_accelerometer::totalAcceleration(){
    String total_acce = String(sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z)) + "m/s2";
    return total_acce;
}

String MPU6050_accelerometer::totalAngularVelocity(){
    String total_gyro = String(sqrt(g.gyro.x*g.gyro.x + g.gyro.y*g.gyro.y + g.gyro.z*g.gyro.z)) + "rad/s";
    return total_gyro;
}

