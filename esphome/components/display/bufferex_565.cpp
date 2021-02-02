#include "bufferex_565.h"

namespace esphome {
namespace display {
static const char *TAG = "bufferex_565";

void Bufferex565::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;
  this->buffer_type_ = display::BufferType::BUFFER_TYPE_565;

  this->buffer_ = new uint16_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_length());
}

void Bufferex565::fill_buffer(Color color) {
  display::BufferexBase::fill_buffer(color);

  auto color565 = color.to_565();
  ESP_LOGD(TAG, "fill_buffer color: %d", color565);
  memset(this->buffer_, color565, this->get_buffer_length());
}
size_t Bufferex565::get_buffer_size() { return this->get_buffer_length() * 2; }

void HOT Bufferex565::set_buffer(int x, int y, Color color) {
  uint16_t pos = get_pixel_buffer_position_(x, y);
  const uint16_t color565 = color.to_565();
  this->buffer_[pos] = color565;
}

uint16_t Bufferex565::get_pixel_to_565(int x, int y) {
  const uint16_t pos = get_pixel_buffer_position_(x, y);
  return this->get_pixel_to_565(pos);
}

uint16_t Bufferex565::get_pixel_to_565(uint16_t pos) { return this->buffer_[pos]; }

}  // namespace display
}  // namespace esphome
