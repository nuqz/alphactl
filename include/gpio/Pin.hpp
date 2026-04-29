#pragma once

#include <cstdint>

#include "driver.h"

/**
 * @brief Base structure containing common GPIO enumerations.
 */
struct PinBase
{
    /**
     * @brief Enumeration for GPIO pull resistor configuration.
     *
     * Controls the internal pull-up/pull-down resistors of the GPIO pin.
     * These settings are used to define the default state of a pin when not actively driven.
     *
     * @var None
     * No internal pull resistor. The pin floats when not driven externally.
     *
     * @var Up
     * Enable internal pull-up resistor. Pulls the pin to VDD (logic HIGH) when not driven.
     * Commonly used for buttons connected to ground.
     *
     * @var Down
     * Enable internal pull-down resistor. Pulls the pin to GND (logic LOW) when not driven.
     * Commonly used for buttons connected to VDD.
     */
    enum class PullMode : uint8_t
    {
        None = 0,
        Up,
        Down,
    };
};

template <gpio_num_t PinNumber>
class Pin : PinBase
{
public:
    static_assert(PinNumber != GPIO_NUM_NC && PinNumber <= GPIO_NUM_MAX,
                  "Invalid pin number");
    static constexpr gpio_num_t num = PinNumber;

    inline void deinit() noexcept { ESP_ERROR_CHECK(gpio_reset_pin(num)); }

protected:
    static constexpr uint64_t mask = (1ULL << num);

    constexpr Pin() = default;
};
