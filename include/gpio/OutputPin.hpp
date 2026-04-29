#pragma once

#include "Pin.hpp"

template <gpio_num_t PinNumber,
          PinBase::PullMode PullMode = PinBase::PullMode::None,
          bool OpenDrain = false>
class OutputPin final : public Pin<PinNumber>
{
public:
    constexpr OutputPin() : Pin<PinNumber>() {}

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

    inline void set_level(int level) noexcept { ESP_ERROR_CHECK(gpio_set_level(this->num, level)); }
};
