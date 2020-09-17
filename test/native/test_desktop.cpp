#if !defined(ARDUINO) && defined(UNIT_TEST)
#include <unity.h>
#include "testBms.h"

int main(int argc, char **argv) {
    UNITY_BEGIN();
    testBmsAll();
    UNITY_END();

    return 0;
}

#endif