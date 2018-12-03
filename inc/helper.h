#include <stdbool.h>

#define ELF_HEADER        0x7f454c46
#define PE_HEADER         0x4d5a9000
#define MACHO_HEADER      0xcffaedfe

#define ELF_EOF           0x74657874

bool usage(int argc, char* argv, char* argve);
unsigned int checkFileType(unsigned char *bytes_read);
bool checkEndOfFile(unsigned char *bytes_read, int value);
struct storeSplitStr splitStrStart(const char * strToSplit, const char * delim);
