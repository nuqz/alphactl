#include <stdexcept>

#include "mocks/driver/gpio.h"

namespace MockGPIO
{
    std::unordered_map<int, gpio_config_t> configured_pins;
    std::unordered_map<int, int> pin_levels; // 0 = LOW, 1 = HIGH

    void reset()
    {
        configured_pins.clear();
        pin_levels.clear();
    }

    int get_level(gpio_num_t pin)
    {
        return pin_levels.count(pin) ? pin_levels[pin] : 0;
    }

    void set_level(gpio_num_t pin, int level)
    {
        pin_levels[pin] = (level != 0) ? 1 : 0;
    }

    const gpio_config_t *get_config(gpio_num_t pin)
    {
        auto it = configured_pins.find(pin);
        return (it != configured_pins.end()) ? &it->second : nullptr;
    }
}

esp_err_t gpio_config(const gpio_config_t *cfg)
{
    if (!cfg)
        return ESP_ERR_INVALID_ARG;
    if (cfg->pin_bit_mask == 0)
        return ESP_ERR_INVALID_ARG;

    for (int i = 0; i < GPIO_NUM_MAX; ++i)
    {
        if (cfg->pin_bit_mask & (1ULL << i))
        {
            MockGPIO::configured_pins[i] = *cfg;
            if (cfg->mode == GPIO_MODE_INPUT)
            {
                MockGPIO::pin_levels[i] = 1;
            }
            break;
        }
    }
    return ESP_OK;
}

int gpio_get_level(gpio_num_t pin)
{
    return MockGPIO::get_level(pin);
}

esp_err_t gpio_set_level(gpio_num_t pin, int level)
{
    MockGPIO::set_level(pin, level);
    return ESP_OK;
}

esp_err_t gpio_reset_pin(gpio_num_t pin)
{
    MockGPIO::configured_pins.erase(pin);
    MockGPIO::pin_levels.erase(pin);
    return ESP_OK;
}

void mock_gpio_reset()
{
    MockGPIO::reset();
}

void mock_gpio_set_level(gpio_num_t pin, int level)
{
    MockGPIO::set_level(pin, level);
}

int mock_gpio_get_level(gpio_num_t pin)
{
    return MockGPIO::get_level(pin);
}
