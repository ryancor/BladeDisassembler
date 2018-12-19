#include "../inc/emu.h"
#include "../inc/disass.h"

// Memory Storage
uint16_t memory[UINT32_MAX];

// Register Storage
uint16_t reg[COUNT];

uint16_t sign_extend(uint16_t x, int bit_count)
{
  if((x >> (bit_count -1)) & 1)
  {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

void update_eflags(uint16_t r)
{
  if(reg[r] == 0)
  {
    reg[COND] = EFL_ZRO;
  }
  else if(reg[r] >> 15) {
    // a 1 in the left most bit indicates negative
    reg[COND] = EFL_NEG;
  }
  else {
    reg[COND] = EFL_POS;
  }
}

void print_current_registers(uint16_t eax, uint16_t ebx, uint16_t ecx,
  uint16_t edx, uint16_t esi, uint16_t edi, uint16_t ebp, uint16_t esp,
  uint16_t eip)
{
  printf("EAX => 0x%x\n", eax);
  printf("EBX => 0x%x\n", ebx);
  printf("ECX => 0x%x\n", ecx);
  printf("EDX => 0x%x\n", edx);
  printf("ESI => 0x%x\n", esi);
  printf("EDI => 0x%x\n", edi);
  printf("EBP => 0x%x\n", ebp);
  printf("ESP => 0x%x\n", esp);
  printf("EIP => 0x%x\n", eip);
}

void run_debugger()
{
  printf("Debugger\n");
  print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
  reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
}
