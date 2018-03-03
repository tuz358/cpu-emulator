#include "include/emulator.h"


void Emulator::init(size_t memorysize, FILE *bin){
  memory.init(memorysize);
  memory.load_binary(bin);
  instructions.init(0, (int)memorysize/2);
}

void Emulator::free(){
  memory.free_memory();
}

void Emulator::exec(uint8_t opcode){
  instructions.execute_opcode(opcode);
}

void Emulator::dump_registers(){
}

uint8_t Emulator::read_next_opcode(){
  uint8_t opcode = memory.read_uint8(instructions.eip);
  instructions.eip++;

  return opcode;
}

uint8_t Emulator::read_uint8(uint32_t addr){
  uint8_t data = memory.read_uint8(addr);
  return data;
}

uint32_t Emulator::get_eip(){
  return instructions.eip;
}
