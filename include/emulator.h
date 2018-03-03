#define KB 1024
#define MB 1024*KB

#include <stdio.h>
#include <stdint.h>
#include "instructions.h"
#include "memory.h"

class Emulator{
public:
  Instructions instructions;
  Memory memory;

  void init(size_t memorysize, FILE *bin);
  void free();
  void exec(uint8_t opcode);
  void dump_registers();
  uint8_t read_next_opcode();
  uint32_t get_eip();
};
