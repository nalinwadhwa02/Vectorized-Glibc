# VZEROUPPER_RETURN

- [TODO]: Not sure which return codeblock is most widely used. For now, going with this.

Description: Zero upper vector registers and return with xtest.  NB: Use VZEROALL
to avoid RTM abort triggered by VZEROUPPER inside transactionally. Zeroing registers is good for performance (see Section 6.3 of [Agner Fog's Calling Conventions](https://www.agner.org/optimize/calling_conventions.pdf)).

Code:
```{c}
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

```{asm}
vpcmpeq (%rloc), %creg, %ymm1
vpmovmskb %ymm1, %eax
testl %eax, %eax
```

Example Usage:

- `strlen()` main loop. Assume `%ymm0` has all zeroes, `%rdi` has the start location, `VEC_SIZE` is 32.
```{asm}
L(main_loop):
    search-k(rloc=rdi, creg=ymm0)
    subq $-(VEC_SIZE), %rdi // [doubt]: not sure why. see question 2.
    jz L(main_loop)
    tzcntl %eax, %eax
    VZEROUPPER_RETURN
```