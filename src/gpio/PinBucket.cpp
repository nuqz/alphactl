#include "gpio/PinBucket.hpp"
#include "utils/log.h"

static const char *TAG = "PinBucket";

BitBucket<uint32_t, GPIO_NUM_MAX> PinBucket::_pin_bucket;

void PinBucket::_register_pin(gpio_num_t num) noexcept
{
    if (_pin_bucket.acquire(static_cast<uint32_t>(num)) != BitBucketBase::Result::Ok)
    {
        ESP_LOGE(TAG, "Pin %d already in use!", num);
        abort();
    }
}
