
#pragma once

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

// Базовые типы
typedef int esp_err_t;

// Коды возврата (совместимы с оригиналом по значению)
#define ESP_OK 0
#define ESP_FAIL -1
#define ESP_ERR_NO_MEM 0x101
#define ESP_ERR_INVALID_ARG 0x102
#define ESP_ERR_INVALID_STATE 0x103
#define ESP_ERR_NOT_FOUND 0x104
#define ESP_ERR_NOT_SUPPORTED 0x105
#define ESP_ERR_TIMEOUT 0x106

// ESP_ERROR_CHECK заглушка
// В тестах бросает исключение, чтобы Catch2/Unity могли его перехватить
#define ESP_ERROR_CHECK(x)                                                \
    do                                                                    \
    {                                                                     \
        esp_err_t err_ = (x);                                             \
        if (err_ != ESP_OK)                                               \
        {                                                                 \
            fprintf(stderr, "ESP_ERROR_CHECK failed: err=0x%x (%s:%d)\n", \
                    err_, __FILE__, __LINE__);                            \
            throw std::runtime_error("ESP_ERROR_CHECK failed");           \
        }                                                                 \
    } while (0)
