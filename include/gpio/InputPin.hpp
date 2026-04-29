#pragma once

#include "Pin.hpp"

/**
 * @brief Template class representing a GPIO input pin.
 *
 * InputPin is a strongly-typed, compile-time configured GPIO input pin wrapper.
 * It inherits from Pin<PinNumber> and provides type-safe access to input pin functionality.
 *
 * @tparam PinNumber The GPIO pin number (gpio_num_t enum value).
 * @tparam PullMode The pull-up/pull-down resistor configuration (default: None).
 *         See PinBase::PullMode for available options.
 * @tparam InterruptType The interrupt type configuration (default: GPIO_INTR_DISABLE).
 *         Configures the pin to trigger interrupts on specific edge/level conditions.
 *
 * @note This class uses CRTP (Curiously Recurring Template Pattern) through Pin base class.
 * @note All configuration is done at compile-time via template parameters.
 *
 * @example
 * ```cpp
 * // Create an input pin with pull-up resistor
 * InputPin<GPIO_NUM_5, PinBase::PullMode::Up> button;
 * button.init();
 * int state = button.get_level();
 * ```
 */
template <gpio_num_t PinNumber,
          PinBase::PullMode PullMode = PinBase::PullMode::None,
          gpio_int_type_t InterruptType = GPIO_INTR_DISABLE>
class InputPin final : public Pin<PinNumber>
{
public:
    /**
     * @brief Default constructor.
     *
     * Initializes the base Pin class. The pin is not configured for use until init() is called.
     */
    constexpr InputPin() : Pin<PinNumber>() {}

    /**
     * @brief Initialize the GPIO pin with the configured settings.
     *
     * Configures the pin as an input with the specified pull mode and interrupt type.
     * This method must be called before using the pin for reading.
     *
     * @note This function calls ESP_ERROR_CHECK and will abort if configuration fails.
     * @note Configuration is determined by template parameters at compile-time.
     */
    void init() noexcept
    {
        gpio_config_t cfg = {
            .pin_bit_mask = this->mask,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = (PullMode == PinBase::PullMode::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
            .pull_down_en = (PullMode == PinBase::PullMode::Down) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
            .intr_type = InterruptType};
        ESP_ERROR_CHECK(gpio_config(&cfg));
    };

    /**
     * @brief Read the current logic level of the pin.
     *
     * @return int The current pin level: 0 (LOW) or 1 (HIGH).
     *
     * @note This is a lightweight inline function suitable for frequent polling.
     * @note The pin must be initialized with init() before calling this method.
     */
    [[nodiscard]] inline int get_level() const noexcept { return gpio_get_level(this->num); }
};
