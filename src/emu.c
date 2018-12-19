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
  printf("EAX => 0x%08x\n", eax);
  printf("       AX => 0x%08x\n", eax & 0xFFFF); // get lower most bits, to get
                                               // higher, we do eax >> 16
  printf("             AH => 0x%08x\n", ((eax >> 8) & 0xff)); // get higher least bits
  printf("             AL => 0x%08x\n", eax & 0xFF); // get lower least bits
  printf("EBX => 0x%08x\n", ebx);
  printf("       BX => 0x%08x\n", ebx & 0xFFFF);
  printf("             BH => 0x%08x\n", ((ebx >> 8) & 0xff));
  printf("             BL => 0x%08x\n", ebx & 0xFF);
  printf("ECX => 0x%08x\n", ecx);
  printf("       CX => 0x%08x\n", ecx & 0xFFFF);
  printf("             CH => 0x%08x\n", ((ecx >> 8) & 0xff));
  printf("             CL => 0x%08x\n", ecx & 0xFF);
  printf("EDX => 0x%08x\n", edx);
  printf("       DX => 0x%08x\n", edx & 0xFFFF);
  printf("             DH => 0x%08x\n", ((edx >> 8) & 0xff));
  printf("             DL => 0x%08x\n", edx & 0xFF);
  printf("ESI => 0x%08x\n", esi);
  printf("       SI => 0x%08x\n", esi & 0xFFFF); // only 16 bit register for ESI
  printf("EDI => 0x%08x\n", edi);
  printf("       DI => 0x%08x\n", edi & 0xFFFF); // only 16 bit register for EDI
  printf("EBP => 0x%08x\n", ebp);
  printf("       BP => 0x%08x\n", ebp & 0xFFFF);
  printf("ESP => 0x%08x\n", esp);
  printf("       SP => 0x%08x\n", esp & 0xFFFF);
  printf("EIP => 0x%08x\n", eip);
}

void run_debugger()
{
  printf("Debugger\n");
  print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
  reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
}
