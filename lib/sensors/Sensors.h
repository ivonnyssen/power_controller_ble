
#ifndef POWER_CONTROLLER_BLE_SENSORS_H
#define POWER_CONTROLLER_BLE_SENSORS_H

#include <ctime>
#if defined(ARDUINO)
#include <Arduino.h>
#include <Adafruit_BME280.h>
#else
#include <MockArduino.h>
#include <mocks/MockBME280.h>
#endif

#define DEBUG_SENSORS true

typedef struct SensorData{
    time_t readoutTime;
    float pressure;
    float temperature;
    float humidity;
} SensorData;

class Sensors {
public:
    Sensors();
    bool begin();
    void measureAndLog();
    void printJson(Stream *client);
    time_t lastLogTime;
private:
    Adafruit_BME280 bme;
    static const int numSensorRecords = 24 * 4;
    SensorData sensorData[numSensorRecords]{};
};

#endif //POWER_CONTROLLER_BLE_SENSORS_H
