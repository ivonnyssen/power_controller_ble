#ifndef POWER_CONTROLLER_BLE_CAMERA_H
#define POWER_CONTROLLER_BLE_CAMERA_H
#include <Arduino.h>
#include <ArduCAM.h>

class Camera {
public:
    void begin();
    void captureAndPrintImage(Stream *client);

private:
    static const size_t bufferSize = 1024;
    ArduCAM myCAM = ArduCAM(OV2640, 7);;
    time_t lastCaptureTime;
};
#endif //POWER_CONTROLLER_BLE_CAMERA_H
