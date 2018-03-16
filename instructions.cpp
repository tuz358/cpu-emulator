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
  this->instructions[0x03] = &Instructions::add_r32_rm32;
  this->instructions[0x05] = &Instructions::add_eax_imm32;
  this->instructions[0x09] = &Instructions::or_rm32_r32;
  this->instructions[0x0b] = &Instructions::or_r32_rm32;
  this->instructions[0x0d] = &Instructions::or_eax_imm32;
  this->instructions[0x11] = &Instructions::adc_rm32_r32;
  this->instructions[0x13] = &Instructions::adc_r32_rm32;
  this->instructions[0x15] = &Instructions::adc_eax_imm32;
  this->instructions[0x19] = &Instructions::sbb_rm32_r32;
  this->instructions[0x1b] = &Instructions::sbb_r32_rm32;
  this->instructions[0x1d] = &Instructions::sbb_eax_imm32;
  this->instructions[0x21] = &Instructions::and_rm32_r32;
  this->instructions[0x23] = &Instructions::and_r32_rm32;
  this->instructions[0x25] = &Instructions::and_eax_imm32;
  this->instructions[0x29] = &Instructions::sub_rm32_r32;
  this->instructions[0x2b] = &Instructions::sub_r32_rm32;
  this->instructions[0x2d] = &Instructions::sub_eax_imm32;
  this->instructions[0x31] = &Instructions::xor_rm32_r32;
  this->instructions[0x33] = &Instructions::xor_r32_rm32;
  this->instructions[0x35] = &Instructions::xor_eax_imm32;
  this->instructions[0x39] = &Instructions::cmp_rm32_r32;
  this->instructions[0x3b] = &Instructions::cmp_rm32_r32;
  this->instructions[0x3d] = &Instructions::cmp_eax_imm32;
  this->instructions[0x40] = &Instructions::inc_eax;
  this->instructions[0x41] = &Instructions::inc_ecx;
  this->instructions[0x42] = &Instructions::inc_edx;
  this->instructions[0x43] = &Instructions::inc_ebx;
  this->instructions[0x44] = &Instructions::inc_esp;
  this->instructions[0x45] = &Instructions::inc_ebp;
  this->instructions[0x46] = &Instructions::inc_esi;
  this->instructions[0x47] = &Instructions::inc_edi;
  this->instructions[0x48] = &Instructions::dec_eax;
  this->instructions[0x49] = &Instructions::dec_ecx;
  this->instructions[0x4a] = &Instructions::dec_edx;
  this->instructions[0x4b] = &Instructions::dec_ebx;
  this->instructions[0x4c] = &Instructions::dec_esp;
  this->instructions[0x4d] = &Instructions::dec_ebp;
  this->instructions[0x4e] = &Instructions::dec_esi;
  this->instructions[0x4f] = &Instructions::dec_edi;
  this->instructions[0x50] = &Instructions::push_eax;
  this->instructions[0x51] = &Instructions::push_ecx;
  this->instructions[0x52] = &Instructions::push_edx;
  this->instructions[0x53] = &Instructions::push_ebx;
  this->instructions[0x54] = &Instructions::push_esp;
  this->instructions[0x55] = &Instructions::push_ebp;
  this->instructions[0x56] = &Instructions::push_esi;
  this->instructions[0x57] = &Instructions::push_edi;
  this->instructions[0x58] = &Instructions::pop_eax;
  this->instructions[0x59] = &Instructions::pop_ecx;
  this->instructions[0x5a] = &Instructions::pop_edx;
  this->instructions[0x5b] = &Instructions::pop_ebx;
  this->instructions[0x5c] = &Instructions::pop_esp;
  this->instructions[0x5d] = &Instructions::pop_ebp;
  this->instructions[0x5e] = &Instructions::pop_esi;
  this->instructions[0x5f] = &Instructions::pop_edi;
  this->instructions[0x68] = &Instructions::push_imm32;
  this->instructions[0x6a] = &Instructions::push_imm8;
  this->instructions[0x74] = &Instructions::je_imm8;
  this->instructions[0x75] = &Instructions::jne_imm8;
  this->instructions[0x81] = &Instructions::opcode_81;
  this->instructions[0x83] = &Instructions::opcode_83;
  this->instructions[0x89] = &Instructions::mov_rm32_r32;
  this->instructions[0x8b] = &Instructions::mov_r32_rm32;
  this->instructions[0x90] = &Instructions::nop;
  for(int i=0;i<7;i++){
    this->instructions[0x91+i] = &Instructions::xchg_eax_r32;
  }
  this->instructions[0xb8] = &Instructions::mov_eax_imm32;
  this->instructions[0xb9] = &Instructions::mov_ecx_imm32;
  this->instructions[0xba] = &Instructions::mov_edx_imm32;
  this->instructions[0xbb] = &Instructions::mov_ebx_imm32;
  this->instructions[0xbc] = &Instructions::mov_esp_imm32;
  this->instructions[0xbd] = &Instructions::mov_ebp_imm32;
  this->instructions[0xbe] = &Instructions::mov_esi_imm32;
  this->instructions[0xbf] = &Instructions::mov_edi_imm32;
  this->instructions[0xc3] = &Instructions::ret;
  this->instructions[0xc9] = &Instructions::leave;
  this->instructions[0xe8] = &Instructions::call_imm32;
  this->instructions[0xe9] = &Instructions::jmp_imm32;
  this->instructions[0xeb] = &Instructions::jmp_imm8;
  this->instructions[0xf4] = &Instructions::hlt;
  this->instructions[0xff] = &Instructions::opcode_ff;

  this->instructions[0x66] = &Instructions::nop;
  this->instructions[0x67] = &Instructions::nop;
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

