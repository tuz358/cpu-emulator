#include "include/instructions.h"

void Instructions::init(uint32_t eip, uint32_t esp){
  // initialize general purpose registers
  memset(this->registers, 0, sizeof(this->registers));
  this->registers[4] = esp;

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
