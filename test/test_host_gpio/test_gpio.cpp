#include <unity.h>

#include "gpio/InputPin.hpp"
#include "gpio/OutputPin.hpp"
#include "gpio/PinBucket.hpp"
#include "utils/log.h"

void setUp() {}
void tearDown() {}

void test_input_pin()
{
    // TODO
}

void test_output_pin()
{
    // TODO
}

void test_pin_bucket()
{
    // TODO
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_input_pin);
    RUN_TEST(test_output_pin);
    RUN_TEST(test_pin_bucket);

    return UNITY_END();
}