void Instructions::template_rm32_r32(int calc_type){
  //printf("template_rm32_r32 called.\n");
  uint32_t addr, dst, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // operation [M], R
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // dst : data of [M]
      dst = memory.read_uint32(addr);
      calc_rm32_r32_case0to2(addr, dst, calc_type);
      break;
    case 1:
      // operation [M+imm8], R
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm8]
      dst = memory.read_uint32(addr + imm8);
      calc_rm32_r32_case0to2(addr + imm8, dst, calc_type);
      this->eip++;
      break;
    case 2:
      // operation [M+imm32], R
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm32]
      dst = memory.read_uint32(addr + imm32);
      calc_rm32_r32_case0to2(addr + imm32, dst, calc_type);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // operation M, R
      this->eip++;
      calc_r32_rm32(&this->registers[this->M], &this->registers[this->R], calc_type);
      break;
  }
}

void Instructions::calc_rm32_r32_case0to2(uint32_t addr, uint32_t dst, int calc_type){
  switch (calc_type) {
    case ADD:
      memory.write_uint32(addr, dst + this->registers[this->R]); break;
    case OR:
      memory.write_uint32(addr, dst | this->registers[this->R]); break;
    case ADC:
      memory.write_uint32(addr, dst + this->registers[this->R] + get_flag(CF)); break;
    case SBB:
      memory.write_uint32(addr, dst - (this->registers[this->R] + get_flag(CF))); break;
    case AND:
      memory.write_uint32(addr, dst & this->registers[this->R]); break;
    case SUB:
      memory.write_uint32(addr, dst - this->registers[this->R]); break;
    case XOR:
      memory.write_uint32(addr, dst ^ this->registers[this->R]); break;
    case CMP:
      // TODO: implement
      break;
    default: break;
  }
}

void Instructions::template_r32_rm32(int calc_type){
  uint32_t addr, src, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // operation R, [M]
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // src : data of [M]
      src = memory.read_uint32(addr);
      calc_r32_rm32(&this->registers[this->R], &src, calc_type);
      break;
    case 1:
      // operation R, [M+imm8]
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // src : data of [M+imm8]
      src = memory.read_uint32(addr + imm8);
      calc_r32_rm32(&this->registers[this->R], &src, calc_type);
      this->eip++;
      break;
    case 2:
      // operation R, [M+imm32]
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // src : data of [M+imm32]
      src = memory.read_uint32(addr + imm32);
      calc_r32_rm32(&this->registers[this->R], &src, calc_type);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // operation R, M
      this->eip++;
      calc_r32_rm32(&this->registers[this->R], &this->registers[this->M], calc_type);
      break;
  }
}

