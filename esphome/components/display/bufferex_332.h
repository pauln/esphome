#pragma once
#include "bufferex_base.h"

namespace esphome {
namespace display {

class Bufferex332 : public display::BufferexBase {
 public:
  uint8_t *buffer_{nullptr};

  void init_buffer(int width, int height) override;
  void set_buffer(int x, int y, Color color) override;
  void fill_buffer(Color color) override;
  uint16_t get_pixel_to_565(int x, int y) override;
  uint16_t get_pixel_to_565(uint16_t pos) override;
  size_t get_buffer_size() override;

};  // class Bufferex332
}  // namespace display
}  // namespace esphome
