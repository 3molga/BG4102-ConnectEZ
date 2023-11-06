#include <MAX30102_heartratesensor.h>
#include <Wire.h>
#include <string>

TwoWire I2CMAX = TwoWire(1);

MAX30102_heartratesensor::MAX30102_heartratesensor() {
}

void MAX30102_heartratesensor::begin(int MAX30102_heartratesensor_SDA, int MAX30102_heartratesensor_SCL) {
    Serial.begin(115200);
    Serial.println("MAX30102 Heart Rate Sensor test");
    I2CMAX.begin(MAX30102_heartratesensor_SDA, MAX30102_heartratesensor_SCL);
    bool status;
    status = hbr.begin(I2CMAX, I2C_SPEED_FAST);
    if (!status) {
        Serial.println("Failed to find MAX30102 Heart Rate Sensor chip.");
        while (1) {
            delay(10);
        }
    }
    // Serial.println("MAX30102 Heart Rate Sensor found!");
    hbr.setup();
    hbr.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    hbr.setPulseAmplitudeGreen(0);
}

void MAX30102_heartratesensor::checkforHeartBeat(){
    long irValue = hbr.getIR();

    if (checkForBeat(irValue) == true) {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute;
            rateSpot %= RATE_SIZE;
            beatAvg = 0;
            for (byte x = 0 ; x < RATE_SIZE ; x++){
                beatAvg += rates[x];
            } 
            beatAvg /= RATE_SIZE;
        }
    }

    if (irValue > 50000){
        Serial.print("IR=");
        Serial.print(irValue);
        Serial.print(", BPM=");
        Serial.print(beatsPerMinute);
        Serial.print(", Avg BPM=");
        Serial.println(beatAvg);
    }
}
