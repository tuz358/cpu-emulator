#include "include/memory.h"

uint8_t *Memory::init(size_t memorysize){
  this->memsize = memorysize;
  this->memory = (uint8_t *)malloc(memsize);

  return this->memory;
}

uint8_t Memory::read_uint8(uint32_t addr){
  return this->memory[addr];
}

size_t Memory::get_memsize(){
  return this->memsize;
}

void  Memory::load_binary(FILE *file){
  // fread(this->memory, size_t __size, size_t __nitems, file);
  fread(this->memory, 1, 0x200, file);
}

void Memory::free_memory(){
  free(this->memory);
}
