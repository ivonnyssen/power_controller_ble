#include "webserver.h"
#include "indexpage.h"

#include <cstring>
#include <cctype>
#if defined(ARDUINO)
#include <SD.h>
#else
#include <MockSD.h>
#endif

void WebServer::handleHttpRequest() {
    EthernetClient client = server.available();
    if (client.available()) {
        Request request = parseRequest(&client);
        switch (request.type) {
            case Request::GET:
                sendResponse(&client, request.url, Request::GET);
                break;
            case Request::POST:
                switch(request.command){
                    case OFF:
                        relay->togglePort(request.powerPort, false);
                        break;
                    case ON:
                        relay->togglePort(request.powerPort, true);
                        break;
                    case CYCLE:
                        relay->togglePort(request.powerPort, false);
                        delay(1000);
                        relay->togglePort(request.powerPort, true);
                        break;
                    default:
                        break;
                }
                sendResponse(&client, "/", Request::POST);
            default:
                break;
        }
    }
    delay(10);
    client.stop();
}

Request WebServer::parseRequest(Stream *client) {
    Request result;

    char s[265] {0};
    client->readBytesUntil('\n', &s[0], sizeof(s));
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(s);
#endif
    if(strncmp(&s[0],"GET", sizeof("GET")) == 0){
        result.type = Request::GET;
        strncpy(result.url, &s[4], s - strrchr(s, ' ') + 1);
        readAndLogRequestLines(client);
    } else if(strncmp(s,"POST", sizeof("POST")) == 0){
        result.type = Request::POST;
        readAndLogRequestLines(client);
        if(client->available()){
            client->readBytesUntil('\n', &s[0], sizeof(s));
            if(strncmp(s, "power", sizeof("power")) == 0){
                if(isdigit(s[5])){
                    result.powerPort = s[5] - '0';
                }
                if(isdigit(s[7])) {
                    result.command = s[7] - '0';
                }
            }
        }
    } else {
        result.type = Request::UNSUPPORTED;
    }
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(result.url);
#endif
    return result;
}

void WebServer::sendResponse(Stream *client, const char* url, int type) {
    //print header
    if (type == Request::POST) {
        client->println("HTTP/1.1 303 See Other");
        char buffer[64] = {0};
        sprintf(buffer, "Location: http://%d.%d.%d.%d%s",EthernetClass::localIP()[0],EthernetClass::localIP()[1],
                EthernetClass::localIP()[2],EthernetClass::localIP()[3],url);
        client->println(buffer);
    } else {
        client->println("HTTP/1.1 200 OK");
        if(strncmp(url, "/", sizeof("/")) == 0){
            char buffer[64] = {0};
            sprintf(buffer, "Refresh: 450; url=http://%d.%d.%d.%d%s",EthernetClass::localIP()[0],
                    EthernetClass::localIP()[1],EthernetClass::localIP()[2],EthernetClass::localIP()[3],url);
            client->println(buffer);
        }
    }
    const char* endOfUrl = strrchr(url, 0);
    if(strncmp(endOfUrl - sizeof(".html"),".html", sizeof(".html")) == 0){
        client->println("Content-Type: text/html");
    } else if(strncmp(endOfUrl - sizeof(".json"),".json", sizeof(".json")) == 0){
        client->println("Content-Type: application/json");
    } else if(strncmp(endOfUrl - sizeof(".log"),".log", sizeof(".log")) == 0) {
        client->println("Content-Type: text/csv");
    } else if(strncmp(endOfUrl - sizeof(".jpg"),".jpg", sizeof(".jpg")) == 0){
        client->println("Content-Type: image/jpeg");
        //may need to finagle size in here
    }
    client->println("Connection: close");
    client->println();

    if(strncmp(url, "/", sizeof("/")) == 0) {
        printIndexPage(client);
    } else if(strncmp(url, "/sensors.json", sizeof("/sensors.json")) == 0){
        sensors->printJson(client);
    } else if(strncmp(url, "/battery.json", sizeof("/battery.json")) == 0){
        bms->printCellVoltages(client);
        bms->printFaults(client);
        bms->printStates(client);
    } else if(strncmp(url, "/switches.json", sizeof("/switches.json")) == 0) {
        relay->printJSON(client);
    }else if(strncmp(url, "/capture.jpg", sizeof("/capture.jpg")) == 0){
        camera->captureAndPrintImage(client);
    } else {
        File page = SD.open(&url[1]);
        uint8_t buffer[1024] = {0};
        while(page.available() > 0){
            int bytesToBeRead = min((unsigned int) page.available(), sizeof(buffer));
            page.read(&buffer[0], bytesToBeRead);
            client->write(buffer, bytesToBeRead);
        }
    }
}

void WebServer::readAndLogRequestLines(Stream *client) {
    char s[265]{0};

    while (client->available()) {
        client->readBytesUntil('\n', &s[0], sizeof(s));
#if DEBUG_WEB_SERVER
        if(Serial) Serial.println(s);
#endif
        if(s[0] == '\r'){
            break;
        }
    }
}

void WebServer::printIndexPage(Stream *client) {
    for(auto line : pageTop){
        client->println(line);
    }
    char buffer[265] = {0};
    for(int i =0; i< Relay::numPorts; i++){
        client->println(R"===(<tr>)===");
        sprintf(buffer, R"===(<td class="align-middle" id="n%d"></td>)===", i);
        client->println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><div id="s%d"></div></td>)===", i);
        client->println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><form method="post"><input name="power%d" type="hidden" value="1" id="i%d"><button type="submit" id="b%d"></button></form></td>)===", i, i ,i);
        client->println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><form method="post"><input name="power%d" type="hidden" value="2"><button type="submit" class="btn btn-dark btn-block">Cycle</button></form></td>)===", i);
        client->println(buffer);
        sprintf(buffer, R"===(</tr>)===");
        client->println(buffer);
    }
    for(auto line : pageBottom){
        client->println(line);
    }
}

WebServer::WebServer(Relay *aRelayPtr, Bms *aBmsPtr, Sensors *aSensorPtr, Camera *aCameraPtr) {
    this->relay = aRelayPtr;
    this->bms = aBmsPtr;
    this->sensors = aSensorPtr;
    this->camera = aCameraPtr;
}

void WebServer::begin() {
    EthernetClass::begin(mac);
    if(!EthernetClass::begin(mac)){
        if(Serial) Serial.println("Ethernet/DHCP failed");
    }
    server.begin();
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(EthernetClass::localIP());
#endif
}

