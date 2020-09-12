/*
 Power Controller on a web page
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Relay module is attached to digital pins 2, 3, 4, 5
 */

#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <mbed.h>
#include <page.h>
#include <Adafruit_BME280.h>
#include <bms.h>

#define GET 0
#define POST 1
#define UNSUPPORTED 2

#define OFF 0
#define ON 1
#define CYCLE 2
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY 86400

#define NUM_PORTS 4

#define DEBUG true

typedef struct Request{
    int type;
    String url;
    long powerPort;
    long command;
} Request;

typedef struct SensorData{
    time_t readoutTime;
    float pressure;
    float temperature;
    float humidity;
} SensorData;

Request parseRequest(EthernetClient client);

void readAndLogRequestLines(EthernetClient client);

void printWebPage(EthernetClient client, const String &url, int type);

void sendNtpPacket(const char * address);

void handleHttpRequest(EthernetClient &client);

time_t getNtpTime();

void measureAndLogSensors(time_t &now);

void printBmsFaults(EthernetClient &client);

void printCellVoltages(EthernetClient &client);

void printBmsStates(EthernetClient &client);

void printSensorsJson(EthernetClient &client);

void printIndexPage(EthernetClient &client);

uint8_t getPinForPort(uint8_t port);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//NTP stuff
#define 	NTP_OFFSET   3155673600
#define 	UNIX_OFFSET   946684800
const char ntpServer[] = "time.nist.gov";
const int ntpPacketSize = 48;
uint8_t packetBuffer[ntpPacketSize];
const int timeZone = -8; //PST
unsigned int localPort = 8888;
EthernetUDP Udp;

//sensor data
const int numSensorRecords = 24 * 4;
SensorData sensorData[numSensorRecords] = {{0}};
time_t lastSensorLogTime;

#define PORT0_PIN 2
#define PORT1_PIN 3
#define PORT2_PIN 5
#define PORT3_PIN 6
//variables for states
bool ports[4] {false,false,false,false};

//BME280
Adafruit_BME280 bme;

//Serial BMS connection
BMS bms;
time_t lastBmsCheckTime;

#ifndef UNIT_TEST
void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    //initialize sd card
    pinMode(10,OUTPUT);
    digitalWrite(10, HIGH);
    if (!SD.begin(4)) {
        if(Serial) Serial.println("Card failed, or not present");
    }

    // start the Ethernet connection and the server:
    EthernetClass::begin(mac);
    server.begin();
#if DEBUG
    if(Serial) Serial.println(EthernetClass::localIP());
#endif
    //UDP for NTP
    Udp.begin(localPort);
    delay(2000);
    set_time(getNtpTime());
    lastSensorLogTime = time(nullptr);

    //relay module setup
    for(int i=0; i < NUM_PORTS; i++){
        pinMode(getPinForPort(i), OUTPUT);
        ports[i] = digitalRead(getPinForPort(i)) != HIGH;
    }

    //bme280, reade the first value - it is likely garbage
    bme.begin();
    char buffer[64] = {0};
    struct tm* logTime = localtime(&lastSensorLogTime);
    sprintf(buffer, "%02d-%02d %02d:%02d, %.2f, %.2f, %.2f", logTime->tm_mon + 1, logTime->tm_mday, logTime->tm_hour, logTime->tm_min,
            bme.readPressure()/ 100.0F, bme.readTemperature(), bme.readHumidity());
#if DEBUG
    if(Serial) Serial.println(buffer);
#endif

    //BMS
    bms.begin(&Serial1);
}

void  loop() {

    // listen for incoming clients
    EthernetClient client = server.available();
    if (client) {
        handleHttpRequest(client);
    }

    time_t seconds = time(nullptr);
    if(seconds % 900 == 0 && seconds != lastSensorLogTime) {
        measureAndLogSensors(seconds);
        lastSensorLogTime = seconds;
    }

    //make sure not to use a prime number, so timeouts do not prevent sensor reads too often 900 * 61 = 54900
    if(seconds % 61 == 0 && seconds != lastBmsCheckTime){
        bms.poll();
        lastBmsCheckTime = seconds;
    }

    if(seconds % SECS_PER_DAY == 0){
        bms.clear24Values();
        bms.clearFaultCounts();
    }
}
#endif

uint8_t getPinForPort(uint8_t port){
    switch (port) {
        case 0:
            return PORT0_PIN;
        case 1:
            return PORT1_PIN;
        case 2:
            return PORT2_PIN;
        case 3:
            return PORT3_PIN;
        default:
            return PORT0_PIN;
    }
}

