- Codeblocks for:
  - return
  - zeroing registers
  - cascadable comparision codeblocks
- Changes in existing code blocks
  - make cmp-mem-reg-32 not specific to arguments (remove first args mem dependency)
  - in search_4x, use `VPMINU` macro instead of `vpminub` to handle wide characters also.
  - cover all `CHAR_SET` cases in search_4x

## Update for strcpy
- add copy 4x code block
### Search 4x
- add ```ifdef``` for vpxor to make search for char ```c``` optional
- use unique registers to copy from memory since they will be needed to move to another location ahead (6 register in total now, 4 immutable, 2 reusable)
- for slow path, copy to original code (after finding end of string)  