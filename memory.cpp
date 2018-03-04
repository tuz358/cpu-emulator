#include "include/memory.h"

uint8_t *Memory::init(size_t memorysize){
  this->memsize = memorysize;
  this->memory = (uint8_t *)malloc(memsize);

  return this->memory;
}

uint8_t Memory::read_uint8(uint32_t addr){
  return this->memory[addr];
}

uint32_t Memory::read_uint32(uint32_t addr){
  uint32_t data = 0x00;
  for (int i = 0;i < 4;i++) {
    data = data << 8;
    data += this->memory[addr + i];
  }
  return data;
}

void Memory::write_uint8(uint32_t addr, uint8_t data){
  this->memory[addr] = data;
}

void Memory::write_uint32(uint32_t addr, uint32_t data){
  this->memory[addr] = data;
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
