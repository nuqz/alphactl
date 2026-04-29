#pragma once

#include <utility>

#include "Pin.hpp"
#include "utils/BitBucket.hpp"

/**
 * @brief Singleton-like class for tracking GPIO pin usage across the application.
 *
 * PinBucket provides a centralized mechanism to prevent accidental reuse of GPIO pins.
 * It maintains a bit-field registry where each bit represents a GPIO pin's allocation status.
 * When a pin is registered through PinBucket::get(), it is marked as "in use".
 * Attempting to register an already-used pin will cause a runtime error (abort).
 *
 * @note This class uses a static BitBucket internally to track pin allocations.
 * @note The class follows RAII principles - pins are registered upon construction.
 * @note Thread-safety depends on the underlying BitBucket implementation.
 *
 * @example
 * ```cpp
 * // Register and use an input pin
 * auto button = PinBucket::get(InputPin<GPIO_NUM_5, PinBase::PullMode::Up>{});
 * button.init();
 *
 * // Register and use an output pin
 * auto led = PinBucket::get(OutputPin<GPIO_NUM_18>{});
 * led.init();
 * led.set_level(1);
 * ```
 *
 * @throws Aborts the program if attempting to register an already-registered pin.
 */
class PinBucket final
{
public:
    /**
     * @brief Register a GPIO pin and return it for use.
     *
     * This template method registers the specified pin in the internal registry
     * and returns the pin object for further use. If the pin is already registered,
     * the program will abort with an error message.
     *
     * @tparam T The pin type (must be derived from PinBase).
     * @param pin The pin object to register (passed by value for forwarding).
     * @return T The same pin object, forwarded for use.
     *
     * @note Uses perfect forwarding to preserve value category of the pin object.
     * @note Statically asserts that T is derived from PinBase at compile-time.
     * @throws Aborts the program if the pin is already in use.
     *
     * @example
     * ```cpp
     * auto sensor_pin = PinBucket::get(InputPin<GPIO_NUM_4>{});
     * sensor_pin.init();
     * ```
     */
    template <typename T>
    [[nodiscard]] static T get(T &&pin) noexcept
    {
        using RawT = std::decay_t<T>;
        static_assert(std::is_base_of<PinBase, RawT>::value,
                      "Invalid Pin subclass");

        _register_pin(pin.num);
        return std::forward<T>(pin);
    }

private:
    /// Static bit bucket to track which pins are currently in use.
    static BitBucket<uint32_t, GPIO_NUM_MAX> _pin_bucket;

    /**
     * @brief Internal method to register a pin number in the bucket.
     *
     * @param num The GPIO pin number to register.
     *
     * @note This method aborts the program if the pin is already registered.
     * @note Called automatically by get() during pin registration.
     */
    static void _register_pin(gpio_num_t num) noexcept;
};
