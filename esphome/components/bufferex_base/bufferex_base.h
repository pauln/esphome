#pragma once
#include "esphome/core/color.h"
#include "esphome/core/log.h"
namespace esphome {
namespace bufferex_base {

enum BufferType : uint8_t {
  BUFFER_TYPE_332 = 0,
  BUFFER_TYPE_565 = 1,
  BUFFER_TYPE_1BIT = 2,
};

class BufferexBase {
 public:
  
  virtual void init_buffer(int width, int height) = 0;
  virtual void set_pixel(int x, int y, Color color) = 0;
  virtual uint16_t get_pixel_to_565(int x, int y) = 0;
  virtual size_t get_buffer_length();

  size_t BufferexBase::get_to_write_field_length();
  bufferex_base::BufferType get_buffer_type() { return buffer_type_; };

 protected:
  int16_t width_ = 0, height_ = 0;
  uint32_t to_write_field_ = 0;

  uint16_t get_pixel_buffer_position_(int x, int y) { return (x + y * width_); }
  bufferex_base::BufferType buffer_type_;

};  // class BufferexBase
}  // namespace bufferex_base
}  // namespace esphome