void Instructions::calc_r32_rm32(uint32_t *dst, uint32_t *src, int calc_type){
  switch (calc_type) {
    case ADD:
      *dst += *src; break;
    case OR:
      *dst |= *src; break;
    case ADC:
      *dst += *src + get_flag(CF); break;
    case SBB:
      *dst -= *src + get_flag(CF); break;
    case AND:
      *dst &= *src; break;
    case SUB:
      *dst -= *src; break;
    case XOR:
      *dst ^= *src; break;
    case CMP:
      // TODO: implement
      break;
    default: break;
  }
}

void Instructions::template_eax_imm32(int calc_type){
  this->eip++;
  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);

  switch (calc_type) {
    case ADD:
      this->registers[0] += imm32; break;
    case OR:
      this->registers[0] |= imm32; break;
    case ADC:
      this->registers[0] += imm32 + get_flag(CF); break;
    case SBB:
      this->registers[0] -= imm32 + get_flag(CF); break;
    case AND:
      this->registers[0] &= imm32; break;
    case SUB:
      this->registers[0] -= imm32; break;
    case XOR:
      this->registers[0] ^= imm32; break;
    case CMP:
      // TODO: implement
      break;
    default: break;
  }
}

void Instructions::add_rm32_r32() { this->template_rm32_r32(ADD); }
void Instructions::add_r32_rm32() { this->template_r32_rm32(ADD); }
void Instructions::add_eax_imm32(){ this->template_eax_imm32(ADD); }

void Instructions::or_rm32_r32() { this->template_rm32_r32(OR); }
void Instructions::or_r32_rm32() { this->template_r32_rm32(OR); }
void Instructions::or_eax_imm32(){ this->template_eax_imm32(OR); }

void Instructions::adc_rm32_r32() { this->template_rm32_r32(ADC); }
void Instructions::adc_r32_rm32() { this->template_r32_rm32(ADC); }
void Instructions::adc_eax_imm32(){ this->template_eax_imm32(ADC); }

void Instructions::sbb_rm32_r32() { this->template_rm32_r32(SBB); }
void Instructions::sbb_r32_rm32() { this->template_r32_rm32(SBB); }
void Instructions::sbb_eax_imm32(){ this->template_eax_imm32(SBB); }

void Instructions::and_rm32_r32() { this->template_rm32_r32(AND); }
void Instructions::and_r32_rm32() { this->template_r32_rm32(AND); }
void Instructions::and_eax_imm32(){ this->template_eax_imm32(AND); }

void Instructions::sub_rm32_r32() { this->template_rm32_r32(SUB); }
void Instructions::sub_r32_rm32() { this->template_r32_rm32(SUB); }
void Instructions::sub_eax_imm32(){ this->template_eax_imm32(SUB); }

void Instructions::xor_rm32_r32() { this->template_rm32_r32(XOR); }
void Instructions::xor_r32_rm32() { this->template_r32_rm32(XOR); }
void Instructions::xor_eax_imm32(){ this->template_eax_imm32(XOR); }

void Instructions::cmp_rm32_r32(){
  //printf("cmp_rm32_r32 called.\n");
  uint32_t addr, dst, imm32, result;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // cmp [M], R
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // dst : data of [M]
      dst = memory.read_uint32(addr);
      result = dst - this->registers[this->R];
      set_flag(!result, ZF);
      break;
    case 1:
      // cmp [M+imm8], R
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm8]
      dst = memory.read_uint32(addr + imm8);
      result = dst - this->registers[this->R];
      set_flag(!result, ZF);
      break;
    case 2:
      // cmp [M+imm32], R
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm32]
      dst = memory.read_uint32(addr + imm32);
      result = dst - this->registers[this->R];
      set_flag(!result, ZF);
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // cmp M, R
      this->eip++;
      result = this->registers[this->M] - this->registers[this->R];
      set_flag(!result, ZF);
      break;
  }
}

