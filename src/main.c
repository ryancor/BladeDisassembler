#include "inc/gui.h"
#include "inc/emu.h"
#include "inc/disass.h"
#include "inc/helper.h"
#include "inc/obfuscate.h"

int main(int argc, char** argv)
{
  int option_flag = 0;
  option_flag = usage(argc, argv[0], argv[2]);
  option_flag >= 2 ? ({goto end_of_loop;}) : false;

  FILE *fp;
  unsigned char *bytes_read;
  long fileLen;
  const char *instr;
  const char *reg;

  fp = fopen(argv[1], "r");
  if(fp == NULL)
  {
      fileNotExist();
      exit(0);
  }

  if(fseek(fp, 0, SEEK_END) == -1) // jump to end of file
  {
    fileNotReadable();
    exit(0);
  }

  fileLen = ftell(fp);
  if(fileLen <= 0)
  {
    fileEmpty();
    exit(0);
  }

  rewind(fp); // jump back to beginning of file
  bytes_read = (unsigned char *)malloc((fileLen + 1) * sizeof(char));
  fread(bytes_read, fileLen, 1, fp); // read entire file

  unsigned int start_address = checkFileType(bytes_read);

  printEncryptedString("_start:");
  for(int i = 0; i < fileLen; i++)
  {
    // read past the binary headers
    if(i >= start_address)
    {
      instr = stringFromInstruction(ReturnInstructionNumber(bytes_read, i));
      reg = stringFromRegisters(ReturnRegisterNumber(bytes_read, i));

      start_address++;
      // check for new function start
      labelFunctions(bytes_read, i, start_address);
      printAssemblyCode(instr, reg, bytes_read, i, start_address);

      // symbol for .text is found, we know program is finished
      if(checkEndOfFile(bytes_read, i))
      {
        goto end_of_loop;
      }
    }
  }
end_of_loop:
  if(option_flag == 1)
  {
    show_gui_init(argc, argv);
  }
  else if(option_flag == 2) {
    show_strings(fp, argv);
  }
  else if(option_flag == 3) {
    run_debugger(argv[1]);
  }

  UNHIDE_STRING(filename);
  remove(filename); // delete blade log file
  HIDE_STRING(filename);

  fclose(fp);
  return 0;
}
