#include <stdbool.h>

bool usage(int argc, char* argv, char* argve);
unsigned int checkFileType(unsigned char *bytes_read);
struct storeSplitStr splitStrStart(const char * strToSplit, const char * delim);
