#ifndef MAX30102_heartratesensor_h
#define MAX30102_heartratesensor_h

#include <Arduino.h>
#include <MAX30105.h>
#include <heartRate.h>
#include <Wire.h>

class MAX30102_heartratesensor {
public:
    MAX30102_heartratesensor();
    void begin(int MAX30102_heartratesensor_SDA, int MAX30102_heartratesensor_SCL);
    void checkforHeartBeat();

private:
    MAX30105 hbr;
    float beatsPerMinute;
    static const byte RATE_SIZE = 50;
    byte rates[RATE_SIZE];
    byte rateSpot = 0;
    long lastBeat = 0;
    int beatAvg;
    long irValue;
};

#endif
