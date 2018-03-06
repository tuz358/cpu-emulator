#include "include/emulator.h"


void Emulator::init(size_t memorysize, FILE *bin){
  memory.init(memorysize);
  memory.load_binary(bin);
  instructions.init(0, (int)memorysize, memory);
}

void Emulator::free(){
  memory.free_memory();
}

void Emulator::exec(uint8_t opcode){
  instructions.execute_opcode(opcode);
}

void Emulator::dump_registers(){
  printf("eax = 0x%08x (%d)\n", instructions.registers[0], instructions.registers[0]);
  printf("ecx = 0x%08x (%d)\n", instructions.registers[1], instructions.registers[1]);
  printf("edx = 0x%08x (%d)\n", instructions.registers[2], instructions.registers[2]);
  printf("ebx = 0x%08x (%d)\n", instructions.registers[3], instructions.registers[3]);
  printf("esp = 0x%08x (%d)\n", instructions.registers[4], instructions.registers[4]);
  printf("ebp = 0x%08x (%d)\n", instructions.registers[5], instructions.registers[5]);
  printf("esi = 0x%08x (%d)\n", instructions.registers[6], instructions.registers[6]);
  printf("edi = 0x%08x (%d)\n", instructions.registers[7], instructions.registers[7]);
  printf("eip = 0x%08x (%d)\n", instructions.eip, instructions.eip);
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
