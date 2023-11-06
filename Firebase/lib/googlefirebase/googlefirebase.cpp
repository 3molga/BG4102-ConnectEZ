#include <googlefirebase.h>
#include <Arduino.h>
#include <addons/TokenHelper.h>    // Provide the token generation process info.
#include <addons/RTDBHelper.h>     // Provide the RTDB payload printing info and other helper functions.

googlefirebase::googlefirebase() {
}

void googlefirebase::initWiFi(String WIFI_SSID, String WIFI_PASSWORD) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println("");
}

void googlefirebase::setup(String API_KEY, String USER_EMAIL, String USER_PASSWORD, String DATABASE_URL) {
    configTime(0, 0, ntpServer);
    config.api_key = API_KEY;                                   // Assign the api key (required)
    auth.user.email = USER_EMAIL;                               // Assign the user sign in credentials
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;                         // Assign the RTDB URL (required)
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback;         //see addons/TokenHelper.h
    config.max_token_generation_retry = 10;                     // Assign the maximum retry of token generation
    Firebase.begin(&config, &auth);                             // Initialize the library with the Firebase authen and config
    Serial.println("Getting User UID");                         // Getting the user UID might take a few seconds
    while ((auth.token.uid) == "") {
        Serial.print('.');
        delay(1000);
    }
    uid = auth.token.uid.c_str();                               // Print user UID
    Serial.print("User UID: ");
    Serial.println(uid);
    databasePath = "/UsersData/" + uid + "/readings";           // Update database path
}

void googlefirebase::sendDataToDatabase(String timestamp, String x_acce, String y_acce, String z_acce, String x_gyro, String y_gyro, String z_gyro, String totalAcc, String W) {
    parentPath = databasePath + "/" + String(timestamp);
    json.set(xPath.c_str(), String(x_acce));
    json.set(yPath.c_str(), String(y_acce));
    json.set(zPath.c_str(), String(z_acce));
    json.set(xGyroPath.c_str(), String(x_gyro));
    json.set(yGyroPath.c_str(), String(y_gyro));
    json.set(zGyroPath.c_str(), String(z_gyro));
    json.set(fallMessage.c_str(), String(fallMessage));
    json.set(totalAccPath.c_str(), String(totalAcc));
    json.set(WPath.c_str(), String(W));
    json.set(timePath, String(timestamp));
    Serial.printf("Incident is recorded into Firebase %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "" : fbdo.errorReason().c_str());
    Serial.println("");
}

