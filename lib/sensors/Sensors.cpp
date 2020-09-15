#include "Sensors.h"
#include <SD.h>
void Sensors::measureAndLog() {
    for(int i = 0; i < numSensorRecords - 1; i++){
        sensorData[i] = sensorData[i + 1];
    }
    bme.takeForcedMeasurement();
    lastLogTime = time(nullptr);
    sensorData[numSensorRecords - 1] = {lastLogTime, bme.readPressure() / 100.0F, bme.readTemperature(), bme.readHumidity()};

    char buffer[64] = {0};
    struct tm* logTime = localtime(&sensorData[numSensorRecords - 1].readoutTime);
    sprintf(buffer, "%02d-%02d %02d:%02d, %.2f, %.2f, %.2f", logTime->tm_mon + 1, logTime->tm_mday, logTime->tm_hour, logTime->tm_min,
            sensorData[numSensorRecords - 1].pressure, sensorData[numSensorRecords - 1].temperature, sensorData[numSensorRecords - 1].humidity);
    char filename[12] = {0};
    int result = snprintf(filename, sizeof(filename), "%04d%02d.log", logTime->tm_year + 1900, logTime->tm_mon + 1);
    if(result < 0) {
        if(Serial) Serial.println("buffer overflow in filename");
    } else{
        File logFile = SD.open(filename, FILE_WRITE);
        logFile.println(buffer);
        logFile.close();
    }
#if DEBUG_SENSORS
    if(Serial) Serial.println(buffer);
#endif
}

void Sensors::printJson(Stream *client) {
    client->println(R"===({ "values":[)===");
    for(int i = 0; i < numSensorRecords; i++){
        char buffer[128] = {0};
        struct tm* logTime = localtime(&sensorData[i].readoutTime);
        sprintf(buffer, R"===({"time":"%02d-%02d %02d:%02d", "pressure":%.2f, "temp":%.2f, "humidity":%.2f})===",
                logTime->tm_mon + 1, logTime->tm_mday, logTime->tm_hour, logTime->tm_min, sensorData[i].pressure,
                sensorData[i].temperature, sensorData[i].humidity);
        client->print(buffer);
        if(i != numSensorRecords - 1) {
            client->println(",");
        } else {
            client->println();
        }
    }
    client->println("]}");
}

bool Sensors::begin() {
    if(!bme.begin()){
        if(Serial) Serial.println("BME280 failed.");
        return false;
    } else {
        bme.setSampling(Adafruit_BME280::MODE_FORCED,
                        Adafruit_BME280::SAMPLING_X1, // temperature
                        Adafruit_BME280::SAMPLING_X1, // pressure
                        Adafruit_BME280::SAMPLING_X1, // humidity
                        Adafruit_BME280::FILTER_OFF   );
    }
    return true;
}

Sensors::Sensors() {
    lastLogTime = 0;
}
