#ifndef POWER_CONTROLLER_BLE_MOCKETHERNET_H
#define POWER_CONTROLLER_BLE_MOCKETHERNET_H

#include <MockArduino.h>

class EthernetClient : public Stream{
public:
    void stop(){}
};

class EthernetServer {
public:
    explicit EthernetServer(int listenPort);
    void begin();
    static EthernetClient available();
};

class IPAddress : public Printable{
public:
    uint8_t operator[](int index) const;
private:
    uint8_t _address[4] = {192,168,2,14};
};

class EthernetClass{
public:
    static int begin(uint8_t *mac, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
    static IPAddress localIP();
};

#endif //POWER_CONTROLLER_BLE_MOCKETHERNET_H
