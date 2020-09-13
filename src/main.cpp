/*
 * Power Controller on a web page
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Relay module is attached to digital pins 2, 3, 5, 6
 * SD card CS pin is 4
 * Arducam CS pin is 7
 * Batter Management System is attached to RX and TX (Serial 1)
 */

#define DEBUG true

#include <SPI.h>
#include <SD.h>
#include <Ntp.h>
#include <webserver.h>
#include <Relay.h>
#include <Sensors.h>
#include <mbed.h>
#include <Bms.h>
#include <ArduCAM.h>
#include "memorysaver.h"

#define ETHERNET_CS_PIN 10
#define SD_CARD_CS_PIN 4
#define ARDUCAM_CS_PIN 7

#define NUM_FRAMES 0x02

Relay relay;

Sensors sensors;
#define SENSOR_UPDATE_INTERVAL 10

Bms bms;
#define BMS_UPDATE_INTERVAL 61

WebServer webServer;

#define PST -8
Ntp ntp(PST);

//Arducam
bool is_header = false;
int total_time = 0;
ArduCAM myCAM( OV2640, ARDUCAM_CS_PIN );
uint8_t read_fifo_burst(ArduCAM myCAM);
//End Arducam


#ifndef UNIT_TEST
void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    //get all CS pins to high
    pinMode(SD_CARD_CS_PIN,OUTPUT);
    digitalWrite(SD_CARD_CS_PIN, HIGH);
    pinMode(ARDUCAM_CS_PIN, OUTPUT);
    digitalWrite(ARDUCAM_CS_PIN, HIGH);
    pinMode(ETHERNET_CS_PIN,OUTPUT);
    digitalWrite(ETHERNET_CS_PIN, HIGH);

    //initialize sd card
    if (!SD.begin(SD_CARD_CS_PIN)) {
        if(Serial) Serial.println("Card failed, or not present");
    }

    webServer.begin(relay, bms, sensors);
    ntp.begin();
    relay.begin();
    sensors.begin();
    bms.begin(&Serial1);

    //arducam
    uint8_t vid, pid;
    uint8_t temp;
    Serial.println(F("ArduCAM Start!"));
    // initialize SPI:
//    SPI.begin();
    //Reset the CPLD
    myCAM.write_reg(0x07, 0x80);
    delay(100);
    myCAM.write_reg(0x07, 0x00);
    delay(100);
    while (1) {
        //Check if the ArduCAM SPI bus is OK
        myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
        temp = myCAM.read_reg(ARDUCHIP_TEST1);
        if (temp != 0x55)
        {
            Serial.println(F("SPI interface Error!"));
            delay(1000); continue;
        } else {
            Serial.println(F("SPI interface OK.")); break;
        }
    }
#if defined (OV2640_MINI_2MP_PLUS)
    while (1) {
        //Check if the camera module type is OV2640
        myCAM.wrSensorReg8_8(0xff, 0x01);
        myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
        myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
        if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))) {
            Serial.println(F("ACK CMD Can't find OV2640 module!"));
            delay(1000); continue;
        }
        else {
            Serial.println(F("ACK CMD OV2640 detected.")); break;
        }
    }
#endif
    //Change to JPEG capture mode and initialize the OV5640 module
    myCAM.set_format(JPEG);
    myCAM.InitCAM();
    myCAM.clear_fifo_flag();
    myCAM.write_reg(ARDUCHIP_FRAMES, NUM_FRAMES);
}

void  loop() {
    // put your main code here, to run repeatedly:
    myCAM.flush_fifo();
    myCAM.clear_fifo_flag();
#if defined (OV2640_MINI_2MP_PLUS)
    myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
#endif
    //Start capture
    myCAM.start_capture();
    Serial.println(F("start capture."));
    total_time = millis();
    while ( !myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
    Serial.println(F("CAM Capture Done."));
    total_time = millis() - total_time;
    Serial.print(F("capture total_time used (in miliseconds):"));
    Serial.println(total_time, DEC);
    total_time = millis();
    read_fifo_burst(myCAM);
    total_time = millis() - total_time;
    Serial.print(F("save capture total_time used (in miliseconds):"));
    Serial.println(total_time, DEC);
    //Clear the capture done flag
    myCAM.clear_fifo_flag();
    delay(5000);

    // listen for incoming requests and handle them
    webServer.handleHttpRequest();

    time_t seconds = time(nullptr);
    if(seconds % SENSOR_UPDATE_INTERVAL == 0 && seconds != sensors.lastLogTime) {
        sensors.measureAndLog(seconds);
    }

    if(seconds % NTP_SYNC_INTERVAL == 0 && seconds != ntp.lastSyncTime){
        ntp.syncTime();
    }

    if(seconds % BMS_UPDATE_INTERVAL == 0 && seconds != bms.lastPollTime){
        bms.poll();
    }

    if(seconds % SECS_PER_DAY == 0){
        bms.clear24Values();
        bms.clearFaultCounts();
    }
}

uint8_t read_fifo_burst(ArduCAM myCAM)
{
    uint8_t temp = 0, temp_last = 0;
    uint32_t length = 0;
    static int i = 0;
    static int k = 0;
    char str[16];
    File outFile;
    byte buf[256];
    length = myCAM.read_fifo_length();
    Serial.print(F("The fifo length is :"));
    Serial.println(length, DEC);
    if (length >= MAX_FIFO_SIZE) //8M
    {
        Serial.println("Over size.");
        return 0;
    }
    if (length == 0 ) //0 kb
    {
        Serial.println(F("Size is 0."));
        return 0;
    }
    myCAM.CS_LOW();
    myCAM.set_fifo_burst();//Set fifo burst mode
    i = 0;
    while ( length-- )
    {
        temp_last = temp;
        temp =  SPI.transfer(0x00);
        //Read JPEG data from FIFO
        if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
        {
            buf[i++] = temp;  //save the last  0XD9
            //Write the remain bytes in the buffer
            myCAM.CS_HIGH();
            outFile.write(buf, i);
            //Close the file
            outFile.close();
            Serial.println(F("OK"));
            is_header = false;
            myCAM.CS_LOW();
            myCAM.set_fifo_burst();
            i = 0;
        }
        if (is_header == true)
        {
            //Write image data to buffer if not full
            if (i < 256)
                buf[i++] = temp;
            else
            {
                //Write 256 bytes image data to file
                myCAM.CS_HIGH();
                outFile.write(buf, 256);
                i = 0;
                buf[i++] = temp;
                myCAM.CS_LOW();
                myCAM.set_fifo_burst();
            }
        }
        else if ((temp == 0xD8) & (temp_last == 0xFF))
        {
            is_header = true;
            myCAM.CS_HIGH();
            //Create a avi file
            k = k + 1;
            itoa(k, str, 10);
            strcat(str, ".jpg");
            //Open the new file
            outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
            if (! outFile)
            {
                Serial.println(F("File open failed"));
                while (1);
            }
            myCAM.CS_LOW();
            myCAM.set_fifo_burst();
            buf[i++] = temp_last;
            buf[i++] = temp;
        }
    }
    myCAM.CS_HIGH();
    return 1;
}
#endif




