#include "../inc/disass.h"
#include "../inc/helper.h"

bool usage(int argc, char* argv, char* argve)
{
  if(argc < 2)
  {
    printf("Usage: %s <binary_file>\n", argv);
    exit(-1);
  }
  if(argc == 3)
  {
    if(strncmp(argve, "-g", 2) == 0)
    {
      return true;
    }
    else {
      printf("More Usage: %s <binary_file> -g\n", argv);
      exit(-1);
    }
  }
  return false;
}

unsigned int checkFileType(unsigned char *bytes_read)
{
  unsigned int header;
  unsigned int code_address = 0x000;

  header = (bytes_read[0] << 24) | (bytes_read[1] << 16)| (bytes_read[2] << 8 )
          | bytes_read[3];

  if(header == 0x7f454c46)
  {
    printf("[+] ELF Magic Header Found: 0x%x\n", header);
    code_address += 0x10F;
  }
  else if(header == 0x4d5a9000) {
    printf("[+] PE Magic Header Found: 0x%x\n", header);
    code_address += 0x10F;
  }
  else if(header == 0xcffaedfe) {
    printf("[+] Mach-O Magic Header Found: 0x%x\n", header);
    code_address += 0x10F;
  }
  else {
    printf("[!] Could not find a proper header, results may vary\n");
  }
  printf("========== Blade Disassembler ==========\n");

  return code_address;
}

// store multiple values
struct storeSplitStr splitStrStart(const char * strToSplit, const char * delim)
{
  struct storeSplitStr storeStr;
  int i = 0;
  char *copy = strdup(strToSplit);
  storeStr.reg1 = strtok(copy, delim);
  storeStr.reg2 = strtok(NULL, delim);

  return storeStr;
}
