#pragma once

#include <cstdint>
#include <unordered_map>

#include "../esp/err.h"
#include "gpio_types.h"

namespace MockGPIO
{
    extern std::unordered_map<int, gpio_config_t> configured_pins;
    extern std::unordered_map<int, int> pin_levels;

    void reset();
    int get_level(gpio_num_t pin);
    void set_level(gpio_num_t pin, int level);
    const gpio_config_t *get_config(gpio_num_t pin);
}

esp_err_t gpio_config(const gpio_config_t *cfg);
int gpio_get_level(gpio_num_t pin);
esp_err_t gpio_set_level(gpio_num_t pin, int level);
esp_err_t gpio_reset_pin(gpio_num_t pin);

void mock_gpio_reset();
void mock_gpio_set_level(gpio_num_t pin, int level);
int mock_gpio_get_level(gpio_num_t pin);
