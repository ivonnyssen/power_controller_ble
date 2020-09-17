#ifndef POWER_CONTROLLER_BLE_MOCKNTP_H
#define POWER_CONTROLLER_BLE_MOCKNTP_H
#include <ctime>

class Ntp{
public:
    explicit Ntp(int timeZone);
    void begin();
    bool syncTime();
    time_t lastSyncTime;
    int timeZone;
};


#endif //POWER_CONTROLLER_BLE_MOCKNTP_H
