#include "include/instructions.h"

void Instructions::init(uint32_t eip, uint32_t esp, Memory memory){
  // initialize general purpose registers
  memset(this->registers, 0, sizeof(this->registers));
  this->registers[4] = esp;

  // initialize eflags register
  this->eflags = 0;

  // initialize instruction pointer
  this->eip = eip;

  this->memory = memory;

  this->init_instructions();
  this->init_modrm();
}

void Instructions::init_instructions(){
  memset(this->instructions, 0, sizeof(this->instructions));

  this->instructions[0x31] = &Instructions::xor_rm32_r32;
  this->instructions[0x90] = &Instructions::nop;
  this->instructions[0xb9] = &Instructions::mov_ecx_imm32;
  this->instructions[0xbb] = &Instructions::mov_ebx_imm32;
  this->instructions[0xf4] = &Instructions::hlt;
}

void Instructions::init_modrm(){
  this->modrm = 0;
  this->mod   = 0;
  this->R     = 0;
  this->M     = 0;
}

void Instructions::calc_modrm(){
  this->mod = (this->modrm & 0xc0) >> 6;
  this->R   = (this->modrm & 0x38) >> 3;
  this->M   =  this->modrm & 0x07;
}

void Instructions::execute_opcode(uint8_t opcode){
  (this->*instructions[opcode])();
}

void Instructions::xor_rm32_r32(){
  printf("xor_rm32_r32 called.\n");
  uint32_t addr, dst, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // xor [M], R
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // dst : data of [M]
      dst = memory.read_uint32(addr);
      memory.write_uint32(addr, dst ^ this->registers[this->R]);
      break;
    case 1:
      // xor [M+imm8], R
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm8]
      dst = memory.read_uint32(addr + imm8);
      memory.write_uint32(addr + imm8, dst ^ this->registers[this->R]);
      break;
    case 2:
      // xor [M+imm32], R
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm32]
      dst = memory.read_uint32(addr + imm32);
      memory.write_uint32(addr, dst ^ this->registers[this->R]);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // xor M, R
      this->eip++;
      this->registers[this->M] ^= this->registers[this->R];
      break;
  }
}

void Instructions::nop(){
  printf("nop called.\n");
}

void Instructions::mov_ecx_imm32(){
  printf("mov_ecx_imm32 called.\n");

  this->eip++;
  uint32_t imm32 = memory.read_uint32(this->eip);
  this->registers[1] = imm32;
  this->eip += 3;
}

void Instructions::mov_ebx_imm32(){
  printf("mov_ebx_imm32 called.\n");

  this->eip++;
  uint32_t imm32 = memory.read_uint32(this->eip);
  this->registers[3] = imm32;
  this->eip += 3;
}

void Instructions::hlt(){
  printf("hlt called.\n");
  this->eip = 0x00;
}
