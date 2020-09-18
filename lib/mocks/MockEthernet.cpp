#include "MockEthernet.h"

EthernetServer::EthernetServer(int listenPort){}
void EthernetServer::begin(){}
EthernetClient EthernetServer::available(){ return {};}

uint8_t IPAddress::operator[](int index) const { return _address[index]; }
IPAddress ip;

int EthernetClass::begin(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout) {return 1;}
IPAddress EthernetClass::localIP() { return ip;}

