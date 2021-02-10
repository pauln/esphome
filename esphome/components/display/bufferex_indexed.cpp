#include "bufferex_indexed.h"

namespace esphome {
namespace display {
static const char *TAG = "bufferex_indexed";

void BufferexIndexed::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;

  this->buffer_ = new uint8_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_size());
}

void BufferexIndexed::fill_buffer(Color color) {
  display::BufferexBase::fill_buffer(color);
  ESP_LOGD(TAG, "fill_buffer color: %d", color.to_565());
  memset(this->buffer_, color.r + color.b + color.g == 0 ? 0 : 1, this->get_buffer_size());
}

void HOT BufferexIndexed::set_buffer(int x, int y, Color color) {
  const uint16_t pixel_bit_start = pos * this->pixel_storage_size_;
  const uint16_t pixel_bit_end = pixel_bit_start + this->pixel_storage_size_;

  const uint16_t byte_location_start = pixel_bit_start / 8;
  const uint16_t byte_location_end = pixel_bit_end / 8;

  uint8_t index_byte_start = this->buffer_[byte_location_start];
  const uint8_t byte_offset_start = pixel_bit_start - (byte_location_start * 8);

  // const uint16_t byte_location = get_pixel_buffer_position_internal_(x, y) / 8;
  // const uint8_t byte_offset = get_pixel_buffer_position_internal_(x, y) - (byte_location * 8);

  // uint8_t color_byte = this->buffer_[byte_location];
  // color_byte ^= (-(color.r + color.b + color.g == 0 ? 0 : 1) ^ color_byte) & (1UL << byte_offset);
  // this->buffer_[byte_location] = color_byte;
}

uint8_t BufferexIndexed::get_index_value_(int x, int y) { return this->get_index_value_((x + y * width_)); }

uint8_t BufferexIndexed::get_index_value_(uint16_t pos) {
  const uint16_t pixel_bit_start = pos * this->pixel_storage_size_;
  const uint16_t pixel_bit_end = pixel_bit_start + this->pixel_storage_size_;

  const uint16_t byte_location_start = pixel_bit_start / 8;
  const uint16_t byte_location_end = pixel_bit_end / 8;

  uint8_t index_byte_start = this->buffer_[byte_location_start];
  const uint8_t byte_offset_start = pixel_bit_start - (byte_location_start * 8);

  index_byte_start = (index_byte_start >> byte_offset_start);

  if (byte_location_start == byte_location_end) {  // Index is in the same byte
    const uint8_t mask = 0xFF >> (8 - pixel_storage_size_);
    index_byte_start = (index_byte_start & mask);
    return index_byte_start;
  }
  const uint8_t byte_offset_end = pixel_bit_end - (byte_location_end * 8);
  const uint8_t mask = 0xFF >> (8 - byte_offset_end);
  uint8_t index_byte_end = this->buffer_[byte_location_end];
  index_byte_end = (index_byte_end & mask) << (pixel_storage_size_ - byte_offset_end);
  index_byte_end = index_byte_end | index_byte_start;

  return index_byte_end;
}

uint16_t BufferexIndexed::get_pixel_to_565(int x, int y) {
  // uint8_t color_bit = this->get_color_bit_(x, y);

  // if (color_bit == 0)
  //   return this->color_off_.to_565();

  return this->get_index_value_(x, y);
}

uint16_t BufferexIndexed::get_pixel_to_565(uint16_t pos) {
  // uint8_t color_bit = this->get_color_bit_(pos);

  // if (color_bit == 0)
  //   return this->color_off_.to_565();

  return this->get_index_value_(pos);
}

uint32_t BufferexIndexed::get_pixel_to_666(int x, int y) {
  // uint8_t color_bit = this->get_color_bit_(x, y);

  // if (color_bit == 0)
  //   return this->color_off_.to_666();

  return this->get_index_value_(x, y);
}

uint32_t BufferexIndexed::get_pixel_to_666(uint16_t pos) {
  // uint8_t color_bit = this->get_color_bit_(pos);

  // if (color_bit == 0)
  //   return this->color_off_.to_666();

  return this->get_index_value_(pos);
}

size_t BufferexIndexed::get_buffer_length() {  // How many unint8_t bytes does the buffer need
  if (this->get_buffer_length_ != 0)
    return this->get_buffer_length_;

  auto total_pixels = size_t(this->width_) * size_t(this->height_);

  if (this->index_size_ <= 1) {
    this->pixel_storage_size_ = 1;
  } else {
    this->pixel_storage_size_ = std::ceil(std::log(this->index_size_) / std::log(2));
  }

  auto screensize = total_pixels * pixel_storage_size_;

  auto bufflength = (screensize % 8) ? screensize / 8 + 1 : screensize / 8;

  ESP_LOGD(TAG, "Pixel index size: %zu", this->index_size_);
  ESP_LOGD(TAG, "Total Pixels: %zu", total_pixels);
  ESP_LOGD(TAG, "Pixel Storage Size: %d", this->pixel_storage_size_);
  ESP_LOGD(TAG, "Buffer length %zu", bufflength);

  this->get_buffer_length_ = bufflength;

  return bufflength;
}

size_t BufferexIndexed::get_buffer_size() { return this->get_buffer_length(); }

}  // namespace display
}  // namespace esphome
