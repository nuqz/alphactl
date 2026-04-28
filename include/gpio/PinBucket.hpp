#pragma once

#include "driver.h"
#include "Pin.hpp"
#include "utils/BitBucket.hpp"

class PinBucket final
{
public:
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
    static BitBucket<uint32_t, GPIO_NUM_MAX> _pin_bucket;

    static void _register_pin(gpio_num_t num) noexcept;
};
