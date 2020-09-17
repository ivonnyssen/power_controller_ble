#include "MockArduino.h"
void Stream::setTimeout(unsigned long timeout) {}

int Stream::available(){ return 256; }
size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length) { return 0; };
size_t Stream::readBytesUntil(char terminator, uint8_t *buffer, size_t length) { return readBytesUntil(terminator, (char *)buffer,length); };
size_t Stream::print(const char *) { return 0; }
size_t Stream::print(double, int) { return 0; }
size_t Stream::println() { return 0; }
size_t Stream::println(const char[]) { return 0; }
size_t Stream::println(const Printable&) { return 0; }
size_t Stream::println(int, int){ return 0;}
size_t Stream::write(const uint8_t *buffer, size_t size){ return size;};

SerialUsb::operator bool() { return true; }

void pinMode(pin_size_t pinNumber, PinMode pinMode) { }

void digitalWrite(pin_size_t pin, PinStatus val) { }

void delay(int milliSeconds) { }

SerialUsb Serial;
SerialUsb Serial1;