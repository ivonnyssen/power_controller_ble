#ifndef POWER_CONTROLLER_BLE_CAMERA_H
#define POWER_CONTROLLER_BLE_CAMERA_H

#if defined(ARDUINO)
#include <Arduino.h>
#include <ArduCAM.h>

class Camera {
public:
    explicit Camera(int csPin);
    void begin();
    void captureAndPrintImage(Stream *client);
    ~Camera();

private:
    static const size_t bufferSize = 1024;
    ArduCAM* myCAM;
    time_t lastCaptureTime;
};
#endif // ARDUINO

#endif //POWER_CONTROLLER_BLE_CAMERA_H
