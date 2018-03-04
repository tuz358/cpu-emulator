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

  this->instructions[0x90] = &Instructions::nop;
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

void Instructions::nop(){
  printf("nop called.\n");
}

void Instructions::hlt(){
  printf("hlt called.\n");
  this->eip = 0x00;
}
