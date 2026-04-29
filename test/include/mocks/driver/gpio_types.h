#pragma once

#include <cstdint>

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
#define GPIO_NUM_11 11
#define GPIO_NUM_12 12
#define GPIO_NUM_13 13
#define GPIO_NUM_14 14
#define GPIO_NUM_15 15
#define GPIO_NUM_16 16
#define GPIO_NUM_17 17
#define GPIO_NUM_18 18
#define GPIO_NUM_19 19
#define GPIO_NUM_20 20
#define GPIO_NUM_21 21
#define GPIO_NUM_22 22

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
