#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ReturnInstructionNumber(unsigned char* opcode, int value);
int ReturnRegisterNumber(unsigned char* opcode, int value);
void printAssemblyCode(const char* instr, const char* reg,
  unsigned char* bytes_read,
  int i,
  unsigned int start_address
);

struct instrReg {
  int instruction;
  int registr;
};

struct storeSplitStr {
  char *reg1;
  char *reg2;
};

enum instructions {
  AAA, AAD, AAM, AAS, ADC, ADD, AND, CALL, CBW, CLC, CLD, CLI, CMC, CMP, CMPSB,
  CMPSW, CWD, DAA, DAS, DEC, DIV, ESC, HLT, IDIV, IMUL, IN, INC, INT, INTO,
  IRET, JA, JAE, JB, JBE, JC, JE, JG, JGE, JL, JLE, JNA, JNAE, JNB, JNBE, JNC,
  JNE, JNG, JNGE, JNL, JNLE, JNO, JNP, JNS, JNZ, JO, JP, JPE, JPO, JS, JZ, JCXZ,
  JMP, LAHF, LDS, LEA, LES, LOCK, LODSB, LODSW, LOOP, MOV, MOVSB, MOVSW, MUL,
  NEG, NOP, NOT, OR, OUT, POP, POPF, PUSH, PUSHF, RCL, RCR, RET, RETN, RETF,
  ROL, ROR, SAHF, SAL, SAR, SBB, SCASB, SCASW, SHL, SHR, STC, STD, STI, STOSB,
  STOSW, SUB, TEST, WAIT, XCHG, XLAT, XOR
};

static inline const char *stringFromInstruction(enum instructions i)
{
  static const char *instr[] = {
    "AAA", "AAD", "AAM", "AAS", "ADC", "ADD", "AND", "CALL", "CBW", "CLC", "CLD",
    "CLI", "CMC", "CMP", "CMPSB", "CMPSW", "CWD", "DAA", "DAS", "DEC", "DIV", "ESC",
    "HLT", "IDIV", "IMUL", "IN", "INC", "INT", "INTO", "IRET", "JA", "JAE", "JB",
    "JBE", "JC", "JE", "JG", "JGE", "JL", "JLE", "JNA", "JNAE", "JNB", "JNBE", "JNC",
    "JNE", "JNG", "JNGE", "JNL", "JNLE", "JNO", "JNP", "JNS", "JNZ", "JO", "JP",
    "JPE", "JPO", "JS", "JZ", "JCXZ", "JMP", "LAHF", "LDS", "LEA", "LES", "LOCK",
    "LODSB", "LODSW", "LOOP", "MOV", "MOVSB", "MOVSW", "MUL", "NEG", "NOP", "NOT",
    "OR", "OUT", "POP", "POPF", "PUSH", "PUSHF", "RCL", "RCR", "RET", "RETN", "RETF",
    "ROL", "ROR", "SAHF", "SAL", "SAR", "SBB", "SCASB", "SCASW", "SHL", "SHR", "STC",
    "STD", "STI", "STOSB", "STOSW", "SUB", "TEST", "WAIT", "XCHG", "XLAT", "XOR"
  };

  return instr[i];
}

enum registers {
  EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP, EIP, AX, BX, CX, DX, SI, DI, BP, SP,
  AH, AL, BH, BL, CH, CL, DH, DL, SPEC, EAXEBX, EAXECX, EAXEDX, ECXOFFSET, EBXESP
};

static inline const char *stringFromRegisters(enum registers r)
{
  static const char *reg[] = {
    "EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP", "EIP", "AX", "BX", "CX",
    "DX", "SI", "DI", "BP", "SP", "AH", "AL", "BH", "BL", "CH", "CL", "DH", "DL",
    "SPEC", "EAX EBX", "EAX ECX", "EAX EDX", "ECX OFFSET", "EBX ESP"
  };

  return reg[r];
}