void Instructions::cmp_eax_imm32(){
  this->eip++;
  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  uint32_t result = this->registers[0] - imm32;
  set_flag(!result, ZF);
}

void Instructions::inc_eax(){ this->registers[0]++; }
void Instructions::inc_ecx(){ this->registers[1]++; }
void Instructions::inc_edx(){ this->registers[2]++; }
void Instructions::inc_ebx(){ this->registers[3]++; }
void Instructions::inc_esp(){ this->registers[4]++; }
void Instructions::inc_ebp(){ this->registers[5]++; }
void Instructions::inc_esi(){ this->registers[6]++; }
void Instructions::inc_edi(){ this->registers[7]++; }

void Instructions::dec_eax(){ this->registers[0]--; }
void Instructions::dec_ecx(){ this->registers[1]--; }
void Instructions::dec_edx(){ this->registers[2]--; }
void Instructions::dec_ebx(){ this->registers[3]--; }
void Instructions::dec_esp(){ this->registers[4]--; }
void Instructions::dec_ebp(){ this->registers[5]--; }
void Instructions::dec_esi(){ this->registers[6]--; }
void Instructions::dec_edi(){ this->registers[7]--; }

void Instructions::push_eax(){
  //printf("push_eax called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[0]);
}

void Instructions::push_ecx(){
  //printf("push_ecx called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[1]);
}

void Instructions::push_edx(){
  //printf("push_edx called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[2]);
}

void Instructions::push_ebx(){
  //printf("push_ebx called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[3]);
}

void Instructions::push_esp(){
  //printf("push_esp called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[4]);
}

void Instructions::push_ebp(){
  //printf("push_ebp called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[5]);
}

void Instructions::push_esi(){
  //printf("push_esi called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[6]);
}

void Instructions::push_edi(){
  //printf("push_edi called.\n");
  this->registers[4] -= 4;
  memory.write_uint32(this->registers[4], this->registers[7]);
}

void Instructions::pop_eax(){
  //printf("pop_eax called.\n");
  this->registers[0] = memory.read_uint32(this->registers[4]);
  this->registers[0] = swap_endian32(this->registers[0]);
  this->registers[4] += 4;
}

void Instructions::pop_ecx(){
  //printf("pop_ecx called.\n");
  this->registers[1] = memory.read_uint32(this->registers[4]);
  this->registers[1] = swap_endian32(this->registers[1]);
  this->registers[4] += 4;
}

void Instructions::pop_edx(){
  //printf("pop_edx called.\n");
  this->registers[2] = memory.read_uint32(this->registers[4]);
  this->registers[2] = swap_endian32(this->registers[2]);
  this->registers[4] += 4;
}

void Instructions::pop_ebx(){
  //printf("pop_ebx called.\n");
  this->registers[3] = memory.read_uint32(this->registers[4]);
  this->registers[3] = swap_endian32(this->registers[3]);
  this->registers[4] += 4;
}

void Instructions::pop_esp(){
  //printf("pop_esp called.\n");
  this->registers[4] = memory.read_uint32(this->registers[4]);
  this->registers[4] = swap_endian32(this->registers[4]);
  this->registers[4] += 4;
}

void Instructions::pop_ebp(){
  //printf("pop_ebp called.\n");
  this->registers[5] = memory.read_uint32(this->registers[4]);
  this->registers[5] = swap_endian32(this->registers[5]);
  this->registers[4] += 4;
}

void Instructions::pop_esi(){
  //printf("pop_esi called.\n");
  this->registers[6] = memory.read_uint32(this->registers[4]);
  this->registers[6] = swap_endian32(this->registers[6]);
  this->registers[4] += 4;
}

