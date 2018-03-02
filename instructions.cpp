#include "include/instructions.h"

void Instructions::init(uint32_t eip, uint32_t esp){
  // initialize general purpose registers
  this->eax = 0;
  this->ecx = 0;
  this->edx = 0;
  this->ebx = 0;
  this->esp = esp;
  this->ebp = 0;
  this->esi = 0;
  this->edi = 0;

  // initialize eflags register
  this->eflags = 0;

  // initialize instruction pointer
  this->eip = eip;

  this->init_instructions();
}

void Instructions::init_instructions(){
  memset(this->instructions, 0, sizeof(this->instructions));

  this->instructions[0x90] = &Instructions::nop;
  this->instructions[0xf4] = &Instructions::hlt;
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
