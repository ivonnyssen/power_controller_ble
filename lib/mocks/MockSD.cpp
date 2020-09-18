#include "MockSD.h"

    size_t File::write(const uint8_t *buf, size_t size) { return 0; }

    int File::availableForWrite() { return 1; }
    int File::available() { return 1; }
    int File::read(void *buf, uint16_t nbyte) { return nbyte; }
    void File::close() {}

    File::operator bool() { return true; }

    File SDClass::open(const char *filename, uint8_t mode) { return File(); }

SDClass SD;