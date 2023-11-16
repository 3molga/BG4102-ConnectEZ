#include <MPU6050_accelerometer.h>
#include <Wire.h> 
#include <googlefirebase.h>

TwoWire I2CMPU = TwoWire(0);
googlefirebase firebaseInstance; // Create an instance of the firebase class

MPU6050_accelerometer::MPU6050_accelerometer() {
}

void MPU6050_accelerometer::begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL) {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);  // Will pause Zero, Leonardo, etc., until the serial console opens
    } 
    Serial.println("Adafruit MPU6050 test");

    I2CMPU.begin(MPU6050_accelerometer_SDA, MPU6050_accelerometer_SCL, 400000);
    bool status;
    status = mpu.begin(0x69, &I2CMPU); // 0x68 (low) or 0x69 (high)

    if (!status) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

    delay(100);
}

void MPU6050_accelerometer::readSensorData() {
    mpu.getEvent(&a, &g, &temp);
    x_acce = a.acceleration.x;
    y_acce = a.acceleration.y; 
    z_acce = a.acceleration.z;
    x_gyro = g.gyro.x;
    y_gyro = g.gyro.y;
    z_gyro = g.gyro.z;
}

void MPU6050_accelerometer::printAccelerationData() {
    Serial.print("Acceleration X: ");
    Serial.print(x_acce);
    Serial.print(", Y: ");
    Serial.print(y_acce);
    Serial.print(", Z: ");
    Serial.print(z_acce);
    Serial.println(" m/s^2");
}

void MPU6050_accelerometer::printGyroData() {
    Serial.print("Rotation X: ");
    Serial.print(x_gyro);
    Serial.print(", Y: ");
    Serial.print(y_gyro);
    Serial.print(", Z: ");
    Serial.print(z_gyro);
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

bool MPU6050_accelerometer::fallDetection(){
    readSensorData();
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    // x_acce = a.acceleration.x;
    // y_acce = a.acceleration.y; 
    // z_acce = a.acceleration.z;
    // x_gyro = g.gyro.x;
    // y_gyro = g.gyro.y;
    // z_gyro = g.gyro.z;
    
    // Calculate total acceleration vector magnitude
    float totalAcc = sqrt(x_acce*x_acce + y_acce*y_acce + z_acce*z_acce);

    // Check if total acceleration is greater than the upper threshold
    if (totalAcc > upperAcceleration) {
        Serial.println("Acceleration exceed: ");
        float W = sqrt(x_gyro*x_gyro + y_gyro*y_gyro + z_gyro*z_gyro);
        Serial.print(W);
        Serial.println("m/s^2");
    
        // Check if angular velocity is greater than the upper threshold
        if (W > upperAngularVelocity) {
            // Fall detected
            Serial.println("Fall detected!");
            printAccelerationData();
            printGyroData();
            Serial.println("");
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        Serial.println("No Fall");
        return 0;
    }
}

