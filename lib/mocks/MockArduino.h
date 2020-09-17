#ifndef POWER_CONTROLLER_BLE_MOCKARDUINO_H
#define POWER_CONTROLLER_BLE_MOCKARDUINO_H
#include <cstddef>
#include <cstdint>
#include <cstdio>

#define PROGMEM
#define DEC 1

#ifdef __cplusplus
template<class T, class L>
auto min(const T& a, const L& b) -> decltype((b < a) ? b : a)
{
    return (b < a) ? b : a;
}

template<class T, class L>
auto max(const T& a, const L& b) -> decltype((b < a) ? b : a)
{
    return (a < b) ? b : a;
}
#else
#ifndef min
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif
#ifndef max
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif
#endif

typedef enum {
    LOW     = 0,
    HIGH    = 1,
    CHANGE  = 2,
    FALLING = 3,
    RISING  = 4,
} PinStatus;

typedef enum {
    INPUT           = 0x0,
    OUTPUT          = 0x1,
    INPUT_PULLUP    = 0x2,
    INPUT_PULLDOWN  = 0x3,
} PinMode;

typedef uint8_t pin_size_t;
void pinMode(pin_size_t pinNumber, PinMode pinMode);
PinStatus digitalRead(pin_size_t pin);
void digitalWrite(pin_size_t pin, PinStatus val);

class Printable{

};

class Stream {
public:
    static int available();
    static size_t readBytesUntil(char terminator, char *buffer, size_t length);
    static size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length);
    static size_t print(const char *);
    static size_t print(double, int = 2);
    static size_t println();
    static size_t println(const char[]);
    static size_t println(const Printable&);
    static size_t println(int, int = 10);
    static size_t write(const uint8_t *buffer, size_t size);
    void setTimeout(unsigned long timeout);
};

class SerialUsb : public Stream {
public:
    using Stream::println;
    using Stream::print;
    explicit operator bool();
};

extern SerialUsb Serial;
extern SerialUsb Serial1;

void delay(int milliSeconds);

#endif //POWER_CONTROLLER_BLE_MOCKARDUINO_H