void measureAndLogSensors(time_t &now) {
    for(int i = 0; i < numSensorRecords - 1; i++){
        sensorData[i] = sensorData[i + 1];
    }
    sensorData[numSensorRecords - 1] = {now, bme.readPressure() / 100.0F, bme.readTemperature(), bme.readHumidity()};

    char buffer[64] = {0};
    struct tm* logTime = localtime(&sensorData[numSensorRecords - 1].readoutTime);
    sprintf(buffer, "%02d-%02d %02d:%02d, %.2f, %.2f, %.2f", logTime->tm_mon + 1, logTime->tm_mday, logTime->tm_hour, logTime->tm_min,
            sensorData[numSensorRecords - 1].pressure, sensorData[numSensorRecords - 1].temperature, sensorData[numSensorRecords - 1].humidity);
    char filename[12] = {0};
    int result = snprintf(filename, sizeof(filename), "%04d%02d.log", logTime->tm_year + 1900, logTime->tm_mon + 1);
    if(result < 0) {
        if(Serial) Serial.println("buffer overflow in filename");
    } else{
        File logFile = SD.open(filename, FILE_WRITE);
        logFile.println(buffer);
        logFile.close();
    }
#if DEBUG
    if(Serial) Serial.println(buffer);
#endif
}

void handleHttpRequest(EthernetClient &client) {
    if (client.available()) {
        Request request = parseRequest(client);
        switch (request.type) {
            case GET:
                printWebPage(client, request.url, GET);
                break;
            case POST:
                switch(request.command){
                    case OFF:
                        ports[request.powerPort] = false;
                        digitalWrite(getPinForPort(request.powerPort), HIGH);
                        break;
                    case ON:
                        ports[request.powerPort] = true;
                        digitalWrite(getPinForPort(request.powerPort), LOW);
                        break;
                    case CYCLE:
                        ports[request.powerPort] = false;
                        digitalWrite(getPinForPort(request.powerPort), HIGH);
                        delay(1000);
                        ports[request.powerPort] = true;
                        digitalWrite(getPinForPort(request.powerPort), LOW);
                        break;
                    default:
                        break;
                }
                printWebPage(client, "/", POST);
            default:
                break;
        }
    }

    delay(10);
    client.stop();
}

Request parseRequest(EthernetClient client) {
    Request result{};

    String s = client.readStringUntil('\n');
#if DEBUG
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
                result.powerPort=s.substring(5,6).toInt();
                result.command=s.substring(7,8).toInt();
            }
        }
    } else {
        result.type = UNSUPPORTED;
    }
#if DEBUG
    if(Serial) Serial.println(result.url);
#endif
    return result;
}

void readAndLogRequestLines(EthernetClient client) {
    while (client.available()) {
        String s = client.readStringUntil('\n');
#if DEBUG
        if(Serial) Serial.println(s);
#endif
        if(s.equals(String('\r'))){
            break;
        }
    }
}

void printWebPage(EthernetClient client, const String &url, const int type) {
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
        printSensorsJson(client);
    } else if(url.equals("/battery.json")){
        printCellVoltages(client);
        printBmsFaults(client);
        printBmsStates(client);
    } else if(url.equals("/switches.json")){
        client.println(R"===({ "switches": [)===");
        char buffer[64] = {0};
        sprintf(buffer,R"===({"name": "Imaging Computer 1", "state": %s},)===", ports[0] ? "true" : "false");
        client.println(buffer);
        sprintf(buffer,R"===({"name": "Imaging Computer 2", "state": %s},)===", ports[1] ? "true" : "false");
        client.println(buffer);
        sprintf(buffer,R"===({"name": "Port 3", "state": %s},)===", ports[2] ? "true" : "false");
        client.println(buffer);
        sprintf(buffer,R"===({"name": "Port 4", "state": %s})===", ports[3] ? "true" : "false");
        client.println(buffer);
        client.println(R"===(]})===");
    } else {
        File page = SD.open(url.substring(1));
        char buffer[1024] = {0};
        while(page.available() > 0){
            int bytesToBeRead = min((unsigned int) page.available(), sizeof(buffer));
            page.read(buffer, bytesToBeRead);
            client.write(buffer, bytesToBeRead);
        }
    }
}

