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

  this->instructions[0x01] = &Instructions::add_rm32_r32;
  this->instructions[0x31] = &Instructions::xor_rm32_r32;
  this->instructions[0x49] = &Instructions::dec_ecx;
  this->instructions[0x75] = &Instructions::jne_imm8;
  this->instructions[0x83] = &Instructions::opcode_83;
  this->instructions[0x89] = &Instructions::mov_rm32_r32;
  this->instructions[0x90] = &Instructions::nop;
  this->instructions[0xb8] = &Instructions::mov_eax_imm32;
  this->instructions[0xb9] = &Instructions::mov_ecx_imm32;
  this->instructions[0xba] = &Instructions::mov_edx_imm32;
  this->instructions[0xbb] = &Instructions::mov_ebx_imm32;
  this->instructions[0xbc] = &Instructions::mov_esp_imm32;
  this->instructions[0xeb] = &Instructions::jmp_imm8;
  this->instructions[0xf4] = &Instructions::hlt;
  this->instructions[0xff] = &Instructions::opcode_ff;

  this->instructions[0x66] = &Instructions::nop;
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

void Instructions::add_rm32_r32(){
  //printf("add_rm32_r32 called.\n");
  uint32_t addr, dst, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // add [M], R
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // dst : data of [M]
      dst = memory.read_uint32(addr);
      memory.write_uint32(addr, dst + this->registers[this->R]);
      break;
    case 1:
      // add [M+imm8], R
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm8]
      dst = memory.read_uint32(addr + imm8);
      memory.write_uint32(addr + imm8, dst + this->registers[this->R]);
      this->eip++;
      break;
    case 2:
      // add [M+imm32], R
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm32]
      dst = memory.read_uint32(addr + imm32); // error
      memory.write_uint32(addr, dst + this->registers[this->R]);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // add M, R
      this->eip++;
      this->registers[this->M] += this->registers[this->R];
      break;
  }
}

void Instructions::xor_rm32_r32(){
  //printf("xor_rm32_r32 called.\n");
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

void Instructions::dec_ecx(){
  //printf("dec_ecx called.\n");
  this->registers[1]--;
}

void Instructions::jne_imm8(){
  //printf("jne_imm8 called.\n");

  int8_t imm8 = memory.read_int8(this->eip);

  int zero_flag = this->get_flag(ZF);
  if (!zero_flag){
    this->eip += imm8;
  }
  this->eip++;
}

void Instructions::opcode_83(){
  //printf("opcode_83 called.\n");

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->R) {
    case 7:
      cmp_rm32_imm8();
      break;
    default:
      break;
  }
}

void Instructions::mov_rm32_r32(){
  //printf("mov_rm32_r32 called.\n");
  uint32_t addr, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // mov [M], R
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      memory.write_uint32(addr, this->registers[this->R]);
      break;
    case 1:
      // mov [M+imm8], R
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      memory.write_uint32(addr + imm8, this->registers[this->R]);
      this->eip++;
      break;
    case 2:
      // mov [M+imm32], R
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      memory.write_uint32(addr, this->registers[this->R]);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // mov M, R
      this->eip++;
      this->registers[this->M] = this->registers[this->R];
      break;
  }
}

void Instructions::nop(){
  //printf("nop called.\n");
}

void Instructions::mov_eax_imm32(){
  //printf("mov_eax_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[0] = imm32;
  this->eip += 4;
}

void Instructions::mov_ecx_imm32(){
  //printf("mov_ecx_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[1] = imm32;
  this->eip += 4;
}

void Instructions::mov_edx_imm32(){
  //printf("mov_edx_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[2] = imm32;
  this->eip += 4;
}

void Instructions::mov_ebx_imm32(){
  //printf("mov_ebx_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[3] = imm32;
  this->eip += 4;
}

void Instructions::mov_edx_imm32(){
  //printf("mov_esp_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[4] = imm32;
  this->eip += 4;
}

void Instructions::jmp_imm8() {
  //printf("jmp_imm8 called.\n");

  int8_t imm8 = memory.read_int8(this->eip);
  this->eip += imm8;
  this->eip++;
}

void Instructions::hlt(){
  printf("hlt called.\n");
  this->eip = 0x00;
}

void Instructions::opcode_ff(){
  //printf("opcode_ff called.\n");

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->R) {
    case 0:
      // TODO: add inc_r32() function
      // this->execute_opcode(0x40+this->M)
      break;
    case 1:
      this->execute_opcode(0x48+this->M); // dec r32
      this->eip++;
      break;
    default:
      break;
  }
}

void Instructions::cmp_rm32_imm8(){
  //printf("cmp_rm32_imm8 called.\n");

  this->eip++;
  uint8_t imm8 = memory.read_uint8(this->eip);
  //printf("imm8: 0x%08x (%d)\n", imm8, imm8);
  uint32_t result = this->registers[this->M] - imm8;

  set_flag(!result, ZF);

  this->eip++;
}

void Instructions::set_flag(int flag, uint32_t flag_type){
  if (flag) {
    this->eflags |= flag_type;
  } else {
    this->eflags &= ~flag_type;
  }
}

int Instructions::get_flag(uint32_t flag_type){
  int flag_status = 0;

  if ((this->eflags & flag_type) == 0) {
    flag_status = 0;
  } else {
    flag_status = 1;
  }

  return flag_status;
}
