#include "../inc/disass.h"
#include "../inc/helper.h"
#include "../inc/gui.h"
#include "../inc/obfuscate.h"

// each enum value returns a number from instruction
int ReturnInstructionNumber(unsigned char* opcode, int value)
{
  struct instrReg instreg;

  if((opcode[value] >= 0x50 && opcode[value] <= 0x68 && opcode[value] != 0x66)
  || opcode[value] == 0x0e)
  {
    // 68 opcode holds a special value for PUSH
    instreg.instruction = PUSH;
    return instreg.instruction;
  }
  else if(opcode[value] >= 0x40 && opcode[value] <= 0x42) {
    // syscalls
    instreg.instruction = INC;
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
  else if(opcode[value] == 0x39) {
    instreg.instruction = CMP;
    return instreg.instruction;
  }
  else if(opcode[value] >= 0x48 && opcode[value] <= 0x4f) {
    instreg.instruction = DEC;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xf6 || opcode[value] == 0xf7) {
    instreg.instruction = MUL;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x66 || opcode[value] == 0x89 || (opcode[value] >= 0xb0
    && opcode[value] <= 0xbb)) {
    // 0x66 0x98 == CBW
    if(opcode[value+1] == 0x98)
    {
      instreg.instruction = CBW;
    }
    // 0x66 0x99 == CWD
    else if(opcode[value+1] == 0x99) {
      instreg.instruction = CWD;
    }
    else {
      instreg.instruction = MOV;
    }
    return instreg.instruction;
  }
  else if(opcode[value] == 0x72) {
    instreg.instruction = JB;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x75) {
    instreg.instruction = JNZ;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x03 || (opcode[value] == 0x83 &&
    (opcode[value+1] >= 0xc0 && opcode[value+1] <= 0xc7))) {
    instreg.instruction = ADD;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x83 && (opcode[value+1] >= 0xe8 && opcode[value+1] <= 0xef)) {
    instreg.instruction = SUB;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x87 || (opcode[value] >= 0x91 && opcode[value] <= 0x93)) {
    instreg.instruction = XCHG;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x8D) {
    instreg.instruction = LEA;
    return instreg.instruction;
  }
  else if(opcode[value] == 0x99) {
    instreg.instruction = CDQ;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xc0 || opcode[value] == 0xc1) {
    if(opcode[value+1] >= 0xe0 && opcode[value+1] <= 0xe5)
    {
      instreg.instruction = SHL;
    }
    else {
      instreg.instruction = SHR;
    }
    return instreg.instruction;
  }
  else if(opcode[value] == 0xE9 || (opcode[value] == 0xEA && opcode[value] == 0xEB)) {
    instreg.instruction = JMP;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xF3) {
    if(opcode[value+1] == 0xAA)
    {
      instreg.instruction = STOSB;
    }
    else if(opcode[value+1] == 0xAB) {
      instreg.instruction = STOSD;
    }
    else {
      instreg.instruction = STOSW;
    }
    return instreg.instruction;
  }
  else if(opcode[value] == 0xF4) {
    instreg.instruction = HLT;
    return instreg.instruction;
  }
  else if(opcode[value] == 0xFA) {
    instreg.instruction = CLI;
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

  if(opcode[value] == 0x03)
  {
    if(opcode[value+1] >= 0x18 && opcode[value+1] <= 0x1e)
    {
      // EBX+IMM32 starts at enum 34
      // 34 - 24 = 10, so increment by 10.
      instreg.registr = opcode[value+1] + 0xA;
    }
    else if(opcode[value+1] == 0x5d) {
      instreg.registr = EBXEBP;
    }
    else {
      instreg.registr = EIP;
    }
    return instreg.registr;
  }
  else if(opcode[value] == 0x0e) {
    instreg.registr = CS;
    return instreg.registr;
  }
  else if(opcode[value] == 0x42) {
    instreg.registr = EDX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x50) {
    instreg.registr = EAX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x51) {
    instreg.registr = ECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x72 && opcode[value+1] == 0x32) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0x75) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0x91 && (opcode[value+1] != 0x04
  && opcode[value+2] != 0x08)) {
    instreg.registr = EAXECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x92) {
    instreg.registr = EAXEDX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x93) {
    instreg.registr = EAXEBX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x68 || opcode[value] == 0xcd) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0x31 && opcode[value+1] == 0xc0) {
    instreg.registr = EAX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x31 && opcode[value+1] == 0xc9) {
    instreg.registr = ECX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x39 && opcode[value+1] == 0xd0) {
    instreg.registr = EAXEDX;
    return instreg.registr;
  }
  else if(opcode[value] >= 0x48 && opcode[value] <= 0x4f) {
    instreg.registr = opcode[value] - 0x48;
    return instreg.registr;
  }
  else if(opcode[value] == 0x83) {
    if(opcode[value+1] == 0xc1)
    {
      instreg.registr = ECX;
    }
    else if(opcode[value+1] == 0xc3) {
      instreg.registr = EBX;
    }
    else if(opcode[value+1] == 0xec) {
      instreg.registr = ESP;
    }
    return instreg.registr;
  }
  else if(opcode[value] == 0x8D && opcode[value+1] == 0x44
  && opcode[value+2] == 0x24) {
    instreg.registr = EAXESP;
    return instreg.registr;
  }
  else if(opcode[value] == 0x66 && (opcode[value+1] >= 0x98
    && opcode[value+1] <= 0x99)) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0x99) {
    instreg.registr = SPEC;
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
  else if(opcode[value] == 0xb1 && opcode[value+1] >= 0x00) {
    instreg.registr = CL;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb2 && opcode[value+1] >= 0x00) {
    instreg.registr = DL;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb3 && opcode[value+1] >= 0x00) {
    instreg.registr = BL;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb4 && opcode[value+1] >= 0x00) {
    instreg.registr = AH;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb5 && opcode[value+1] >= 0x00) {
    instreg.registr = CH;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb6 && opcode[value+1] >= 0x00) {
    instreg.registr = DH;
    return instreg.registr;
  }
  else if(opcode[value] == 0xb8 && opcode[value+1] >= 0x00) {
    instreg.registr = EAX;
    return instreg.registr;
  }
  // value+1 will be the offset variable of ECX
  else if(opcode[value] == 0xb9) {
    instreg.registr = ECXOFFSET;
    return instreg.registr;
  }
  else if(opcode[value] == 0xba && opcode[value+1] >= 0x00
  && opcode[value-1] != 0x66) { // mov dx and mov edx dont merge
    instreg.registr = EDX;
    return instreg.registr;
  }
  else if(opcode[value] == 0xbb && opcode[value+1] >= 0x00) {
    instreg.registr = EBX;
    return instreg.registr;
  }
  else if(opcode[value] == 0x66 && opcode[value+1] >= 0xba) {
    if(opcode[value+1] == 0xbe)
    {
      instreg.registr = SI;
    }
    else {
      instreg.registr = DX;
    }
    return instreg.registr;
  }
  else if(opcode[value] == 0x89 && opcode[value+1] == 0xe3) {
    instreg.registr = EBXESP;
    return instreg.registr;
  }
  else if(opcode[value] == 0x89 && opcode[value+1] == 0x1d) {
    instreg.registr = EBX;
    return instreg.registr;
  }
  else if(opcode[value] == 0xc1 && opcode[value+1] == 0xe8) {
    instreg.registr = EAX;
    return instreg.registr;
  }
  else if(opcode[value] == 0xe9 || opcode[value] == 0xea || opcode[value] == 0xeb) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0xf3 && (opcode[value+1] == 0xaa || opcode[value+1] == 0xab)) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else if(opcode[value] == 0xf4 || opcode[value] == 0xfa) {
    instreg.registr = SPEC;
    return instreg.registr;
  }
  else {
    instreg.registr = EIP;
    return instreg.registr;
  }
  return EIP;
}

