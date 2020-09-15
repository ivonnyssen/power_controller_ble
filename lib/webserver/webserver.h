
#ifndef POWER_CONTROLLER_BLE_WEBSERVER_H
#define POWER_CONTROLLER_BLE_WEBSERVER_H
#include <Arduino.h>
#include <Ethernet.h>
#include <Relay.h>
#include <Bms.h>
#include <Sensors.h>
#include <Camera.h>

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
    void begin(Relay* aRelayPtr, Bms* aBmsPtr, Sensors* aSensorPtr, Camera* aCameraPtr);
    void handleHttpRequest();
private:
    byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    EthernetServer server = EthernetServer(LISTEN_PORT);
    Relay* relay{};
    Bms* bms{};
    Sensors* sensors{};
    Camera* camera{};
    static Request parseRequest(Stream *client);
    static void readAndLogRequestLines(Stream *client);
    void sendResponse(Stream *client, const String& url, int type);
    static void printIndexPage(Stream *client) ;
};

#endif //POWER_CONTROLLER_BLE_WEBSERVER_H
