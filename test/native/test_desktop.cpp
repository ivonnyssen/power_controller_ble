#if !defined(ARDUINO) && defined(UNIT_TEST)
#include <unity.h>

int main(int argc, char **argv) {
    UNITY_BEGIN();
    UNITY_END();

    return 0;
}

#endif