# BladeDisassembler
## Usage
```
➜  BladeDisassembler git:(master) ./blade examples/file-reader
========== Blade Disassembler ==========
0x00000110:  XOR      ECX, ECX
0x00000112:  MUL      ECX
0x00000114:  MOV      AL, 0x5
0x00000116:  PUSH     ECX
0x00000117:  PUSH     0x64777373
0x0000011c:  PUSH     0x61702f63
0x00000121:  PUSH     0x74652f2f
0x00000126:  MOV      EBX, ESP
0x00000128:  INT      0x80
```
