#include "esphome/components/bufferex_332/bufferex_332.h"

namespace esphome {
namespace bufferex_332 {
static const char *TAG = "bufferex_332";

void Bufferex332::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;
  this->buffer_type_ = bufferex_base::BufferType::BUFFER_TYPE_332;

  this->buffer_ = new uint8_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_length());
}

void HOT Bufferex332::set_pixel(int x, int y, Color color) {
  if (x >= this->width_ || x < 0 || y >= this->height_ || y < 0)
    return;

  const uint16_t pos = get_pixel_buffer_position_(x, y);
  const uint8_t color332 = color.to_332();

  this->buffer_[pos] = color332;
}

uint16_t Bufferex332::get_pixel_to_565(int x, int y) {
  const uint16_t pos = get_pixel_buffer_position_(x, y);
  return Color(this->buffer_[pos], Color::ColorOrder::COLOR_ORDER_RGB, Color::ColorBitness::COLOR_BITNESS_332, true)
      .to_565();
}
}  // namespace bufferex_332
}  // namespace esphome
