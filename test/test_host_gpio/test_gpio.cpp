#include <unity.h>

#include "gpio/InputPin.hpp"
#include "gpio/OutputPin.hpp"
#include "gpio/PinBucket.hpp"
#include "utils/err.h"

void setUp()
{
    MockGPIO::reset();
}

void tearDown()
{
    MockGPIO::reset();
}

/**
 * @brief Test InputPin initialization and level reading
 *
 * Verifies that:
 * - InputPin can be constructed with default parameters
 * - init() properly configures the GPIO pin
 * - get_level() returns the correct pin state
 */
void test_input_pin()
{
    // Create input pin with pull-up resistor
    InputPin<GPIO_NUM_5, PinBase::PullMode::Up> input_pin;

    // Initialize the pin
    input_pin.init();

    // Verify pin was configured correctly
    const gpio_config_t *config = MockGPIO::get_config(GPIO_NUM_5);
    TEST_ASSERT_NOT_NULL(config);
    TEST_ASSERT_EQUAL(GPIO_MODE_INPUT, config->mode);
    TEST_ASSERT_EQUAL(GPIO_PULLUP_ENABLE, config->pull_up_en);
    TEST_ASSERT_EQUAL(GPIO_PULLDOWN_DISABLE, config->pull_down_en);
    TEST_ASSERT_EQUAL(GPIO_INTR_DISABLE, config->intr_type);

    // Test reading pin level (default should be HIGH due to pull-up)
    int level = input_pin.get_level();
    TEST_ASSERT_EQUAL(1, level);

    // Set pin to LOW externally and verify reading
    MockGPIO::set_level(GPIO_NUM_5, 0);
    level = input_pin.get_level();
    TEST_ASSERT_EQUAL(0, level);

    // Set pin to HIGH externally and verify reading
    MockGPIO::set_level(GPIO_NUM_5, 1);
    level = input_pin.get_level();
    TEST_ASSERT_EQUAL(1, level);
}

/**
 * @brief Test InputPin with different configurations
 *
 * Tests various pull modes and interrupt types
 */
void test_input_pin_configurations()
{
    // Test with pull-down
    InputPin<GPIO_NUM_4, PinBase::PullMode::Down> input_pd;
    input_pd.init();

    const gpio_config_t *config_pd = MockGPIO::get_config(GPIO_NUM_4);
    TEST_ASSERT_NOT_NULL(config_pd);
    TEST_ASSERT_EQUAL(GPIO_PULLDOWN_ENABLE, config_pd->pull_down_en);
    TEST_ASSERT_EQUAL(GPIO_PULLUP_DISABLE, config_pd->pull_up_en);

    // Test with no pull
    InputPin<GPIO_NUM_3, PinBase::PullMode::None> input_none;
    input_none.init();

    const gpio_config_t *config_none = MockGPIO::get_config(GPIO_NUM_3);
    TEST_ASSERT_NOT_NULL(config_none);
    TEST_ASSERT_EQUAL(GPIO_PULLUP_DISABLE, config_none->pull_up_en);
    TEST_ASSERT_EQUAL(GPIO_PULLDOWN_DISABLE, config_none->pull_down_en);
}

/**
 * @brief Test OutputPin initialization and level setting
 *
 * Verifies that:
 * - OutputPin can be constructed with default parameters
 * - init() properly configures the GPIO pin
 * - set_level() correctly sets the pin state
 */
void test_output_pin()
{
    // Create output pin
    OutputPin<GPIO_NUM_18> output_pin;

    // Initialize the pin
    output_pin.init();

    // Verify pin was configured correctly
    const gpio_config_t *config = MockGPIO::get_config(GPIO_NUM_18);
    TEST_ASSERT_NOT_NULL(config);
    TEST_ASSERT_EQUAL(GPIO_MODE_OUTPUT, config->mode);
    TEST_ASSERT_EQUAL(GPIO_PULLUP_DISABLE, config->pull_up_en);
    TEST_ASSERT_EQUAL(GPIO_PULLDOWN_DISABLE, config->pull_down_en);

    // Test setting pin level to HIGH
    output_pin.set_level(1);
    int level = MockGPIO::get_level(GPIO_NUM_18);
    TEST_ASSERT_EQUAL(1, level);

    // Test setting pin level to LOW
    output_pin.set_level(0);
    level = MockGPIO::get_level(GPIO_NUM_18);
    TEST_ASSERT_EQUAL(0, level);

    // Test setting pin level with non-zero value (should be HIGH)
    output_pin.set_level(5);
    level = MockGPIO::get_level(GPIO_NUM_18);
    TEST_ASSERT_EQUAL(1, level);
}

