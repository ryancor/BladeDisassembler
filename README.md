# BladeDisassembler
## Usage
```
➜  BladeDisassembler git:(master) ./blade examples/file-reader
[+] ELF Magic Header Found: 0x7f454c46
========== Blade Disassembler ==========
XOR      ECX, ECX
MUL      ECX
MOV      AL, 0x5
PUSH     ECX
PUSH     0x64777373
PUSH     0x61702f63
PUSH     0x74652f2f
MOV      EBX, ESP
INT      0x80
XCHG     EBX, EAX
```
