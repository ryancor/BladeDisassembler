#include "../inc/disass.h"
#include "../inc/helper.h"
#include "../inc/gui.h"

// each enum value returns a number from instruction
int ReturnInstructionNumber(unsigned char* opcode, int value)
{
  struct instrReg instreg;

  if(opcode[value] >= 0x50 && opcode[value] <= 0x68)
  {
    // 68 opcode holds a special value for PUSH
    instreg.instruction = PUSH;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xcd) {
    // syscalls
    instreg.instruction = INT;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x30 || opcode[value] == 0x31) {
    instreg.instruction = XOR;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xf6 || opcode[value] == 0xf7) {
    instreg.instruction = MUL;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xb0 || opcode[value] == 0x89) {
    instreg.instruction = MOV;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x87 || opcode[value] == 0x93) {
    instreg.instruction = XCHG;
    return instreg.instruction;
  }
  else {
    // opcode 90
    instreg.instruction = NOP;
    return instreg.instruction;
  }
}

// each enum value returns a number from register
int ReturnRegisterNumber(unsigned char* opcode, int value)
{
  struct instrReg instreg;

  if(opcode[value] == 0x50) {
    instreg.registr = EAX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x51) {
    instreg.registr = ECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x93) {
    instreg.registr = EBXEAX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x68 || opcode[value] == 0xcd) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0x31 && opcode[value+1] == 0xc9) {
    instreg.registr = ECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0xf7 && opcode[value+1] == 0xe1) {
    instreg.registr = ECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb0 && opcode[value+1] >= 0x00) {
    instreg.registr = AL;
    return instreg.registr;
  }
  else if(opcode[value] == 0x89 && opcode[value+1] == 0xe3) {
    instreg.registr = EBXESP;
    return instreg.registr;
  }
  else {
    instreg.registr = EIP;
    return instreg.registr;
  }
}

void printAssemblyCode(const char* instr, const char* reg, unsigned char* bytes_read,
  int i, unsigned int start_address)
{
  FILE *file;
  struct storeSplitStr storedStr;

  // Skip anything that returns null
  if(strncmp("EIP", reg, strlen(reg)) != 0)
  {
    file = fopen(filename, "a");
    fprintf(file, "0x00000%x:  ", start_address);
    printf("0x00000%x:  ", start_address);
    if(strncmp("MOV", instr, strlen(instr)) == 0)
    {
      if(bytes_read[i] == 0x89)
      {
        // once we hit an 0x89 opcode, we have series range of what gets mov'd
        if(bytes_read[i+1] == 0xe3)
        {
          storedStr = splitStrStart(reg, " ");
          printf("%s      %s, %s\n", instr, storedStr.reg1, storedStr.reg2);
          fprintf(file, "%s      %s, %s\n", instr, storedStr.reg1, storedStr.reg2);
        }
      }
      // just mov'ing a number into a register
      else {
        printf("%s      %s, 0x%x\n", instr, reg, (int)bytes_read[i+1]);
        fprintf(file, "%s      %s, 0x%x\n", instr, reg, (int)bytes_read[i+1]);
      }
      i += 1;
    }
    else if(strncmp("XCHG", instr, strlen(instr)) == 0) {
      // series of opcodes for xchg we will have to move in
      if(bytes_read[i] == 0x93)
      {
        storedStr = splitStrStart(reg, " ");
        printf("%s     %s, %s\n", instr, storedStr.reg1, storedStr.reg2);
        fprintf(file, "%s     %s, %s\n", instr, storedStr.reg1, storedStr.reg2);
      }
    }
    else if(strncmp("XOR", instr, strlen(instr)) == 0) {
      printf("%s      %s, %s\n", instr, reg, reg);
      fprintf(file, "%s      %s, %s\n", instr, reg, reg);
    }
    else if(strncmp("SPEC", reg, strlen(reg)) == 0) {
      if(strncmp("PUSH", instr, strlen(reg)) == 0)
      {
        printf("%s     0x%x%x%x%x\n", instr, (int)bytes_read[i+4], (int)bytes_read[i+3],
            (int)bytes_read[i+2], (int)bytes_read[i+1]);
        fprintf(file, "%s     0x%x%x%x%x\n", instr, (int)bytes_read[i+4],
            (int)bytes_read[i+3], (int)bytes_read[i+2], (int)bytes_read[i+1]);
        // jump further into the loop, i jumps four bytes
        i += 4;
      }
      else if(strncmp("INT", instr, strlen(reg)) == 0) {
        // INT (interrupt) can have numbers ranging from 0x01 to 0x80
        printf("%s      0x%x\n", instr, (int)bytes_read[i+1]);
        fprintf(file, "%s      0x%x\n", instr, (int)bytes_read[i+1]);
        // jump further into the loop, i jumps four bytes
        i += 1;
      }
    }
    else {
      printf("%s      %s\n", instr, reg);
      fprintf(file, "%s      %s\n", instr, reg);
    }
    fclose(file);
  }
}
