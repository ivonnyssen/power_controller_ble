#if defined(UNIT_TEST)

#include "testWebserver.h"
#include <unity.h>
#include <webserver.h>


void testStrEndsWithSuccess(){
    WebServer sut(nullptr, nullptr, nullptr, nullptr);

    TEST_ASSERT_TRUE(sut.strEndsWith("abc", "bc"))
}

void testStrEndsWithFailure(){
    WebServer sut(nullptr, nullptr, nullptr, nullptr);

    TEST_ASSERT_FALSE(sut.strEndsWith("abc", "a"))
}

void testStrEndsWithEmptySource(){
    WebServer sut(nullptr, nullptr, nullptr, nullptr);

    TEST_ASSERT_FALSE(sut.strEndsWith("", "a"))
}

void testStrEndsWithEmptyEnd(){
    WebServer sut(nullptr, nullptr, nullptr, nullptr);

    TEST_ASSERT_TRUE(sut.strEndsWith("abc", ""))
}

void testWebServerAll(){
    RUN_TEST(testStrEndsWithSuccess);
    RUN_TEST(testStrEndsWithFailure);
    RUN_TEST(testStrEndsWithEmptySource);
    RUN_TEST(testStrEndsWithEmptyEnd);
}

#endif