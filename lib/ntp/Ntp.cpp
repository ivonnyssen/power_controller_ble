#if defined(ARDUINO)
#include "Ntp.h"
// send an NTP request to the time server at the given address
void Ntp::sendPacket(const char * address) {
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

time_t Ntp::getTime() {
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    sendPacket(ntpServer);
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

void Ntp::begin() {
    Udp.begin(localPort);
    delay(2000);
    set_time(getTime());
}

bool Ntp::syncTime() {
    time_t updated = getTime();
    if(updated != 0){
        set_time(updated);
        return true;
    }
    return false;
}

Ntp::Ntp(int aTimeZone) {
    Ntp::timeZone = aTimeZone;
    lastSyncTime = 0;
}

#endif //ARDUINO