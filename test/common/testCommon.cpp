#if defined(UNIT_TEST)
#include <unity.h>
#include "testBms.h"
#include "testWebserver.h"

#if defined(ARDUINO)
void setup() {
    UNITY_BEGIN();
    testBmsAll();
    testWebServerAll();
    UNITY_END();
}

void loop() {

}
#else
int main(int argc, char **argv) {
    UNITY_BEGIN();
    testBmsAll();
    testWebServerAll();
    UNITY_END();

    return 0;
}
#endif

#endif