/**
 * @brief Test OutputPin with open-drain configuration
 *
 * Verifies open-drain mode configuration
 */
void test_output_pin_open_drain()
{
    // Create open-drain output pin with pull-up
    OutputPin<GPIO_NUM_21, PinBase::PullMode::Up, true> od_pin;
    od_pin.init();

    const gpio_config_t *config = MockGPIO::get_config(GPIO_NUM_21);
    TEST_ASSERT_NOT_NULL(config);
    TEST_ASSERT_TRUE(config->mode & GPIO_MODE_OUTPUT_OD);
    TEST_ASSERT_EQUAL(GPIO_PULLUP_ENABLE, config->pull_up_en);
}

/**
 * @brief Test PinBucket registration and duplicate prevention
 *
 * Verifies that:
 * - Pins can be registered through PinBucket
 * - Duplicate registration causes abort
 */
void test_pin_bucket()
{
    // Register an input pin through PinBucket
    auto input_pin = PinBucket::get(InputPin<GPIO_NUM_0, PinBase::PullMode::Up>{});
    input_pin.init();

    // Verify the pin is configured
    const gpio_config_t *config = MockGPIO::get_config(GPIO_NUM_0);
    TEST_ASSERT_NOT_NULL(config);
    TEST_ASSERT_EQUAL(GPIO_MODE_INPUT, config->mode);

    // Register an output pin through PinBucket
    auto output_pin = PinBucket::get(OutputPin<GPIO_NUM_1>{});
    output_pin.init();

    // Verify the output pin is configured
    config = MockGPIO::get_config(GPIO_NUM_1);
    TEST_ASSERT_NOT_NULL(config);
    TEST_ASSERT_EQUAL(GPIO_MODE_OUTPUT, config->mode);

    // Test that duplicate registration causes abort
    TEST_ASSERT_FALSE(MockErr::aborted); // Reset in setUp, should be false

    // Try to register duplicate pin - this should call abort() and set aborted flag
    (void)PinBucket::get(InputPin<GPIO_NUM_0>{});

    // Verify that abort was called
    TEST_ASSERT_TRUE(MockErr::aborted);
}

/**
 * @brief Test PinBucket with multiple different pins
 *
 * Verifies that multiple different pins can be registered
 */
void test_pin_bucket_multiple_pins()
{
    // Register multiple different pins
    auto pin1 = PinBucket::get(InputPin<GPIO_NUM_2>{});
    auto pin2 = PinBucket::get(OutputPin<GPIO_NUM_3>{});
    auto pin3 = PinBucket::get(InputPin<GPIO_NUM_4, PinBase::PullMode::Down>{});

    pin1.init();
    pin2.init();
    pin3.init();

    // Verify all pins are configured
    TEST_ASSERT_NOT_NULL(MockGPIO::get_config(GPIO_NUM_2));
    TEST_ASSERT_NOT_NULL(MockGPIO::get_config(GPIO_NUM_3));
    TEST_ASSERT_NOT_NULL(MockGPIO::get_config(GPIO_NUM_4));

    // Verify each pin type is configured correctly
    TEST_ASSERT_EQUAL(GPIO_MODE_INPUT, MockGPIO::get_config(GPIO_NUM_2)->mode);
    TEST_ASSERT_EQUAL(GPIO_MODE_OUTPUT, MockGPIO::get_config(GPIO_NUM_3)->mode);
    TEST_ASSERT_EQUAL(GPIO_MODE_INPUT, MockGPIO::get_config(GPIO_NUM_4)->mode);
}

/**
 * @brief Test integrated usage of InputPin and OutputPin
 *
 * Simulates a real-world scenario where an output controls an input
 */
void test_pin_integration()
{
    // Create output and input pins
    OutputPin<GPIO_NUM_10> output;
    InputPin<GPIO_NUM_11> input;

    output.init();
    input.init();

    // Set output HIGH and verify we can read it on input (simulated)
    output.set_level(1);
    MockGPIO::set_level(GPIO_NUM_11, 1); // Simulate connection
    TEST_ASSERT_EQUAL(1, input.get_level());

    // Set output LOW and verify
    output.set_level(0);
    MockGPIO::set_level(GPIO_NUM_11, 0); // Simulate connection
    TEST_ASSERT_EQUAL(0, input.get_level());
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_input_pin);
    RUN_TEST(test_input_pin_configurations);
    RUN_TEST(test_output_pin);
    RUN_TEST(test_output_pin_open_drain);
    RUN_TEST(test_pin_bucket);
    RUN_TEST(test_pin_bucket_multiple_pins);
    RUN_TEST(test_pin_integration);

    return UNITY_END();
}
