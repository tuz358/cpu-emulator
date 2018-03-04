#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

class Instructions{
private:
  void init_instructions();
public:
  Memory memory;
  uint32_t registers[8];        // eax, ecx, edx, ebx, esp, ebp, esi, edi
  uint32_t eflags;              // EFLAGS register
  uint32_t eip;                 // Instruction pointer

  void (Instructions::*instructions[256])(void);

  void init(uint32_t eip, uint32_t esp, Memory memory);
  void execute_opcode(uint8_t opcode);

  void nop();
  void hlt();
};
