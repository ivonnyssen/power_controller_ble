
#ifndef POWER_CONTROLLER_BLE_RELAY_H
#define POWER_CONTROLLER_BLE_RELAY_H

#include <Arduino.h>

#define PORT0_PIN 2
#define PORT1_PIN 3
#define PORT2_PIN 5
#define PORT3_PIN 6

#define OFF 0
#define ON 1
#define CYCLE 2

class Relay{
public:
    static const uint8_t numPorts = 4;
    void begin();
    static uint8_t getPinForPort(uint8_t port);
    void printJSON(Stream *client) const;
    void togglePort(uint8_t portNumber, bool on);
private:
    bool ports[numPorts] {false,false,false,false};
};

#endif //POWER_CONTROLLER_BLE_RELAY_H
