#pragma once

#include "Pin.hpp"

template <gpio_num_t PinNumber,
          PinBase::PullMode PullMode = PinBase::PullMode::None,
          gpio_int_type_t InterruptType = GPIO_INTR_DISABLE>
class InputPin final : public Pin<PinNumber>
{
public:
    constexpr InputPin() : Pin<PinNumber>() {}

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

    inline int get_level() const noexcept { return gpio_get_level(this->num); }
    [[nodiscard]] inline int get_level() const noexcept { return gpio_get_level(this->num); }
};
