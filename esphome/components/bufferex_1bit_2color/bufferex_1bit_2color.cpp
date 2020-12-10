#include "esphome/components/bufferex_1bit_2color/bufferex_1bit_2color.h"

namespace esphome {
namespace bufferex_1bit_2color {
static const char *TAG = "bufferex_1bit_2color";

// void Bufferex1bit2color::init_buffer(int width, int height) {
//   BufferexBase::init_buffer_with_size(width, height, bufferex_base::BufferType::BUFFER_TYPE_1BIT);
// }

void Bufferex1bit2color::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;
  this->buffer_type_ = bufferex_base::BufferType::BUFFER_TYPE_1BIT;

  this->buffer_ = new uint8_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_length());
  // bufferex_base::BufferexBase::setup();
}

uint16_t Bufferex1bit2color::get_pixel_buffer_position_internal_(int x, int y) { return (x + y * this->width_); }

void HOT Bufferex1bit2color::set_pixel(int x, int y, Color color) {
  if (x >= this->width_ || x < 0 || y >= this->height_ || y < 0)
    return;

  const uint16_t byte_location = get_pixel_buffer_position_internal_(x, y) / 8;
  const uint8_t byte_offset = get_pixel_buffer_position_internal_(x, y) - (byte_location * 8);

  uint8_t color_byte = this->buffer_[byte_location];
  color_byte ^= (-(color.r + color.b + color.g == 0 ? 0 : 1) ^ color_byte) & (1UL << byte_offset);
  this->buffer_[byte_location] = color_byte;
}

uint16_t Bufferex1bit2color::get_pixel_to_565(int x, int y) {
  const uint16_t byte_location = get_pixel_buffer_position_internal_(x, y) / 8;
  const uint8_t byte_offset = get_pixel_buffer_position_internal_(x, y) - (byte_location * 8);

  auto color_byte = this->buffer_[byte_location];
  auto color_bit = (color_byte >> byte_offset) & 1U;

  if (color_bit == 0)
    return this->color_off_.to_565();

  return this->color_on_.to_565();
}

size_t Bufferex1bit2color::get_buffer_length() {
  auto screensize = size_t(this->width_) * size_t(this->height_);
  auto buffsize = (screensize % 8) ? screensize / 8 + 1 : screensize / 8;
  return buffsize;
}

}  // namespace bufferex_1bit_2color
}  // namespace esphome
