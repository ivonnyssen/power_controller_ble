
#ifndef POWER_CONTROLLER_BLE_NTP_H
#define POWER_CONTROLLER_BLE_NTP_H
#include <Ethernet.h>
#include <mbed.h>

#define NTP_OFFSET   3155673600
#define UNIX_OFFSET   946684800
#define NTP_SYNC_INTERVAL 3600
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY 86400

class Ntp{
public:
    explicit Ntp(int timeZone);
    void begin();
    bool syncTime();
    time_t lastSyncTime;
    int timeZone;
private:
    constexpr static const auto ntpServer = "time.nist.gov";
    static const int ntpPacketSize = 48;
    uint8_t packetBuffer[ntpPacketSize]{0};
    static const unsigned int localPort = 8888;
    EthernetUDP Udp;

    time_t getTime();
    void sendPacket(const char * address);
};

#endif //POWER_CONTROLLER_BLE_NTP_H
