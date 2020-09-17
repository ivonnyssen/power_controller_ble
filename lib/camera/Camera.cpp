#if defined(ARDUINO)
#include <SPI.h>
#include "Camera.h"

void Camera::begin() {
    uint8_t vid, pid;
    uint8_t temp;
    myCAM->write_reg(0x07, 0x80);
    delay(100);
    myCAM->write_reg(0x07, 0x00);
    delay(100);
    while(true) {
        //Check if the ArduCAM SPI bus is OK
        myCAM->write_reg(ARDUCHIP_TEST1, 0x55);
        temp = myCAM->read_reg(ARDUCHIP_TEST1);
        if (temp != 0x55)
        {
            if(Serial) Serial.println("SPI interface Error!");
            delay(1000);
            continue;
        } else {
            if(Serial) Serial.println("SPI interface OK.");
            break;
        }
    }
    while(true) {
        //Check if the camera module type is OV2640
        myCAM->wrSensorReg8_8(0xff, 0x01);
        myCAM->rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
        myCAM->rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
        if ((vid != 0x26 ) || !(( pid == 0x41 ) || ( pid == 0x42 ))) {
            if(Serial) Serial.println("Can't find OV2640 module!");
            delay(1000);
            continue;
        }
        else {
            if(Serial) Serial.println("OV2640 detected.");
            break;
        }
    }
    myCAM->set_format(JPEG);
    myCAM->InitCAM();
    myCAM->OV2640_set_JPEG_size(OV2640_1600x1200);
    myCAM->clear_fifo_flag();
    myCAM->write_reg(ARDUCHIP_FRAMES, 0x00); //only one image
}

void Camera::captureAndPrintImage(Stream *client) {
    myCAM->flush_fifo();
    myCAM->clear_fifo_flag();
    myCAM->start_capture();
    if(Serial) Serial.println("start capture.");
    unsigned long total_time = millis();
    while (!myCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
    total_time = millis() - total_time;
    if(Serial) Serial.print("capture total_time used (in milliseconds):");
    if(Serial) Serial.println(total_time, DEC);
    total_time = millis();
    uint8_t buffer[bufferSize];
    uint32_t bytesRemaining = myCAM->read_fifo_length();
    if(Serial) Serial.print("The fifo length is :");
    if(Serial) Serial.println(bytesRemaining, HEX);
    if(bytesRemaining > MAX_FIFO_SIZE) {
        if(Serial) Serial.println("Over size.");
        return;
    }
    if(bytesRemaining == 0 ) {
        if(Serial) Serial.println("Size is 0.");
        return;
    }
    while(bytesRemaining > 0) {
        myCAM->CS_LOW();
        SPI.transfer(BURST_FIFO_READ);
        size_t bytesToRead = min(bytesRemaining, sizeof(buffer));
        SPI.transfer(&buffer[0], bytesToRead);
        myCAM->CS_HIGH();
        client->write(buffer, bytesToRead);
        bytesRemaining = bytesRemaining < sizeof(buffer) ? 0 : bytesRemaining-sizeof(buffer);
    }
    total_time = millis() - total_time;
    if(Serial) Serial.print("save capture total_time used (in milliseconds):");
    if(Serial) Serial.println(total_time, DEC);
}

Camera::Camera(int csPin) {
    myCAM = new ArduCAM(OV2640, csPin);
}

Camera::~Camera() {
    delete myCAM;
}
#endif //ARDUINO