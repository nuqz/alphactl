#pragma once

#include "../err.h" // esp_err_t, ESP_OK

#include <cstdint>
#include <unordered_map>

// Pin numbers
typedef int gpio_num_t;
#define GPIO_NUM_NC -1
#define GPIO_NUM_MAX 22
#define GPIO_NUM_0 0
#define GPIO_NUM_1 1
#define GPIO_NUM_2 2
#define GPIO_NUM_3 3
#define GPIO_NUM_4 4
#define GPIO_NUM_5 5
#define GPIO_NUM_6 6
#define GPIO_NUM_7 7
#define GPIO_NUM_8 8
#define GPIO_NUM_9 9
#define GPIO_NUM_10 10
#define GPIO_NUM_18 18

enum gpio_mode_t
{
    GPIO_MODE_DISABLE = 0,
    GPIO_MODE_INPUT = 1,
    GPIO_MODE_OUTPUT = 2,
    GPIO_MODE_OUTPUT_OD = 4,
    GPIO_MODE_INPUT_OUTPUT = 3,
    GPIO_MODE_INPUT_OUTPUT_OD = 5
};

enum gpio_pullup_t
{
    GPIO_PULLUP_DISABLE = 0,
    GPIO_PULLUP_ENABLE = 1
};

enum gpio_pulldown_t
{
    GPIO_PULLDOWN_DISABLE = 0,
    GPIO_PULLDOWN_ENABLE = 1
};

enum gpio_int_type_t
{
    GPIO_INTR_DISABLE = 0,
    GPIO_INTR_POSEDGE = 1,
    GPIO_INTR_NEGEDGE = 2,
    GPIO_INTR_ANYEDGE = 3,
    GPIO_INTR_LOW_LEVEL = 4,
    GPIO_INTR_HIGH_LEVEL = 5
};

// Структура конфига (точное соответствие оригиналу)
struct gpio_config_t
{
    uint64_t pin_bit_mask;        /*!< Bit mask of the pins */
    gpio_mode_t mode;             /*!< Mode of the pins */
    gpio_pullup_t pull_up_en;     /*!< Enable GPIO pull-up resistor */
    gpio_pulldown_t pull_down_en; /*!< Enable GPIO pull-down resistor */
    gpio_int_type_t intr_type;    /*!< Interrupt type */
};

// === Состояние мока (глобальное, для простоты) ===
namespace MockGPIO
{
    inline std::unordered_map<int, gpio_config_t> configured_pins;
    inline std::unordered_map<int, int> pin_levels; // 0 = LOW, 1 = HIGH
    inline bool aborted = false;

    inline void reset()
    {
        configured_pins.clear();
        pin_levels.clear();
        aborted = false;
        // Default: все пины в высоком импедансе → читаем как 1 (подтяжка)
    }

    inline int get_level(gpio_num_t pin)
    {
        return pin_levels.count(pin) ? pin_levels[pin] : 1;
    }

    inline void set_level(gpio_num_t pin, int level)
    {
        pin_levels[pin] = (level != 0) ? 1 : 0;
    }

    inline const gpio_config_t *get_config(gpio_num_t pin)
    {
        auto it = configured_pins.find(pin);
        return (it != configured_pins.end()) ? &it->second : nullptr;
    }
}

// === API заглушки (сигнатуры как в ESP-IDF) ===

inline esp_err_t gpio_config(const gpio_config_t *cfg)
{
    if (!cfg)
        return ESP_ERR_INVALID_ARG;
    if (cfg->pin_bit_mask == 0)
        return ESP_ERR_INVALID_ARG;

    // Извлекаем номер пина из маски (первый установленный бит)
    for (int i = 0; i < GPIO_NUM_MAX; ++i)
    {
        if (cfg->pin_bit_mask & (1ULL << i))
        {
            MockGPIO::configured_pins[i] = *cfg;
            // По умолчанию: вход → высокий уровень (подтяжка)
            if (cfg->mode == GPIO_MODE_INPUT)
            {
                MockGPIO::pin_levels[i] = 1;
            }
            break;
        }
    }
    return ESP_OK;
}

inline int gpio_get_level(gpio_num_t pin)
{
    return MockGPIO::get_level(pin);
}

inline esp_err_t gpio_set_level(gpio_num_t pin, int level)
{
    MockGPIO::set_level(pin, level);
    return ESP_OK;
}

inline esp_err_t gpio_reset_pin(gpio_num_t pin)
{
    MockGPIO::configured_pins.erase(pin);
    MockGPIO::pin_levels.erase(pin);
    return ESP_OK;
}

// === Утилиты для тестов ===
// Вызывать в setUp() каждого теста
inline void mock_gpio_reset() { MockGPIO::reset(); }
inline void mock_gpio_set_level(gpio_num_t pin, int level) { MockGPIO::set_level(pin, level); }
inline int mock_gpio_get_level(gpio_num_t pin) { return MockGPIO::get_level(pin); }

// Подмена abort() для тестов (бросает исключение вместо краха)
#undef abort
#define abort()                                             \
    do                                                      \
    {                                                       \
        MockGPIO::aborted = true;                           \
        throw std::runtime_error("abort() called in mock"); \
    } while (0)
