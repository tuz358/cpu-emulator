#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class Instructions{
private:
  void init_instructions();
public:
  uint32_t eax, ecx, edx, ebx;  // General purpose
  uint32_t esp, ebp, esi, edi;  // registers
  uint32_t eflags;              // EFLAGS register
  uint32_t eip;                 // Instruction pointer

  void (Instructions::*instructions[256])(void);

  void init(uint32_t eip, uint32_t esp);
  void execute_opcode(uint8_t opcode);

  void nop();
  void hlt();
};
