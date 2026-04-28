#pragma once

#include <cstdint>

#include "driver.h"

struct PinBase
{
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
