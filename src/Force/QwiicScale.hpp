
#pragma once
#include <Wire.h>
#include <EEPROM.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"

class QwiicScale {
public:
    QwiicScale();
    void begin();
    void calibrateScale();
    void tare();
    void readSystemSettings();
    void recordSystemSettings();
    float getAverageWeight();
    void update();

private:
    NAU7802 myScale;
    bool settingsDetected;
    static const int AVG_SIZE = 1;
    float avgWeights[AVG_SIZE];
    byte avgWeightSpot;
    void initializeScale();
};
