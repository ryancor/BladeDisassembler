#ifndef OBFUSCATE_H
#define OBFUSCATE_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SECRET_CHAR = 0x01;
#define HIDE_LETTER(x)      (x) ^ SECRET_CHAR
#define UNHIDE_STRING(str)  do { char *ptr = (str); while(*ptr)*ptr++ ^= SECRET_CHAR;} while(0)
#define HIDE_STRING(str)    do { char *ptr = (str); while(*ptr)*ptr++ ^= SECRET_CHAR;} while(0)

static char filename[] = { HIDE_LETTER('s'), HIDE_LETTER('a'), HIDE_LETTER('m'),
  HIDE_LETTER('p'), HIDE_LETTER('l'), HIDE_LETTER('e'), HIDE_LETTER('o'),
  HIDE_LETTER('s'), HIDE_LETTER('l'), HIDE_LETTER('r'), HIDE_LETTER('.'),
  HIDE_LETTER('b'), HIDE_LETTER('l'), HIDE_LETTER('a'), HIDE_LETTER('d'),
  HIDE_LETTER('e'), '\0'
};

void fileNotExist();
void fileEmpty();
void fileNotReadable();
void fileSizeLarge(const char *size);
void printEncryptedString(const char *text);

#ifdef __cplusplus
}
#endif
#endif