void Instructions::pop_edi(){
  //printf("pop_esi called.\n");
  this->registers[7] = memory.read_uint32(this->registers[4]);
  this->registers[7] = swap_endian32(this->registers[7]);
  this->registers[4] += 4;
}

void Instructions::push_imm32(){
  //printf("push_imm32 called.\n");
  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[4] -= 4; // esp -= 4
  memory.write_uint32(this->registers[4], imm32);
  this->eip += 4;
}

void Instructions::push_imm8(){
  //printf("push_imm8 called.\n");
  uint8_t imm8 = memory.read_uint8(this->eip);
  this->registers[4] -= 4; // esp -= 4
  memory.write_uint8(this->registers[4], imm8);
  this->eip++;
}

void Instructions::je_imm8(){
  //printf("je_imm8 called.\n");

  int8_t imm8 = memory.read_int8(this->eip);

  int zero_flag = this->get_flag(ZF);
  if (zero_flag){
    this->eip += imm8;
  }
  this->eip++;
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

void Instructions::opcode_81(){
  //printf("opcode_81 called.\n");

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->R) {
    case 0:
      add_rm32_imm(IMM32);
      break;
    case 1:
      or_rm32_imm(IMM32);
      break;
    case 2:
      adc_rm32_imm(IMM32);
      break;
    case 3:
      sbb_rm32_imm(IMM32);
      break;
    case 4:
      // TODO: and_rm32_imm32();
      break;
    case 5:
      // TODO: sub_rm32_imm32();
      break;
    case 6:
      // TODO: xor_rm32_imm32();
      break;
    case 7:
      // TODO: cmp_rm32_imm8();
      break;
    default:
      break;
  }
}

void Instructions::opcode_83(){
  //printf("opcode_83 called.\n");

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->R) {
    case 0:
      add_rm32_imm(IMM8);
      break;
    case 1:
      or_rm32_imm(IMM8);
      break;
    case 2:
      adc_rm32_imm(IMM8);
      break;
    case 3:
      sbb_rm32_imm(IMM8);
      break;
    case 4:
      // TODO: and_rm32_imm8();
      break;
    case 5:
      // TODO: sub_rm32_imm8();
      break;
    case 6:
      // TODO: xor_rm32_imm8();
      break;
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

void Instructions::mov_r32_rm32(){
  //printf("mov_r32_rm32 called.\n");
  uint32_t addr, dst, imm32;
  uint8_t imm8;

  this->modrm = memory.read_uint8(this->eip);
  this->calc_modrm();

  switch (this->mod) {
    case 0:
      // mov R, [M]
      // addr : M
      this->eip++;
      addr = this->registers[this->M];
      // dst : data of [M]
      dst = memory.read_uint32(addr);
      this->registers[this->R] = dst;
      break;
    case 1:
      // mov R, [M+imm8]
      this->eip++;
      imm8 = memory.read_uint8(this->eip);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm8]
      dst = memory.read_uint32(addr + imm8);
      this->registers[this->R] = dst;
      this->eip++;
      break;
    case 2:
      // mov R, [M+imm32]
      this->eip++;
      imm32 = memory.read_uint32(this->eip);
      imm32 = swap_endian32(imm32);
      // addr : M
      addr = this->registers[this->M];
      // dst : data of [M+imm32]
      dst = memory.read_uint32(addr + imm32);
      this->registers[this->R] = dst;
      this->eip += 4;
      break;
    default:
      // case mod == 3
      // mov R, M
      this->eip++;
      this->registers[this->R] = this->registers[this->M];
      break;
  }
}

void Instructions::nop(){
  //printf("nop called.\n");
}

