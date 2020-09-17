//
// Created by Igor von Nyssen on 9/16/20.
//

#ifndef POWER_CONTROLLER_BLE_MOCKCAMERA_H
#define POWER_CONTROLLER_BLE_MOCKCAMERA_H
#include <MockArduino.h>

class Camera {
public:
    explicit Camera(int csPin) {};
    void begin() {};
    void captureAndPrintImage(Stream *client) {}
    ~Camera();
};

#endif //POWER_CONTROLLER_BLE_MOCKCAMERA_H
