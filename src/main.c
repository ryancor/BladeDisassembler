#include "inc/disass.h"

void usage(int argc, char* argv);

int main(int argc, char** argv)
{
  usage(argc, argv[0]);

  FILE *fp;
  unsigned char *bytes_read;
  unsigned int header;
  long fileLen;
  const char *instr;
  const char *reg;

  fp = fopen(argv[1], "r");
  if(fp == NULL)
  {
      printf("File does not exit\n");
      exit(0);
  }

  if(fseek(fp, 0, SEEK_END) == -1) // jump to end of file
  {
    printf("Can't read bytes from file\n");
    exit(0);
  }

  fileLen = ftell(fp);
  if(fileLen <= 0)
  {
    printf("File is empty\n");
    exit(0);
  }

  rewind(fp); // jump back to beginning of file
  bytes_read = (unsigned char *)malloc((fileLen + 1) * sizeof(char));
  fread(bytes_read, fileLen, 1, fp); // read entire file

  header = (bytes_read[0] << 24) | (bytes_read[1] << 16)| (bytes_read[2] << 8 )
          | bytes_read[3];

  if(header == 0x7f454c46)
  {
    printf("[+] ELF Magic Header Found: 0x%x\n", header);
  }
  printf("========== Blade Disassembler ==========\n");

  for(int i = 0; i < fileLen; i++)
  {
    // read past the binary headers
    if(i > 0x10F)
    {
      instr = stringFromInstruction(ReturnInstructionNumber(bytes_read, i));
      reg = stringFromRegisters(ReturnRegisterNumber(bytes_read, i));

      printAssemblyCode(instr, reg, bytes_read, i);
    }
  }

  fclose(fp);
  return 0;
}

void usage(int argc, char* argv)
{
  if(argc < 2)
  {
    printf("Usage: %s <binary_file>\n", argv);
    exit(0);
  }
}