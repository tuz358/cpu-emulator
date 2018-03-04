#include "include/utils.h"

uint32_t swap_endian32(uint32_t data){
  uint32_t swapped = 0x00;

  swapped += (data & 0xff000000) >> 24;
  swapped += (data & 0x00ff0000) >>  8;
  swapped += (data & 0x0000ff00) <<  8;
  swapped += (data & 0x000000ff) << 24;

  return swapped;
}
