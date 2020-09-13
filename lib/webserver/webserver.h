
#ifndef POWER_CONTROLLER_BLE_WEBSERVER_H
#define POWER_CONTROLLER_BLE_WEBSERVER_H
#include <Arduino.h>
#include <Ethernet.h>
#include <Relay.h>
#include <Bms.h>
#include <Sensors.h>

#define GET 0
#define POST 1
#define UNSUPPORTED 2
#define LISTEN_PORT 80
#define DEBUG_WEB_SERVER true

typedef struct Request{
    int type;
    String url;
    long powerPort;
    long command;
} Request;

class WebServer {
public:
    void begin(Relay& aRelayRef, Bms& aBmsRef, Sensors& aSensorRef);
    void handleHttpRequest();
private:
    byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    EthernetServer server = EthernetServer(LISTEN_PORT);
    Relay relay;
    Bms bms;
    Sensors sensors;
    static Request parseRequest(Client &client);
    static void readAndLogRequestLines(Client &client);
    void sendResponse(Client &client, const String &url, int type);
    static void printIndexPage(Client &client) ;
};

#endif //POWER_CONTROLLER_BLE_WEBSERVER_H
