#pragma once

#include "Pin.hpp"

/**
 * @brief Template class representing a GPIO output pin.
 *
 * OutputPin is a strongly-typed, compile-time configured GPIO output pin wrapper.
 * It inherits from Pin<PinNumber> and provides type-safe access to output pin functionality.
 *
 * @tparam PinNumber The GPIO pin number (gpio_num_t enum value).
 * @tparam PullMode The pull-up/pull-down resistor configuration (default: None).
 *         See PinBase::PullMode for available options.
 * @tparam OpenDrain Whether to configure the pin in open-drain mode (default: false).
 *         - false: Push-pull output mode (standard)
 *         - true: Open-drain output mode (allows multiple devices to share the line)
 *
 * @note This class uses CRTP (Curiously Recurring Template Pattern) through Pin base class.
 * @note All configuration is done at compile-time via template parameters.
 *
 * @example
 * ```cpp
 * // Create a standard output pin for an LED
 * OutputPin<GPIO_NUM_5> led;
 * led.init();
 * led.set_level(1);  // Turn on LED
 *
 * // Create an open-drain output for I2C SDA line
 * OutputPin<GPIO_NUM_21, PinBase::PullMode::Up, true> sda;
 * sda.init();
 * ```
 */
template <gpio_num_t PinNumber,
          PinBase::PullMode PullMode = PinBase::PullMode::None,
          bool OpenDrain = false>
class OutputPin final : public Pin<PinNumber>
{
public:
    /**
     * @brief Default constructor.
     *
     * Initializes the base Pin class. The pin is not configured for use until init() is called.
     */
    constexpr OutputPin() : Pin<PinNumber>() {}

    /**
     * @brief Initialize the GPIO pin with the configured settings.
     *
     * Configures the pin as an output with the specified pull mode and drive mode.
     * This method must be called before using the pin for writing.
     *
     * @note This function calls ESP_ERROR_CHECK and will abort if configuration fails.
     * @note Configuration is determined by template parameters at compile-time.
     */
    void init() noexcept
    {
        gpio_config_t cfg = {
            .pin_bit_mask = this->mask,
            .mode = OpenDrain ? GPIO_MODE_OUTPUT_OD : GPIO_MODE_OUTPUT,
            .pull_up_en = (PullMode == PinBase::PullMode::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
            .pull_down_en = (PullMode == PinBase::PullMode::Down) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};
        ESP_ERROR_CHECK(gpio_config(&cfg));
    }

    /**
     * @brief Set the logic level of the pin.
     *
     * @param level The desired pin level: 0 (LOW) or non-zero (HIGH).
     *
     * @note This is a lightweight inline function suitable for frequent toggling.
     * @note The pin must be initialized with init() before calling this method.
     */
    inline void set_level(int level) noexcept { ESP_ERROR_CHECK(gpio_set_level(this->num, level)); }
};
