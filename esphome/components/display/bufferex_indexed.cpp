#include "bufferex_indexed.h"

namespace esphome {
namespace display {
static const char *TAG = "bufferex_indexed";

void BufferexIndexed::init_buffer(int width, int height) {
  this->width_ = width;
  this->height_ = height;

#ifdef ARDUINO_ARCH_ESP32
  uint8_t *psrambuffer = (uint8_t *) malloc(1);  // NOLINT

  if (psrambuffer == nullptr) {
    ESP_LOGW(TAG, "PSRAM is NOT supported");
  } else {
    ESP_LOGW(TAG, "PSRAM is supported");
    ESP_LOGW(TAG, "Total heap: %d", ESP.getHeapSize());
    ESP_LOGW(TAG, "Free heap: %d", ESP.getFreeHeap());
    ESP_LOGW(TAG, "Total PSRAM: %d", ESP.getPsramSize());
    ESP_LOGW(TAG, "Free PSRAM: %d", ESP.getFreeHeap());
  }
#endif
  this->buffer_ = new uint8_t[this->get_buffer_length()];
  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    return;
  }
  memset(this->buffer_, 0x00, this->get_buffer_size());
}

void BufferexIndexed::fill_buffer(Color color) {
  display::BufferexBase::fill_buffer(color);

  ESP_LOGD(TAG, "fill_buffer %d", color.b);
  for (uint16_t x = 0; x < this->height_; ++x) {
    for (uint16_t y = 0; y < this->width_; y++) {
      this->set_buffer(x, y, color.b);
    }
  }
}

void HOT BufferexIndexed::set_buffer(int x, int y, Color color) {
  uint16_t pos = (x + y * this->width_);
  bool debug = false;
  // pos >= 40 && pos < 48;

  uint8_t index = color.b;

  const uint16_t pixel_bit_start = pos * this->pixel_storage_size_;
  const uint16_t pixel_bit_end = pixel_bit_start + this->pixel_storage_size_;

  const uint16_t byte_location_start = pixel_bit_start / 8;
  const uint16_t byte_location_end = pixel_bit_end / 8;

  const uint8_t byte_offset_start = pixel_bit_start - (byte_location_start * 8);
  uint8_t index_byte_start = this->buffer_[byte_location_start];

  uint8_t mask = ((1 << pixel_storage_size_) - 1) << byte_offset_start;
  if (debug)
    ESP_LOGD(TAG, " pixel_storage_size_: %d byte_offset_start: %d  mask: %d index_byte_start: %d", pixel_storage_size_,
             byte_offset_start, mask, index_byte_start);

  index_byte_start = (index_byte_start & ~mask) | ((index << byte_offset_start) & mask);

  this->buffer_[byte_location_start] = index_byte_start;

  if (byte_location_start == byte_location_end) {  // Index is in the same byte
    if (debug)
      ESP_LOGD(TAG,
               "set_buffer x %d pos %d value %d byte_location_start %d byte_location_end %d byte_offset_start %d "
               "index_byte_start %d",
               x, pos, color.b, byte_location_start, byte_location_end, byte_offset_start, index_byte_start);
    return;
  }
  const uint8_t byte_offset_end = pixel_bit_end - (byte_location_end * 8);
  uint8_t index_byte_end = this->buffer_[byte_location_end];
  mask = (((uint8_t) 1 << byte_offset_end) - 1) << (byte_offset_end - 1);
  index_byte_end = (index_byte_end & ~mask) | ((index << (byte_offset_end - 1)) & mask);
  this->buffer_[byte_location_end] = index_byte_end;
  if (debug)
    ESP_LOGD(TAG,
             "set_buffer x %d pos %d value %d byte_location_start %d byte_location_end %d "
             "byte_offset_start %d byte_offset_end %d index_byte_start %d "
             "index_byte_end %d",
             x, pos, color.b, byte_location_start, byte_location_end, byte_offset_start, byte_offset_end,
             index_byte_start, index_byte_end);
}

uint8_t BufferexIndexed::get_index_value_(int x, int y) { return this->get_index_value_((x + y * this->width_)); }

uint8_t BufferexIndexed::get_index_value_(uint16_t pos) {
  bool debug = false;
  // pos % this->width_ == 0;

  const uint16_t pixel_bit_start = pos * this->pixel_storage_size_;
  const uint16_t pixel_bit_end = pixel_bit_start + this->pixel_storage_size_;

  const uint16_t byte_location_start = pixel_bit_start / 8;
  const uint16_t byte_location_end = pixel_bit_end / 8;

  uint8_t index_byte_start = this->buffer_[byte_location_start];
  const uint8_t byte_offset_start = pixel_bit_start - (byte_location_start * 8);

  index_byte_start = (index_byte_start >> byte_offset_start);

  if (byte_location_start == byte_location_end) {  // Index is in the same byte
    const uint8_t mask = 0xFF >> (8 - this->pixel_storage_size_);
    index_byte_start = (index_byte_start & mask);

    if (debug)
      ESP_LOGD(TAG, "get_index_value_ byte_location_start %d byte_location_end %d index_byte_start %d",
               byte_location_start, byte_location_end, index_byte_start);
    return index_byte_start;
  }
  const uint8_t byte_offset_end = pixel_bit_end - (byte_location_end * 8);
  const uint8_t mask = 0xFF >> (8 - byte_offset_end);
  uint8_t index_byte_end = this->buffer_[byte_location_end];
  index_byte_end = (index_byte_end & mask) << (this->pixel_storage_size_ - byte_offset_end);
  index_byte_end = index_byte_end | index_byte_start;

  if (debug)
    ESP_LOGD(TAG, "get_index_value_ byte_location_start %d byte_location_end %d index_byte_start %d index_byte_end %d",
             byte_location_start, byte_location_end, index_byte_start, index_byte_end);

  return index_byte_end;
}

uint16_t BufferexIndexed::get_pixel_to_565(int x, int y) {
  uint8_t value = this->get_index_value_(x, y);

  if (value > this->index_size_)
    value = 0;

  return this->colors_[value].to_565(this->driver_right_bit_aligned_);
}

uint16_t BufferexIndexed::get_pixel_to_565(uint16_t pos) {
  uint8_t value = this->get_index_value_(pos);

  if (value > this->index_size_)
    value = 0;

  return this->colors_[value].to_565(this->driver_right_bit_aligned_);
}

uint32_t BufferexIndexed::get_pixel_to_666(int x, int y) {
  uint8_t value = this->get_index_value_(x, y);

  if (x == 0)
    ESP_LOGD(TAG, "Got value %d from x %d, y %d", value, x, y);

  if (value > this->index_size_)
    value = 0;

  return this->colors_[value].to_666(this->driver_right_bit_aligned_);
}

uint32_t BufferexIndexed::get_pixel_to_666(uint16_t pos) {
  uint8_t value = this->get_index_value_(pos);

  if (value > this->index_size_)
    value = 0;

  // if (pos % this->width_ == 0)
  //   ESP_LOGD(TAG, "Got value %d from pos %d, R:%d G:%d B:%d", value, pos, this->colors_[value].r,
  //            this->colors_[value].g, this->colors_[value].b);

  return this->colors_[value].to_666(this->driver_right_bit_aligned_);
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

  ESP_LOGD(TAG, "Pixel index size: %hhu", this->index_size_);
  ESP_LOGD(TAG, "Total Pixels: %zu", total_pixels);
  ESP_LOGD(TAG, "Pixel Storage Size: %d", this->pixel_storage_size_);
  ESP_LOGD(TAG, "Buffer length %zu", bufflength);

  this->get_buffer_length_ = bufflength;

  return bufflength;
}

size_t BufferexIndexed::get_buffer_size() { return this->get_buffer_length(); }

}  // namespace display
}  // namespace esphome
