#include "esphome/components/bufferex_base/bufferex_base.h"

namespace esphome {
namespace bufferex_base {
static const char *TAG = "bufferex_base";

BufferexBase::BufferexBase(){}

size_t BufferexBase::get_buffer_length() { return size_t(this->width_) * size_t(this->height_); }



}  // namespace bufferex_base
}  // namespace esphome
