#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "utils.h"

// eflags
const uint32_t CF = 1;
const uint32_t ZF = 1 << 6;

// used by opcode_81 and opcode_83
const int IMM8 = 0;
const int IMM32 = 1;

// calculation type
const int ADD = 0;
const int OR  = 1;
const int ADC = 2;
const int SBB = 3;
const int AND = 4;
const int SUB = 5;
const int XOR = 6;
const int CMP = 7;

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

  // templates
  void template_rm32_r32(int calc_type);
  void calc_rm32_r32_case0to2(uint32_t addr, uint32_t dst, int calc_type);
  void template_r32_rm32(int calc_type);
  void calc_r32_rm32(uint32_t *dst, uint32_t *src, int calc_type);
  void template_eax_imm32(int calc_type);

  void add_rm32_r32();  // 0x01
  void add_r32_rm32();  // 0x03
  void add_eax_imm32(); // 0x05
  void or_rm32_r32();   // 0x09
  void or_r32_rm32();   // 0x0b
  void or_eax_imm32();  // 0x0d
  void adc_rm32_r32();  // 0x11
  void adc_r32_rm32();  // 0x13
  void adc_eax_imm32(); // 0x15
  void sbb_rm32_r32();  // 0x19
  void sbb_r32_rm32();  // 0x1b
  void sbb_eax_imm32(); // 0x1d
  void and_rm32_r32();  // 0x21
  void and_r32_rm32();  // 0x23
  void and_eax_imm32(); // 0x25
  void sub_rm32_r32();  // 0x29
  void sub_r32_rm32();  // 0x2b
  void sub_eax_imm32(); // 0x2d
  void xor_rm32_r32();  // 0x31
  void xor_r32_rm32();  // 0x33
  void xor_eax_imm32(); // 0x35
  void cmp_rm32_r32();  // 0x39
  void cmp_eax_imm32(); // 0x3d
  void inc_eax();       // 0x40
  void inc_ecx();       // 0x41
  void inc_edx();       // 0x42
  void inc_ebx();       // 0x43
  void inc_esp();       // 0x44
  void inc_ebp();       // 0x45
  void inc_esi();       // 0x46
  void inc_edi();       // 0x47
  void dec_eax();       // 0x48
  void dec_ecx();       // 0x49
  void dec_edx();       // 0x4a
  void dec_ebx();       // 0x4b
  void dec_esp();       // 0x4c
  void dec_ebp();       // 0x4d
  void dec_esi();       // 0x4e
  void dec_edi();       // 0x4f
  void push_eax();      // 0x50
  void push_ecx();      // 0x51
  void push_edx();      // 0x52
  void push_ebx();      // 0x53
  void push_esp();      // 0x54
  void push_ebp();      // 0x55
  void push_esi();      // 0x56
  void push_edi();      // 0x57
  void pop_eax();       // 0x58
  void pop_ecx();       // 0x59
  void pop_edx();       // 0x5a
  void pop_ebx();       // 0x5b
  void pop_esp();       // 0x5c
  void pop_ebp();       // 0x5d
  void pop_esi();       // 0x5e
  void pop_edi();       // 0x5f
  void push_imm32();    // 0x68
  void push_imm8();     // 0x6a
  void je_imm8();       // 0x74
  void jne_imm8();      // 0x75
  void opcode_81();     // 0x81
  void opcode_83();     // 0x83
  void mov_rm32_r32();  // 0x89
  void mov_r32_rm32();  // 0x8b
  void nop();           // 0x90
  void xchg_eax_r32();  // 0x91 ~ 0x97
  void mov_eax_imm32(); // 0xb8
  void mov_ecx_imm32(); // 0xb9
  void mov_edx_imm32(); // 0xba
  void mov_ebx_imm32(); // 0xbb
  void mov_esp_imm32(); // 0xbc
  void mov_ebp_imm32(); // 0xbd
  void mov_esi_imm32(); // 0xbe
  void mov_edi_imm32(); // 0xbf
  void ret();           // 0xc3
  void leave();         // 0xc9
  void call_imm32();    // 0xe8
  void jmp_imm32();     // 0xe9
  void jmp_imm8();      // 0xeb
  void hlt();           // 0xf4
  void opcode_ff();     // 0xff

  // called by opcode_81 and opcode_83
  void add_rm32_imm(int imm_flag);
  void or_rm32_imm(int imm_flag);
  void adc_rm32_imm(int imm_flag);
  void sbb_rm32_imm(int imm_flag);
  void and_rm32_imm(int imm_flag);
  void sub_rm32_imm(int imm_flag);
  void xor_rm32_imm(int imm_flag);
  void cmp_rm32_imm(int imm_flag);
};
