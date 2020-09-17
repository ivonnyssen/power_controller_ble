#ifndef POWER_CONTROLLER_BLE_MOCKSD_H
#define POWER_CONTROLLER_BLE_MOCKSD_H

#include <MockArduino.h>

#define FILE_WRITE 0x00
#define FILE_READ 0x01

class File : public Stream{
public:
    File() = default;

    static size_t write(const uint8_t *buf, size_t size) { return 0; }

    static int availableForWrite() { return 1; }
    static int available() { return 1; }
    static int read(void *buf, uint16_t nbyte) { return nbyte; }
    void close() {}

    explicit operator bool() { return true; }
};

class SDClass{
public:
    File open(const char *filename, uint8_t mode = FILE_READ) { return File(); }
};

SDClass SD;

#endif //POWER_CONTROLLER_BLE_MOCKSD_H
