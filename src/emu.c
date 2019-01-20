#include "../inc/emu.h"
#include "../inc/disass.h"

// Memory Storage
uint16_t memory[UINT32_MAX];

// Register Storage
uint16_t reg[COUNT];

uint16_t sign_extend(uint16_t instr, int bit_count)
{
  if((instr >> (bit_count -1)) & 1)
  {
    instr |= (0xFFFF << bit_count);
  }
  return instr;
}

uint16_t swap16(uint16_t x)
{
  return (x << 8) | (x >> 8);
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

// Read Image File
void read_image_file(FILE* file)
{
  // the origin tells us where in memory to place the image
  uint16_t origin;
  fread(&origin, sizeof(origin), 1, file);

  printf("[!] Original Origin: 0x%x\n", origin);

  origin = swap16(origin);

  printf("[+] New Origin: 0x%x\n", origin);

  // we know the max file size so we only need one fread
  uint16_t max_read = UINT16_MAX - origin;
  uint16_t *pMemOr = memory + origin;
  size_t read = fread(pMemOr, sizeof(uint16_t), max_read, file);

  // swap to little endian
  while(read-- > 0)
  {
    // &pMemOr will be constant memory address of 0x7fff5460a3e8
    *pMemOr = swap16(*pMemOr);
    // Values or bytes from file being stored at &pMemOr will be stored like 0x100f098a
    ++pMemOr;
  }
  printf("[+] New Memory Origin Location (data successfully stored): %p\n", &pMemOr);
}

// Read Image
int read_image(const char *image_path)
{
	FILE *file = fopen(image_path, "rb");
	if(!file)
	{
		return 0;
	}

	read_image_file(file);
	fclose(file);
	return 1;
}

// check key
uint16_t check_key()
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

// memory access
void mem_write(uint16_t address, uint16_t val)
{
	memory[address] = val;
}

uint16_t mem_read(uint16_t address)
{
	if(address == MR_KBSR)
	{
		memory[MR_KBSR] = (1 << 15); // get the MSB address I.E. 0x0804067a == 0x8000
		memory[MR_KBDR] = getchar();
	}
	else {
		memory[MR_KBSR] = 0;
	}
	return memory[address];
}

void handle_interrupt(int signal)
{
  printf("\n");
  exit(signal);
}

void print_current_registers(uint16_t eax, uint16_t ebx, uint16_t ecx,
  uint16_t edx, uint16_t esi, uint16_t edi, uint16_t ebp, uint16_t esp,
  uint16_t eip)
{
  system("clear");

  printf("EAX => 0x%08x\t\t\t", eax);
  printf("       AX => 0x%08x\t", eax & 0xFFFF); // get lower most bits, to get
                                               // higher, we do eax >> 16
  printf("             AH => 0x%04x\t", ((eax >> 8) & 0xff)); // get higher least bits
  printf("             AL => 0x%04x\n", eax & 0xFF); // get lower least bits
  printf("EBX => 0x%08x\t\t\t", ebx);
  printf("       BX => 0x%08x\t", ebx & 0xFFFF);
  printf("             BH => 0x%04x\t", ((ebx >> 8) & 0xff));
  printf("             BL => 0x%04x\n", ebx & 0xFF);
  printf("ECX => 0x%08x\t\t\t", ecx);
  printf("       CX => 0x%08x\t", ecx & 0xFFFF);
  printf("             CH => 0x%04x\t", ((ecx >> 8) & 0xff));
  printf("             CL => 0x%04x\n", ecx & 0xFF);
  printf("EDX => 0x%08x\t\t\t", edx);
  printf("       DX => 0x%08x\t", edx & 0xFFFF);
  printf("             DH => 0x%04x\t", ((edx >> 8) & 0xff));
  printf("             DL => 0x%04x\n", edx & 0xFF);
  printf("ESI => 0x%08x\t\t\t", esi);
  printf("       SI => 0x%08x\n", esi & 0xFFFF); // only 16 bit register for ESI
  printf("EDI => 0x%08x\t\t\t", edi);
  printf("       DI => 0x%08x\n", edi & 0xFFFF); // only 16 bit register for EDI
  printf("EBP => 0x%08x\t\t\t", ebp);
  printf("       BP => 0x%08x\n", ebp & 0xFFFF);
  printf("ESP => 0x%08x\t\t\t", esp);
  printf("       SP => 0x%08x\n", esp & 0xFFFF);
  printf("EIP => 0x%08x\n\n\n", eip);

  sleep(2);
}

void debugger_loop()
{
  int running = 1;
  while(running)
  {
    // FETCH
    // First loop: EIP would == 0x1001
    uint16_t instr = mem_read(reg[EIP]++);
    uint16_t op = instr >> 12;

    //printf("\n**** OP: %02x\n", op);
    switch(op)
    {
      // OPERAND = AAA
      case 0x37:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t al = reg[eax] & 0xFF;

        if((al & 0x0F) > 9 || EFL_POS)
        {
          reg[al] = reg[al] + 6;
          reg[eax] = reg[al];
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = AAD
      case 0xD5:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ax = reg[eax] & 0xFFFF;
        uint16_t ah = (reg[eax] >> 8) & 0xFF;
        uint16_t al = reg[eax] & 0xFF;

        if((al & 0x0F) > 9 || EFL_POS)
        {
          reg[al] = reg[al] + (10 * reg[ah]);
          reg[ah] &= ~reg[ah];
          reg[ax] = reg[ah] | reg[al];

          reg[eax] = reg[ax];
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = AAM
      case 0xD4:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ah = (reg[eax] >> 8) & 0xFF;
        uint16_t al = reg[eax] & 0xFF;
        uint16_t imm_flag = (instr >> 8) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[ah] = reg[al] / imm8;
          reg[al] = reg[al] % imm8;
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = AAS
      case 0x3F:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ax = reg[eax] & 0xFFFF;
        uint16_t ah = (reg[eax] >> 8) & 0xFF;
        uint16_t al = reg[eax] & 0xFF;

        if((al & 0x0F) > 9 || EFL_POS)
        {
          reg[ax] = reg[ax] - 6;
          reg[ah] = reg[ah] - 1;
          reg[al] = reg[al] & 0x0F;
        }
        else {
          reg[al] = reg[al] & 0x0F;
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = ADC
      case 0x10:
      case 0x11:
      case 0x12:
      case 0x13:
      case 0x14:
      case 0x15:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t imm_flag = (instr >> 8) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[eax] = reg[eax] + imm8 + EFL_ZRO;
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = ADD
      case 0x01:
      case 0x02:
      case 0x03:
      case 0x04:
      case 0x05:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ebx = (instr >> 8) & 0x07;
        uint16_t imm_flag = (instr >> 7) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[eax] = reg[ebx] + imm8;
        }
        else {
          uint16_t ecx = instr & 0x07;
          reg[eax] = reg[ebx] + reg[ecx];
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = AND
      case 0x20:
      case 0x21:
      case 0x22:
      case 0x23:
      case 0x24:
      case 0x25:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ebx = (instr >> 8) & 0x07;
        uint16_t imm_flag = (instr >> 7) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[eax] = reg[ebx] & imm8;
        }
        else {
          uint16_t ecx = instr & 0x07;
          reg[eax] = reg[ebx] & reg[ecx];
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = CALL
      case 0x9A:
      {
        reg[ESP] = reg[EIP];
        reg[ESP]++;
        reg[EIP] = op & 0x0FFF;

        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = DIV
      case 0xF6:
      case 0xF7:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t edx = (instr >> 6) & 0x07;
        uint16_t imm_flag = (instr >> 8) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[eax] = reg[eax] / imm8;
          if(reg[eax] < 0xFFF)
          {
            reg[eax] = eax;
            reg[edx] = reg[eax] % imm8;
          }
        }
        else {
          reg[eax] = reg[eax] / reg[edx];
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = JMP
      case 0xE9:
      case 0xEA:
      case 0xEB:
      {
        // EIP is instruction following JMP instruction
        uint16_t eip_offset = sign_extend((instr) & 0x1ff, 9);

        reg[eip_offset] = reg[EIP] + (op & 0x0FFF);
        reg[EIP] = reg[eip_offset];

        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = LEA
      case 0x8D:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        // mov esp+value into eax
        uint16_t eip_offset = sign_extend((instr) & 0x1ff, 9);

        reg[eax] = reg[EIP] + eip_offset;

        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = MOV
      case 0x88:
      case 0x89:
      case 0x8A:
      case 0x8B:
      case 0x8C:
      case 0x8E:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t imm_flag = (instr >> 8) & 0x01;;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          if(reg[eax] < 0xFFF)
          {
            reg[eax] = reg[imm8];
            reg[EIP] += 2;
          }
        }

        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = MUL/IMUL
      case 0x69:
      case 0x6B:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t imm_flag = (instr >> 8) & 0x01;

        if(imm_flag)
        {
          uint16_t imm8 = sign_extend(instr & 0x1F, 8);
          reg[eax] = reg[eax] * imm8;
          if(reg[eax] < 0xFFF)
          {
            reg[eax] = eax;
            reg[eax] = reg[eax] * imm8;
          }
        }
        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = POP
      case 0x07:
      {
        reg[ESP] = reg[ESP] + 4;

        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = PUSH
      case 0x06:
      {
        reg[ESP] = reg[ESP] - 4;

        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;

      // OPERAND = SHL/SHR
      case 0xc0:
      case 0xc1:
      {
        uint16_t eax = (instr >> 9) & 0x07;
        uint16_t ebx = (instr >> 8) & 0x07;
        uint16_t imm_flag = (instr >> 7) & 0x01;

        uint16_t count = 31; // 32 bit
        uint16_t countMASK = 0x1F;
        uint16_t tempCount = countMASK & count;

        do
        {
          if(imm_flag)
          {
            uint16_t imm8 = sign_extend(instr & 0x1F, 8);
            if(imm8 >= 0xe0 && imm8 <= 0xe5) // if instruction is SHL
            {
              reg[eax] = reg[ebx] << imm8;
            }
            else {
              reg[eax] = reg[ebx] >> imm8;
            }
          }
          else {
            uint16_t ecx = instr & 0x07;
            if(ecx >= 0xe0 && ecx <= 0xe5) // if instruction is SHL
            {
              reg[eax] = reg[ebx] << reg[ecx];
            }
            else {
              reg[eax] = reg[ebx] >> reg[ecx];
            }
          }
          tempCount--;
        } while(tempCount != 0);

        update_eflags(eax);
        print_current_registers(reg[EAX], reg[EBX], reg[ECX], reg[EDX], reg[ESI],
        reg[EDI], reg[EBP], reg[ESP], reg[EIP]);
      }
      break;
    }
  }
}

void run_debugger(const char *image_path)
{
  if(!read_image(image_path))
  {
    printf("[-] Failed to load image: %s\n", image_path);
    exit(1);
  }

  // Setup
	signal(SIGINT, handle_interrupt);

  // set the PC to starting position
  enum { EIP_START = 0x1000 };
  reg[EIP] = EIP_START;

  printf("Start with EIP address of 0x%x\n", reg[EIP]);

  debugger_loop();
}
