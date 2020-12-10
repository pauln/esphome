#include "esphome/components/bufferex_565/bufferex_565.h"

namespace esphome {
namespace bufferex_565 {
static const char *TAG = "bufferex_565";

void Bufferex565::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;
  this->buffer_type_ = bufferex_base::BufferType::BUFFER_TYPE_565;

  this->buffer_ = new uint16_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_length());
  this->to_write_field_ = 0;
}

void HOT Bufferex565::set_pixel(int x, int y, Color color) {
  if (x >= this->width_ || x < 0 || y >= this->height_ || y < 0)
    return;

  uint16_t pos = get_pixel_buffer_position_(x, y);
  const uint16_t color565 = color.to_565();
  if (this->buffer_[pos] == color565) {
    ((this->to_write_field_) &= ~(1UL << (pos)));  // Clear the bit
    return;
  }
  ((this->to_write_field_) |= (1UL << (pos)));  // Set bit

  this->buffer_[pos] = color565;
}

uint16_t Bufferex565::get_pixel_to_565(int x, int y) {
  const uint16_t pos = get_pixel_buffer_position_(x, y);
  return this->buffer_[pos];
}
}  // namespace bufferex_565
}  // namespace esphome
