#pragma once
#include "esphome/components/bufferex_base/bufferex_base.h"

namespace esphome {
namespace bufferex_565 {

class Bufferex565 : public bufferex_base::BufferexBase {
 public:
  uint16_t *buffer_{nullptr};
  void init_buffer(int width, int height) override;
  void set_pixel(int x, int y, Color color) override;
  uint16_t get_pixel_to_565(int x, int y) override;
};  // class Bufferex565
}  // namespace bufferex_565
}  // namespace esphome
