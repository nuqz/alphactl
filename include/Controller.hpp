#pragma once

#include <cstdint>

// ============================================================================
// КОНФИГУРАЦИЯ ПИНОВ (изменить под вашу схему)
// ============================================================================
// Датчик (например, I2C или аналоговый) - пока заглушка
constexpr gpio_num_t SENSOR_SDA_PIN = GPIO_NUM_8;   // SDA для I2C
constexpr gpio_num_t SENSOR_SCL_PIN = GPIO_NUM_9;   // SCL для I2C

// PWM выходы для 4 транзисторов (используем LEDC периферию)
constexpr gpio_num_t PWM_PIN_0 = GPIO_NUM_10;
constexpr gpio_num_t PWM_PIN_1 = GPIO_NUM_11;
constexpr gpio_num_t PWM_PIN_2 = GPIO_NUM_12;
constexpr gpio_num_t PWM_PIN_3 = GPIO_NUM_13;

// ============================================================================
// КЛАСС CONTROLLER - главный управляющий класс устройства
// ============================================================================
class Controller
{
public:
    // Конструктор по умолчанию
    Controller();
    
    // Деструктор (важно для освобождения ресурсов)
    ~Controller();
    
    // Запуск основного цикла устройства
    void run();

private:
    // TAG для логирования - имя класса в логах
    static constexpr const char *TAG = "Controller";
    
    // Методы инициализации (будем реализовывать поэтапно)
    void initLogging();
    void initWifi();
    void initBluetooth();
    void initMQTT();
    void initSensor();
    void initPWM();
    
    // Основной цикл устройства
    void mainLoop();
};
