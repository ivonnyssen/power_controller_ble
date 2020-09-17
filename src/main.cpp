/*
 * Power Controller on a web page
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Relay module is attached to digital pins 2, 3, 5, 6
 * Ethernet CS pin is 10
 * SD card CS pin is 4
 * Arducam CS pin is 7
 * Battery Management System is attached to RX and TX (Serial 1)
 */

#define DEBUG true

#if defined(ARDUINO)
#include <Camera.h>
#include <SD.h>
#include <SPI.h>
#else
#include <ntp/mocks/MockNtp.h>
#endif

#include <Ntp.h>
#include <webserver.h>
#include <Relay.h>
#include <Sensors.h>
#include <mbed.h>
#include <Bms.h>

#define ETHERNET_CS_PIN 10
#define SD_CARD_CS_PIN 4
#define ARDUCAM_CS_PIN 7

Relay* relay;

Sensors* sensors;
#define SENSOR_UPDATE_INTERVAL 10

Bms* bms;
#define BMS_UPDATE_INTERVAL 61

WebServer* webServer;

#define PST -8
Ntp* ntp;

Camera* camera;

#ifndef UNIT_TEST
void setup() {
    Serial.begin(256000);

    relay = new Relay;
    sensors = new Sensors;
    bms = new Bms(&Serial1);
    ntp = new Ntp(PST);
    camera = new Camera(ARDUCAM_CS_PIN);
    webServer = new WebServer(relay, bms, sensors, camera);

    //get all CS pins to high before we do anything to the bus
    pinMode(SD_CARD_CS_PIN,OUTPUT);
    digitalWrite(SD_CARD_CS_PIN, HIGH);
    pinMode(ARDUCAM_CS_PIN, OUTPUT);
    digitalWrite(ARDUCAM_CS_PIN, HIGH);
    pinMode(ETHERNET_CS_PIN,OUTPUT);
    digitalWrite(ETHERNET_CS_PIN, HIGH);

    if (!SD.begin(SD_CARD_CS_PIN)) {
        if(Serial) Serial.println("SDCard failed, or not present");
    }
    Serial.println("Here");
    webServer->begin();
    ntp->begin();
    relay->begin();
    sensors->begin();
    bms->begin();
    camera->begin();
}

void  loop() {

    // listen for incoming requests and handle them
    webServer->handleHttpRequest();

    time_t seconds = time(nullptr);
    if(seconds % SENSOR_UPDATE_INTERVAL == 0 && seconds != sensors->lastLogTime) {
        sensors->measureAndLog();
    }

    if(seconds % NTP_SYNC_INTERVAL == 0 && seconds != ntp->lastSyncTime){
        ntp->syncTime();
    }

    if(seconds % BMS_UPDATE_INTERVAL == 0 && seconds != bms->lastPollTime){
        bms->poll();
    }

    if(seconds % SECS_PER_DAY == 0){
        bms->clear24Values();
        bms->clearFaultCounts();
    }
//    if(seconds % 60 == 0 && seconds != camera.lastCaptureTime){
//    }
}

#endif




