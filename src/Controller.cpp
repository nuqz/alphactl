#include "Controller.hpp"

#include <esp_log.h>

Controller::Controller()
{
    ESP_LOGI(TAG, "constructor called");
}

void Controller::run()
{
    ESP_LOGI(TAG, "Running in normal mode");
}

