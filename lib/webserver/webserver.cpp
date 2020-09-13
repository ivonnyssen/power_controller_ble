#include "webserver.h"
#include "indexpage.h"
#include <SD.h>

void WebServer::handleHttpRequest() {
    EthernetClient client = server.available();
    if (client.available()) {
        Request request = parseRequest(client);
        switch (request.type) {
            case GET:
                sendResponse(client, request.url, GET);
                break;
            case POST:
                switch(request.command){
                    case OFF:
                        relay.togglePort(request.powerPort, false);
                        break;
                    case ON:
                        relay.togglePort(request.powerPort, true);
                        break;
                    case CYCLE:
                        relay.togglePort(request.powerPort, false);
                        delay(1000);
                        relay.togglePort(request.powerPort, true);
                        break;
                    default:
                        break;
                }
                sendResponse(client, "/", POST);
            default:
                break;
        }
    }
    delay(10);
    client.stop();
}

Request WebServer::parseRequest(Client &client) {
    Request result{};

    String s = client.readStringUntil('\n');
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(s);
#endif
    if(s.startsWith("GET")){
        result.type = GET;
        result.url = s.substring(4, s.lastIndexOf(' '));
        readAndLogRequestLines(client);
    } else if(s.startsWith("POST")){
        result.type = POST;
        readAndLogRequestLines(client);
        if(client.available()){
            s = client.readStringUntil('\n');
            if(s.startsWith("power")){
                result.powerPort = s.substring(5,6).toInt();
                result.command = s.substring(7,8).toInt();
            }
        }
    } else {
        result.type = UNSUPPORTED;
    }
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(result.url);
#endif
    return result;
}

void WebServer::sendResponse(Client &client, const String &url, int type) {
    //print header
    if (type == POST) {
        client.println("HTTP/1.1 303 See Other");
        char buffer[64] = {0};
        sprintf(buffer, "Location: http://%d.%d.%d.%d%s",EthernetClass::localIP()[0],EthernetClass::localIP()[1],
                EthernetClass::localIP()[2],EthernetClass::localIP()[3],url.c_str());
        client.println(buffer);
    } else {
        client.println("HTTP/1.1 200 OK");
        if(url.equals("/")){
            char buffer[64] = {0};
            sprintf(buffer, "Refresh: 450; url=http://%d.%d.%d.%d%s",EthernetClass::localIP()[0],
                    EthernetClass::localIP()[1],EthernetClass::localIP()[2],EthernetClass::localIP()[3],url.c_str());
            client.println(buffer);
        }
    }

    if(url.endsWith(".html")){
        client.println("Content-Type: text/html");
    } else if(url.endsWith(".json")){
        client.println("Content-Type: application/json");
    } else if(url.endsWith(".log")) {
        client.println("Content-Type: text/csv");
    }
    client.println("Connection: close");
    client.println();

    if(url.equals("/")) {
        printIndexPage(client);
    } else if(url.equals("/sensors.json")){
        sensors.printJson(client);
    } else if(url.equals("/battery.json")){
        bms.printCellVoltages(client);
        bms.printFaults(client);
        bms.printStates(client);
    } else if(url.equals("/switches.json")){
        relay.printJSON(client);
    } else {
        File page = SD.open(url.substring(1));
        const uint8_t *buffer = {0};
        while(page.available() > 0){
            int bytesToBeRead = min((unsigned int) page.available(), sizeof(buffer));
            page.read((void *) buffer, bytesToBeRead);
            client.write(buffer, bytesToBeRead);
        }
    }
}

void WebServer::readAndLogRequestLines(Client &client) {
    while (client.available()) {
        String s = client.readStringUntil('\n');
#if DEBUG_WEB_SERVER
        if(Serial) Serial.println(s);
#endif
        if(s.equals(String('\r'))){
            break;
        }
    }
}

void WebServer::printIndexPage(Client &client) {
    for(auto line : pageTop){
        client.println(line);
    }
    char buffer[265] = {0};
    for(int i =0; i< Relay::numPorts; i++){
        client.println(R"===(<tr>)===");
        sprintf(buffer, R"===(<td class="align-middle" id="n%d"></td>)===", i);
        client.println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><div id="s%d"></div></td>)===", i);
        client.println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><form method="post"><input name="power%d" type="hidden" value="1" id="i%d"><button type="submit" id="b%d"></button></form></td>)===", i, i ,i);
        client.println(buffer);
        sprintf(buffer, R"===(<td class="align-middle"><form method="post"><input name="power%d" type="hidden" value="2"><button type="submit" class="btn btn-dark btn-block">Cycle</button></form></td>)===", i);
        client.println(buffer);
        sprintf(buffer, R"===(</tr>)===");
        client.println(buffer);
    }
    for(auto line : pageBottom){
        client.println(line);
    }
}

void WebServer::begin(Relay& aRelayRef, Bms& aBmsRef, Sensors& aSensorRef) {
    this->relay = aRelayRef;
    this->bms = aBmsRef;
    this->sensors = aSensorRef;
    EthernetClass::begin(mac);
    if(!EthernetClass::begin(mac)){
        if(Serial) Serial.println("Ethernet/DHCP failed");
    }
    server.begin();
#if DEBUG_WEB_SERVER
    if(Serial) Serial.println(EthernetClass::localIP());
#endif
}

