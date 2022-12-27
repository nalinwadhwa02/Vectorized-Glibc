# ZERO_LOWER

Params:

- `reg-num`: Number of 256-bit register whose lower 128 bits will be zeroed.

Description: Zero out the lower 128 bits of a `ymm` register.

Code:

```asm
vpxor %xmm{reg-num} %xmm{reg-num}
```

# VZEROUPPER_RETURN

- [TODO]: Not sure which return codeblock is most widely used. For now, going with this.

Description: Zero upper vector registers and return with xtest.  NB: Use VZEROALL
to avoid RTM abort triggered by VZEROUPPER inside transactionally. Zeroing registers is good for performance (see Section 6.3 of [Agner Fog's Calling Conventions](https://www.agner.org/optimize/calling_conventions.pdf)).

Code:

```c
#define ZERO_UPPER_VEC_REGISTERS_RETURN_XTEST \
	xtest;							\
	jnz	1f;						\
	vzeroupper;						\
	ret;							\
1:								\
	vzeroall;						\
	ret
```

# cmp-mem-reg-32

Params:

- `rloc`: 64-bit register containing the start location.
- `creg`: 256-bit register containing the 32 bytes which will be compared with the 32 bytes starting at location `(%rloc)`.

Description: Compare 32 bytes at memory location pointed to by `%rloc` with the corresponding 32 bytes in the `%creg` 256-bit register.

Code:

```asm
vpcmpeq (%rloc), %creg, %ymm1
vpmovmskb %ymm1, %eax
testl %eax, %eax
```


Example Usage:

- `strlen()` main loop. Assume `%ymm0` has all zeroes, `%rdi` has the start location, `VEC_SIZE` is 32.

```asm
L(main_loop):
    cmp-mem-reg-32(rloc=rdi, creg=ymm0)
    subq $-(VEC_SIZE), %rdi // [doubt]: not sure why. see question 2.
    jz L(main_loop)
    tzcntl %eax, %eax
    VZEROUPPER_RETURN
```

# SEARCH_4x

Params:

- `VEC_SIZE`: Number of bytes in a vector register. This is 32 for our purposes.
- `CHAR_SET`: Characters to match against. This can be of three forms - `{\0}`, `{c}` and `{\0,c}` where `c` is any character.
- `rloc`: Register containing the location from which to begin search. Eg, `rdi`.
- `ymm_creg`: In case `CHAR_SET` contains `c`, `creg` is a vector register whose each byte is equal to `c`.
- `ymm_zreg`: A vector register with all bytes set to zero.
- `VPMINU`: `vpminub` if normal (8-bit) characters, `vpminud` for wide characters.
- `VPCMPEQ`: `vpcmpeqb` if normal (8-bit) characters, `vpcmpeqd` for wide characters.

Description: Search the next 4*`VEC_SIZE` bytes for a given set of characters of size <=2. After the end, locations `[%rloc, %rloc + 4*VEC_SIZE)` have a character in `CHAR_SET` iff the `ZF` flag is not set.

Code: (Currently only the case where `CHAR_SET={\0,c}` is covered)

```asm
vmovdqa	(%rloc), %ymm6
vmovdqa	VEC_SIZE(%rloc), %ymm7

/* Leaves only CHARS matching esi as 0.	 */
vpxor	%ymm6, %ymm_creg, %ymm2
vpxor	%ymm7, %ymm_creg, %ymm3

VPMINU	%ymm2, %ymm6, %ymm2 // ymm2 has a 0 iff bytes 0-31 have a NULL byte or CHAR byte
VPMINU	%ymm3, %ymm7, %ymm3

vmovdqa	(VEC_SIZE * 2)(%rloc), %ymm6
vmovdqa	(VEC_SIZE * 3)(%rloc), %ymm7

vpxor	%ymm6, %ymm_creg, %ymm4
vpxor	%ymm7, %ymm_creg, %ymm5

VPMINU	%ymm4, %ymm6, %ymm4
VPMINU	%ymm5, %ymm7, %ymm5

VPMINU	%ymm2, %ymm3, %ymm6
VPMINU	%ymm4, %ymm5, %ymm7

VPMINU	%ymm6, %ymm7, %ymm7

VPCMPEQ	%ymm7, %ymm_zreg %ymm7
vpmovmskb %ymm7, %ecx
testl	%ecx, %ecx
```

# temp

### cmp-cascade-mem-reg-32

Description: Cascadable version of the cmp-mem-reg-32 codeblock

Code: 

```asm
vpcmpeq %ymm0, %creg, %ymm1
vpmovmskb %ymm1, %eax
orl %eax, %ebx
testl %ebx, %ebx
```