#include "inc/disass.h"
#include "inc/helper.h"
#include "inc/gui.h"

int main(int argc, char** argv)
{
  bool gui_flag = FALSE;
  gui_flag = usage(argc, argv[0], argv[2]);

  FILE *fp;
  unsigned char *bytes_read;
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

  unsigned int start_address = checkFileType(bytes_read);

  for(int i = 0; i < fileLen; i++)
  {
    // read past the binary headers
    if(i > start_address)
    {
      instr = stringFromInstruction(ReturnInstructionNumber(bytes_read, i));
      reg = stringFromRegisters(ReturnRegisterNumber(bytes_read, i));

      start_address++;
      printAssemblyCode(instr, reg, bytes_read, i, start_address);
    }
  }

  if(gui_flag == TRUE)
  {
    show_gui_init(argc, argv);
  }
  remove(filename); // delete blade log file

  fclose(fp);
  return 0;
}
