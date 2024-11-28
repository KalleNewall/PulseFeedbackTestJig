
#include "QwiicScale.hpp"

//BASED ON EXAMPLE 2 FROM SPARKFUN QWIICSCALE LIBRARY

#define EEPROM_SIZE 100 
#define LOCATION_CALIBRATION_FACTOR 0 
#define LOCATION_ZERO_OFFSET 10

QwiicScale::QwiicScale() : avgWeightSpot(0), settingsDetected(false) {}

float found_calibration_factor = 586.97;
int32_t found_setting_zero_offset = -8381420;


void QwiicScale::begin() {
    EEPROM.begin(EEPROM_SIZE);
    Wire.begin();
    Wire.setClock(400000);

    if (myScale.begin() == false) {
        Serial.println("Scale not detected. Please check wiring.");
        while (1);
    }
    //Serial.println("Scale detected!");

    readSystemSettings();
    myScale.setSampleRate(NAU7802_SPS_320);
    myScale.calibrateAFE();

    // Serial.print("Zero offset: ");
    // Serial.println(myScale.getZeroOffset());
    // Serial.print("Calibration factor: ");
    // Serial.println(myScale.getCalibrationFactor());
}

void QwiicScale::calibrateScale() {
    Serial.println(F("Scale calibration"));
    Serial.println(F("Setup scale with no weight on it. Press a key when ready."));
    while (Serial.available() == 0) delay(10);

    myScale.calculateZeroOffset(64);
    Serial.print(F("New zero offset: "));
    Serial.println(myScale.getZeroOffset());

    Serial.println(F("Place known weight on scale. Press a key when weight is in place and stable."));
    while (Serial.available() == 0) delay(10);

    Serial.print(F("Please enter the weight, without units, currently sitting on the scale: "));
    while (Serial.available() == 0) delay(10);

    float weightOnScale = Serial.parseFloat();
    myScale.calculateCalibrationFactor(weightOnScale, 64);
    Serial.print(F("New cal factor: "));
    Serial.println(myScale.getCalibrationFactor(), 2);
    Serial.println(myScale.getWeight(), 2);

    recordSystemSettings();
    settingsDetected = true;
}

void QwiicScale::tare() {
    myScale.calculateZeroOffset();
}

void QwiicScale::readSystemSettings() {
    float settingCalibrationFactor;
    int32_t settingZeroOffset;

    EEPROM.get(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
    if (settingCalibrationFactor == 0xFFFFFFFF) {
        settingCalibrationFactor = found_calibration_factor;
        EEPROM.put(LOCATION_CALIBRATION_FACTOR, settingCalibrationFactor);
    }

    EEPROM.get(LOCATION_ZERO_OFFSET, settingZeroOffset);
    if (settingZeroOffset == 0xFFFFFFFF) {
        settingZeroOffset = found_setting_zero_offset;
        EEPROM.put(LOCATION_ZERO_OFFSET, settingZeroOffset);
    }

    myScale.setCalibrationFactor(found_calibration_factor);
    myScale.setZeroOffset(found_setting_zero_offset);

    settingsDetected = (settingCalibrationFactor != 1.0 && settingZeroOffset != 0);
}

void QwiicScale::recordSystemSettings() {
    EEPROM.put(LOCATION_CALIBRATION_FACTOR, myScale.getCalibrationFactor());
    EEPROM.put(LOCATION_ZERO_OFFSET, myScale.getZeroOffset());
    EEPROM.commit();
}

float QwiicScale::getAverageWeight() {
    float avgWeight = 0;
    for (int x = 0; x < AVG_SIZE; x++) {
        avgWeight += avgWeights[x];
    }
    return avgWeight / AVG_SIZE;
}

void QwiicScale::update() {
    if (myScale.available() == true) {
        float currentWeight = myScale.getWeight();
        avgWeights[avgWeightSpot++] = currentWeight;
        if (avgWeightSpot == AVG_SIZE) avgWeightSpot = 0;
    }
}
