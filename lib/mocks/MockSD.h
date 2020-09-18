#ifndef POWER_CONTROLLER_BLE_MOCKSD_H
#define POWER_CONTROLLER_BLE_MOCKSD_H

#include <MockArduino.h>

#define FILE_WRITE 0x00
#define FILE_READ 0x01

class File : public Stream{
public:
    File() = default;

    static size_t write(const uint8_t *buf, size_t size);

    static int availableForWrite();
    static int available();
    static int read(void *buf, uint16_t nbyte);
    void close();

    explicit operator bool();
};

class SDClass{
public:
    static File open(const char *filename, uint8_t mode = FILE_READ);
};

extern SDClass SD;

#endif //POWER_CONTROLLER_BLE_MOCKSD_H
