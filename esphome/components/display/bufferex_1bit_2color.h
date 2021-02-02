#pragma once
#include "bufferex_base.h"

namespace esphome {
namespace display {

class Bufferex1bit2color : public display::BufferexBase {
 public:
  uint8_t *buffer_{nullptr};

  void init_buffer(int width, int height) override;
  void set_buffer(int x, int y, Color color) override;
  void fill_buffer(Color color) override;
  uint16_t get_pixel_to_565(int x, int y) override;
  uint16_t get_pixel_to_565(uint16_t pos) override;
  size_t get_buffer_length() override;
  size_t get_buffer_size() override;

  void set_color_off(Color color) { color_off_ = color; }
  void set_color_on(Color color) { color_on_ = color; }

 protected:
  Color color_on_ = COLOR_WHITE;
  Color color_off_ = COLOR_BLACK;
  uint16_t get_pixel_buffer_position_internal_(int x, int y);
};  // class Bufferex332
}  // namespace display
}  // namespace esphome