void printAssemblyCode(const char* instr, const char* reg, unsigned char* bytes_read,
  int i, unsigned int start_address)
{
  FILE *file;
  struct storeSplitStr storedStr;

  // Skip anything that returns null
  if(strncmp("EIP", reg, strlen(reg)) != 0)
  {
    UNHIDE_STRING(filename);
    file = fopen(filename, "a");
    HIDE_STRING(filename);

    fprintf(file, "0x00000%x:", start_address);
    printf("0x00000%x:", start_address);

    if(strncmp("ADD", instr, strlen(instr)) == 0)
    {
      if(bytes_read[i] == 0x03)
      {
        storedStr = splitStrStart(reg, " ");
        // bytes_read[i + 1] will be the register value
        printf("\t%s\t\t%s, [%s]\n", instr, storedStr.reg1, storedStr.reg2);
        fprintf(file, "\t%s\t\t%s, [%s]\n", instr, storedStr.reg1, storedStr.reg2);
      }
      else {
        // bytes_read[i + 1] will be the register value
        printf("\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
        fprintf(file, "\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
      }
    }
    else if(strncmp("CMP", instr, strlen(instr)) == 0)
    {
      storedStr = splitStrStart(reg, " ");
      printf("\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
      fprintf(file, "\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
    }
    else if(strncmp("LEA", instr, strlen(instr)) == 0) {
      storedStr = splitStrStart(reg, " ");
      printf("\t%s\t\t%s, [%s+0x%x]\n", instr, storedStr.reg1, storedStr.reg2,
        bytes_read[i+3]);
      fprintf(file, "\t%s\t\t%s, [%s+0x%x]\n", instr, storedStr.reg1, storedStr.reg2,
        bytes_read[i+3]);
    }
    else if(strncmp("MOV", instr, strlen(instr)) == 0) {
      if(bytes_read[i] == 0x66)
      {
        printf("\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
        fprintf(file, "\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
      }
      else if(bytes_read[i] == 0x89) {
        // once we hit an 0x89 opcode, we have series range of what gets mov'd
        if(bytes_read[i+1] == 0xe3)
        {
          storedStr = splitStrStart(reg, " ");
          printf("\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
          fprintf(file, "\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
        }
        else if(bytes_read[i+1] == 0x1d) {
          printf("\t%s\t\tdword ptr 0x%x%x%x%x, %s\n", instr,
            bytes_read[i+2], bytes_read[i+3], bytes_read[i+4], bytes_read[i+5],
            reg);
          fprintf(file, "\t%s\t\tdword ptr 0x%x%x%x%x, %s\n", instr,
            bytes_read[i+2], bytes_read[i+3], bytes_read[i+4], bytes_read[i+5],
            reg);
        }
      }
      else if(bytes_read[i] == 0xb9) {
        // i+1 will be the offset variable
        storedStr = splitStrStart(reg, " ");
        if(bytes_read[i+2] == 0x91 && bytes_read[i+3] == 0x04
        && bytes_read[i+4] == 0x08)
        {
          printf("\t%s\t\t%s, %s [var]\n", instr, storedStr.reg1, storedStr.reg2);
          fprintf(file, "\t%s\t\t%s, %s [var]\n", instr, storedStr.reg1, storedStr.reg2);
        }
        // i+1 will be the bytes being moved into ecx
        else {
          printf("\t%s\t\t%s, 0x%x%x%x%x\n", instr, storedStr.reg1, (int)bytes_read[i+1],
            (int)bytes_read[i+2], (int)bytes_read[i+3], (int)bytes_read[i+4]);
          fprintf(file, "\t%s\t\t%s, 0x%x%x%x%x\n", instr, storedStr.reg1, (int)bytes_read[i+1],
            (int)bytes_read[i+2], (int)bytes_read[i+3], (int)bytes_read[i+4]);
        }
        i += 5;
      }
      else if(bytes_read[i] == 0xbb && (bytes_read[i+1] >= 0x00 &&
        bytes_read[i+1] <= 0x10)) {
        printf("\t%s\t\t%s, 0x%x%x%x%x\n", instr, reg, (int)bytes_read[i+1],
        (int)bytes_read[i+2], (int)bytes_read[i+3], (int)bytes_read[i+4]);
        fprintf(file, "\t%s\t\t%s, 0x%x%x%x%x\n", instr, reg, (int)bytes_read[i+1],
        (int)bytes_read[i+2], (int)bytes_read[i+3], (int)bytes_read[i+4]);
      }
      // just mov'ing a number into a register
      else {
        printf("\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+1]);
        fprintf(file, "\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+1]);
      }
      i += 1;
    }
    else if(strncmp("XCHG", instr, strlen(instr)) == 0) {
      // series of opcodes for xchg we will have to move in
      if(bytes_read[i] >= 0x91 && bytes_read[i] <= 0x93)
      {
        storedStr = splitStrStart(reg, " ");
        printf("\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
        fprintf(file, "\t%s\t\t%s, %s\n", instr, storedStr.reg1, storedStr.reg2);
      }
    }
    else if(strncmp("XOR", instr, strlen(instr)) == 0) {
      printf("\t%s\t\t%s, %s\n", instr, reg, reg);
      fprintf(file, "\t%s\t\t%s, %s\n", instr, reg, reg);
    }
    // if SH, then we know its going to be either SHL or SHR
    else if(strncmp("S", &instr[0], 1) == 0 && strncmp("H", &instr[1], 1) == 0) {
      // bytes_read[i + 1] will be the register value
      printf("\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
      fprintf(file, "\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
    }
    else if(strncmp("SPEC", reg, strlen(reg)) == 0) {
      if(strncmp("PUSH", instr, strlen(reg)) == 0)
      {
        printf("\t%s\t\t0x%x%x%x%x\n", instr, (int)bytes_read[i+4], (int)bytes_read[i+3],
            (int)bytes_read[i+2], (int)bytes_read[i+1]);
        fprintf(file, "\t%s\t\t0x%x%x%x%x\n", instr, (int)bytes_read[i+4],
            (int)bytes_read[i+3], (int)bytes_read[i+2], (int)bytes_read[i+1]);
        // jump further into the loop, i jumps four bytes
        i += 4;
      }
      else if(strncmp("JB", instr, strlen(instr)) == 0) {
        if(bytes_read[i+1] == 0x32)
        {
          printf("\t%s\t\tshort [sub_func]\n", instr);
          fprintf(file, "\t%s\t\tshort [sub_func]\n", instr);
        }
        i += bytes_read[i+1];
      }
      else if(strncmp("JMP", instr, strlen(instr)) == 0 ||
      strncmp("JNZ", instr, strlen(instr)) == 0) {
        if(bytes_read[i+1] >= 0x01)
        {
          printf("\t%s\t\t0x%x%x\n", instr, bytes_read[i+1], bytes_read[i+2]);
          fprintf(file, "\t%s\t\t0x%x%x\n", instr, bytes_read[i+1], bytes_read[i+2]);
        }
        i += bytes_read[i+1];
      }
      else if(strncmp("INT", instr, strlen(reg)) == 0) {
        // INT (interrupt) can have numbers ranging from 0x01 to 0x80
        printf("\t%s\t\t0x%x\n", instr, (int)bytes_read[i+1]);
        fprintf(file, "\t%s\t\t\t0x%x\n", instr, (int)bytes_read[i+1]);
        // jump further into the loop, i jumps four bytes
        i += 1;
      }
      else if(strncmp("STOS", instr, 4) == 0) {
        if(bytes_read[i+1] == 0xAB)
        {
          printf("\tREP %s\t\tDWORD PTR ES:[EDI], EAX\n", instr);
          fprintf(file, "\tREP %s\t\t\tDWORD PTR ES:[EDI], EAX\n", instr);
        }
        else {
          printf("\tREP %s\t\tBYTE PTR ES:[EDI], AL\n", instr);
          fprintf(file, "\tREP %s\t\t\tBYTE PTR ES:[EDI], AL\n", instr);
        }
        i += 1;
      }
      else {
        printf("\t%s\n", instr);
        fprintf(file, "\t%s\n", instr);
      }
    }
    else if(strncmp("SUB", instr, strlen(instr)) == 0) {
      // bytes_read[i + 1] will be the register value
      printf("\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
      fprintf(file, "\t%s\t\t%s, 0x%x\n", instr, reg, (int)bytes_read[i+2]);
    }
    else {
      printf("\t%s\t\t%s\n", instr, reg);
      fprintf(file, "\t%s\t\t\t%s\n", instr, reg);
    }
    fclose(file);
  }
}

void show_strings(FILE *fp, char *argv[])
{
  int ch;
  size_t i = 0;

  fp = fopen(argv[1], "r");
  if(fp == NULL)
  {
      fileNotExist();
      exit(0);
  }

  stringBanner();
  while((ch = getc(fp)) != EOF)
  {
    if(ch >= 32 && ch <= 127)
    {
      printf("%c", ch);
    }
    if(ch == '\n' || ch == '\0')
    {
       i++;
       if(i >= 15)
       {
         printf("\n");
         i = 0;
       }
    }
  }
  printf("\n");
  fclose(fp);
}
