#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "utils.h"

const uint32_t ZF = 1 << 6;

class Instructions{
private:
  void init_instructions();
  void set_flag(int flag, uint32_t flag_type);
  int get_flag(uint32_t flag_type);
public:
  Memory memory;
  uint32_t registers[8];        // eax, ecx, edx, ebx, esp, ebp, esi, edi
  uint32_t eflags;              // EFLAGS register
  uint32_t eip;                 // Instruction pointer

  uint8_t modrm, mod, R, M;     // ModRM

  void (Instructions::*instructions[256])(void);

  void init(uint32_t eip, uint32_t esp, Memory memory);
  void init_modrm();
  void calc_modrm();
  void execute_opcode(uint8_t opcode);

  void add_rm32_r32();  // 0x01
  void xor_rm32_r32();  // 0x31
  void dec_ecx();       // 0x49
  void jne_imm8();      // 0x75
  void opcode_83();     // 0x83
  void mov_rm32_r32();  // 0x89
  void nop();           // 0x90
  void mov_ecx_imm32(); // 0xb9
  void mov_ebx_imm32(); // 0xbb
  void jmp_imm8();      // 0xeb
  void hlt();           // 0xf4
  void opcode_ff();     // 0xff

  void cmp_rm32_imm8(); // called by opcode_83
};