void printIndexPage(EthernetClient &client) {
    for(auto line : pageTop){
        client.println(line);
    }
    char buffer[265] = {0};
    for(int i =0; i< NUM_PORTS; i++){
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

void printSensorsJson(EthernetClient &client) {
    client.println(R"===({ "values":[)===");
    for(int i = 0; i < numSensorRecords; i++){
        char buffer[128] = {0};
        struct tm* logTime = localtime(&sensorData[i].readoutTime);
        sprintf(buffer, R"===({"time":"%02d-%02d %02d:%02d", "pressure":%.2f, "temp":%.2f, "humidity":%.2f})===", logTime->tm_mon + 1, logTime->tm_mday, logTime->tm_hour,
                logTime->tm_min, sensorData[i].pressure, sensorData[i].temperature, sensorData[i].humidity);
        client.print(buffer);
        if(i != numSensorRecords - 1) {
            client.println(",");
        } else {
            client.println();
        }
    }
    client.println("]}");
}

void printBmsStates(EthernetClient &client) {
    char buffer[64] = {0};
    sprintf(buffer, R"===("charge": "%.2fA",)===", bms.current < 0 ? 0 : bms.current);
    client.println(buffer);
    sprintf(buffer, R"===("discharge": "%.2fA",)===", bms.current < 0 ? -bms.current : 0);
    client.println(buffer);
    sprintf(buffer, R"===("totalVoltage": "%.2fV",)===", bms.totalVoltage);
    client.println(buffer);
    sprintf(buffer, R"===("remainingSOC": %d,)===", bms.stateOfCharge);
    client.println(buffer);
    sprintf(buffer, R"===("minVoltage": "%.2fV",)===", bms.minVoltage24);
    client.println(buffer);
    sprintf(buffer, R"===("maxVoltage": "%.2fV",)===", bms.maxVoltage24);
    client.println(buffer);
    sprintf(buffer, R"===("maxCharge": "%.2fA",)===", bms.maxCharge24);
    client.println(buffer);
    sprintf(buffer, R"===("maxDischarge": "%.2fA",)===", bms.maxDischarge24);
    client.println(buffer);
    sprintf(buffer, R"===("maxPower": "%.2fW",)===", bms.balanceCapacity);
    client.println(buffer);
    sprintf(buffer, R"===("temp1": "%.2fC",)===", bms.temperatures[0]);
    client.println(buffer);
    sprintf(buffer, R"===("temp2": "%.2fC")===", bms.temperatures[1]);
    client.println(buffer);
    client.println("}");
}

void printCellVoltages(EthernetClient &client) {
    client.println(R"===({ "cellVoltages":[)===");
    for(int i = 0; i < NUM_CELLS; i++){
        char buffer[64] = {0};
        sprintf(buffer, R"===({"cell":"%d", "cellVoltage":%.3f, "balancing": %s})===", i, bms.cellVoltages[i], bms.isBalancing(i) ? "true" : "false");
        client.print(buffer);
        if(i != NUM_CELLS - 1) {
            client.println(",");
        } else {
            client.println();
        }
    }
    client.println(R"===(],)===");
}

void printBmsFaults(EthernetClient &client) {
    client.println(R"===("faults": [)===");
    char buffer[64] = {0};
    sprintf(buffer,R"===({"fault": "Single Cell Over-Voltage", "count": %d},)===", bms.faultCounts.singleCellOvervoltageProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Single Cell Under-Voltage", "count": %d},)===", bms.faultCounts.singleCellUndervoltageProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Whole Pack Over-Voltage", "count": %d},)===", bms.faultCounts.wholePackOvervoltageProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Whole Pack Under-Voltage", "count": %d},)===", bms.faultCounts.wholePackUndervoltageProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Charging Over Temperature", "count": %d},)===", bms.faultCounts.chargingOverTemperatureProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Charging Low Temperature", "count": %d},)===", bms.faultCounts.chargingLowTemperatureProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Discharge Over Temperature", "count": %d},)===", bms.faultCounts.dischargeOverTemperatureProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Discharge Low Temperature", "count": %d},)===", bms.faultCounts.dischargeLowTemperatureProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Charging Over-Current", "count": %d},)===", bms.faultCounts.chargingOvercurrentProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Discharge Over-Current", "count": %d},)===", bms.faultCounts.dischargeOvercurrentProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Short Circuit", "count": %d},)===", bms.faultCounts.shortCircuitProtection);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Front End Detection Ic Error", "count": %d},)===", bms.faultCounts.frontEndDetectionIcError);
    client.println(buffer);
    sprintf(buffer,R"===({"fault": "Software Lock Mos", "count": %d})===", bms.faultCounts.softwareLockMos);
    client.println(buffer);
    client.println(R"===(],)===");
}

// send an NTP request to the time server at the given address
void sendNtpPacket(const char * address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, ntpPacketSize);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); // NTP requests are to port 123
    Udp.write(packetBuffer, ntpPacketSize);
    Udp.endPacket();
}

time_t getNtpTime() {
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    sendNtpPacket(ntpServer);
    uint32_t beginWait = millis();
    uint32_t now = beginWait;
    while (beginWait > now - 2000) {
        int size = Udp.parsePacket();
        if (size >= ntpPacketSize) {
            Udp.read(packetBuffer, ntpPacketSize);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 =  (unsigned long)packetBuffer[40] << (uint8_t) 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << (uint8_t) 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << (uint8_t) 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - NTP_OFFSET + UNIX_OFFSET + timeZone * SECS_PER_HOUR;
        }
        now = millis();
    }
    return 0; // return 0 if unable to get the time
}