void Instructions::xchg_eax_r32(){
  //printf("xchg_eax_r32 called.\n");
  uint8_t opcode = memory.read_uint8(this->eip - 1);
  this->registers[0] ^= this->registers[opcode - 0x90];
  this->registers[opcode - 0x90] ^= this->registers[0];
  this->registers[0] ^= this->registers[opcode - 0x90];
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

void Instructions::mov_esp_imm32(){
  //printf("mov_esp_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[4] = imm32;
  this->eip += 4;
}

void Instructions::mov_ebp_imm32(){
  //printf("mov_ebp_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[5] = imm32;
  this->eip += 4;
}

void Instructions::mov_esi_imm32(){
  //printf("mov_esi_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[6] = imm32;
  this->eip += 4;
}

void Instructions::mov_edi_imm32(){
  //printf("mov_edi_imm32 called.\n");

  uint32_t imm32 = memory.read_uint32(this->eip);
  imm32 = swap_endian32(imm32);
  this->registers[7] = imm32;
  this->eip += 4;
}

void Instructions::ret(){
  //printf("ret called.\n");
  this->eip = memory.read_uint32(this->registers[4]);
  this->eip = swap_endian32(this->eip);
}

void Instructions::leave(){
  //printf("leave called.\n");

  // mov esp, ebp
  this->registers[4] = this->registers[5];
  // pop ebp
  this->pop_ebp();
}

void Instructions::call_imm32(){
  //printf("call_imm32 called.\n");

  int32_t imm32 = memory.read_int32(this->eip);
  imm32 = (int32_t)swap_endian32((uint32_t)imm32);
  // push eip
  this->registers[4] -= 4; // esp -= 4
  memory.write_uint32(this->registers[4], this->eip);
  // jmp imm32
  this->eip += imm32;
  this->eip++;
}

void Instructions::jmp_imm32(){
  //printf("jmp_imm32 called.\n");

  int32_t imm32 = memory.read_int32(this->eip);
  imm32 = (int32_t)swap_endian32((uint32_t)imm32);
  this->eip += imm32;
  this->eip++;
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
      this->execute_opcode(0x40+this->M); // inc r32
      this->eip++;
      break;
    case 1:
      this->execute_opcode(0x48+this->M); // dec r32
      this->eip++;
      break;
    default:
      break;
  }
}

void Instructions::add_rm32_imm(int imm_flag){
  //printf("add_rm32_imm called.\n");

  this->eip++;

  if(imm_flag == IMM8){
    uint8_t imm8 = memory.read_uint8(this->eip);
    this->registers[this->M] += imm8;
  } else if(imm_flag == IMM32){
    uint32_t imm32 = memory.read_uint32(this->eip);
    imm32 = swap_endian32(imm32);
    this->registers[this->M] += imm32;
  } else {
  }

  this->eip++;
}

void Instructions::or_rm32_imm(int imm_flag){
  //printf("or_rm32_imm called.\n");

  this->eip++;

  if(imm_flag == IMM8){
    uint8_t imm8 = memory.read_uint8(this->eip);
    this->registers[this->M] |= imm8;
  } else if(imm_flag == IMM32){
    uint32_t imm32 = memory.read_uint32(this->eip);
    imm32 = swap_endian32(imm32);
    this->registers[this->M] |= imm32;
  } else {
  }

  this->eip++;
}

void Instructions::adc_rm32_imm(int imm_flag){
  //printf("adc_rm32_imm called.\n");

  this->eip++;

  if(imm_flag == IMM8){
    uint8_t imm8 = memory.read_uint8(this->eip);
    this->registers[this->M] += imm8 + get_flag(CF);
  } else if(imm_flag == IMM32){
    uint32_t imm32 = memory.read_uint32(this->eip);
    imm32 = swap_endian32(imm32);
    this->registers[this->M] += imm32 + get_flag(CF);
  } else {
  }

  this->eip++;
}

void Instructions::sbb_rm32_imm(int imm_flag){
  //printf("sbb_rm32_imm called.\n");

  this->eip++;

  if(imm_flag == IMM8){
    uint8_t imm8 = memory.read_uint8(this->eip);
    this->registers[this->M] -= imm8 + get_flag(CF);
  } else if(imm_flag == IMM32){
    uint32_t imm32 = memory.read_uint32(this->eip);
    imm32 = swap_endian32(imm32);
    this->registers[this->M] -= imm32 + get_flag(CF);
  } else {
  }

  this->eip++;
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
