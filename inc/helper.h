#include <stdbool.h>
#include <time.h>

#define ELF_HEADER        0x7f454c46
#define PE_HEADER         0x4d5a9000
#define MACHO_HEADER      0xcffaedfe

#define ELF_EOFILE        0x74657874
#define ELF_EOFUNC1       0xE910FFFF
#define ELF_EOFUNC2       0xC3669066
#define ELF_EOFUNC3       0x66906690
#define ELF_EOFUNC4       0x90669090

#define PE_EOFILE         0xB86F4100
#define PE_EOFILE_DLL     0x38610010
#define PE_EOFUNC1        0xC38D7600
#define PE_EOFUNC2        0x8B442404
#define PE_EOFUNC3        0x8D762600

bool usage(int argc, char* argv, char* argve);
unsigned int checkFileType(unsigned char *bytes_read);
bool checkEndOfFile(unsigned char *bytes_read, int value);
void labelFunctions(unsigned char *bytes_read, int value, unsigned int startAddr);
struct storeSplitStr splitStrStart(const char * strToSplit, const char * delim);
