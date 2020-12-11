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
  virtual void set_buffer(int x, int y, Color color) = 0;
  virtual uint16_t get_pixel_to_565(int x, int y) = 0;
  virtual uint16_t get_pixel_to_565(uint16_t pos) = 0;
  virtual size_t get_buffer_size() = 0;
  virtual size_t get_buffer_length();

  virtual void fill_buffer(Color color);

  void display();
  void set_pixel(int x, int y, Color color);
  bufferex_base::BufferType get_buffer_type() { return buffer_type_; };
  uint16_t x_low_ = 0;
  uint16_t y_low_ = 0;
  uint16_t x_high_ = 0;
  uint16_t y_high_ = 0;

 protected:
  int16_t width_ = 0, height_ = 0;

  uint16_t get_pixel_buffer_position_(int x, int y) { return (x + y * width_); }
  bufferex_base::BufferType buffer_type_;

};  // class BufferexBase
}  // namespace bufferex_base
}  // namespace esphome
