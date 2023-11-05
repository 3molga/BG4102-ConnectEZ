#include <googlefirebase.h>
#include <Arduino.h>
#include <addons/TokenHelper.h>    // Provide the token generation process info.
#include <addons/RTDBHelper.h>     // Provide the RTDB payload printing info and other helper functions.
// #include <MPU6050_accelerometer.h>
// #define WIFI_SSID "Zane’s iPhone"                           // Insert your network credentials
// #define WIFI_PASSWORD "zane1234"                            // Insert your network password
// #define API_KEY "AIzaSyC9IZqH7onhkDCxWYMoz4Hb_ZWK1eQJ7YM"   // Insert Firebase project API Key
// #define USER_EMAIL "zaneyong00@gmail.com"// Insert Authorized Email and Corresponding Password
// #define USER_PASSWORD "zane1234"
// #define DATABASE_URL "https://connectez-87c05-default-rtdb.asia-southeast1.firebasedatabase.app/" // Insert RTDB URLefine the RTDB URL

// MPU6050_accelerometer mpuInstance;
TwoWire I2CMPU = TwoWire(0);

googlefirebase::googlefirebase() {
}

void googlefirebase::initWiFi(String WIFI_SSID, String WIFI_PASSWORD) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    // Serial.println(WiFi.localIP());
    // Serial.println();
}

void googlefirebase::setup(String API_KEY, String USER_EMAIL, String USER_PASSWORD, String DATABASE_URL) {
    configTime(0, 0, ntpServer);
    config.api_key = API_KEY;           // Assign the api key (required)
    auth.user.email = USER_EMAIL;       // Assign the user sign in credentials
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL; // Assign the RTDB URL (required)
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    config.max_token_generation_retry = 10;// Assign the maximum retry of token generation
    Firebase.begin(&config, &auth);// Initialize the library with the Firebase authen and config
    Serial.println("Getting User UID");// Getting the user UID might take a few seconds
    while ((auth.token.uid) == "") {
        Serial.print('.');
        delay(1000);
    }
    uid = auth.token.uid.c_str();// Print user UID
    Serial.print("User UID: ");
    Serial.println(uid);
    databasePath = "/UsersData/" + uid + "/readings";// Update database path
}

void googlefirebase::begin(int MPU6050_accelerometer_SDA, int MPU6050_accelerometer_SCL) {
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

void googlefirebase::readSensorData() {
    mpu.getEvent(&a, &g, &temp);
    x_acce = a.acceleration.x;
    y_acce = a.acceleration.y; 
    z_acce = a.acceleration.z;
    x_gyro = g.gyro.x;
    y_gyro = g.gyro.y;
    z_gyro = g.gyro.z;
}

void googlefirebase::printAccelerationData() {
    Serial.print("Acceleration X: ");
    Serial.print(x_acce);
    Serial.print(", Y: ");
    Serial.print(y_acce);
    Serial.print(", Z: ");
    Serial.print(z_acce);
    Serial.println(" m/s^2");
}

void googlefirebase::printGyroData() {
    Serial.print("Rotation X: ");
    Serial.print(x_gyro);
    Serial.print(", Y: ");
    Serial.print(y_gyro);
    Serial.print(", Z: ");
    Serial.print(z_gyro);
    Serial.println(" rad/s");
}

void googlefirebase::printTemperature() {
    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");
}

unsigned long googlefirebase::getTime() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
    }
    time(&now);
    return now;
}

void googlefirebase::fallDetection(){
    readSensorData();
    sendDataPrevMillis = millis();

    //Get current timestamp
    timestamp = getTime();
    
    // Calculate total acceleration vector magnitude
    float totalAcc = sqrt(x_acce*x_acce + y_acce*y_acce + z_acce*z_acce);

    // Check if total acceleration is greater than the upper threshold
    if (totalAcc > upperAcceleration) {
        Serial.println("");
        Serial.print ("time: ");
        Serial.println (timestamp);
        Serial.print("Acceleration exceed: ");
        float W = sqrt(x_gyro*x_gyro + y_gyro*y_gyro + z_gyro*z_gyro);
        Serial.print(W);
        Serial.println("m/s^2");
    
        // Check if angular velocity is greater than the upper threshold
        if (W > upperAngularVelocity) {
            // Fall detected
            Serial.println("Fall detected!");
            printAccelerationData();
            printGyroData();
            parentPath = databasePath + "/" + String(timestamp);
            json.set(xPath.c_str(), String(x_acce));
            json.set(yPath.c_str(), String(y_acce));
            json.set(zPath.c_str(), String(z_acce));
            json.set(xGyroPath.c_str(), String(x_gyro));
            json.set(yGyroPath.c_str(), String(y_gyro));
            json.set(zGyroPath.c_str(), String(z_gyro));
            json.set(fallMessage.c_str(), String(fallMessage));
            json.set(timePath, String(timestamp));
            Serial.printf("Incident is recorded into Firebase %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "" : fbdo.errorReason().c_str());
            Serial.println("");
        }
    }
}



// void googlefirebase::sendDataToDatabase(int timestamp, float x_acce, float y_acce, float z_acce, float x_gyro, float y_gyro, float z_gyro) {
//     parentPath = databasePath + "/" + String(timestamp);
//     json.set(xPath.c_str(), String(x_acce));
//     json.set(yPath.c_str(), String(y_acce));
//     json.set(zPath.c_str(), String(z_acce));
//     json.set(xGyroPath.c_str(), String(x_gyro));
//     json.set(yGyroPath.c_str(), String(y_gyro));
//     json.set(zGyroPath.c_str(), String(z_gyro));
//     json.set(fallMessage.c_str(), String(fallMessage));
//     json.set(timePath, String(timestamp));
//     Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
// }

