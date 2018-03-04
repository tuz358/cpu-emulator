#include <stdio.h>
#include <stdlib.h>

class Memory {
private:
  uint8_t *memory;
  size_t memsize;
public:
  uint8_t *init(size_t memorysize);
  uint8_t read_uint8(uint32_t addr);
  void write_uint8(uint32_t addr, uint8_t data);
  size_t get_memsize();
  void load_binary(FILE *file);
  void free_memory();
};
