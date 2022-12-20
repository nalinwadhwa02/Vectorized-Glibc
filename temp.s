/* strlen/strnlen/wcslen/wcsnlen optimized with AVX2.
   Copyright (C) 2017-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <isa-level.h>

#if ISA_SHOULD_BUILD (3)

# include <sysdep.h>

# ifndef STRLEN
#  define STRLEN	__strlen_avx2
# endif

#  define VPCMPEQ	vpcmpeqb
#  define VPMINU	vpminub
#  define CHAR_SIZE	1

# ifndef VZEROUPPER
#  define VZEROUPPER	vzeroupper
# endif

# ifndef SECTION
#  define SECTION(p)	p##.avx
# endif

# define VEC_SIZE 32
# define PAGE_SIZE 4096
# define CHAR_PER_VEC	(VEC_SIZE / CHAR_SIZE)

	.section SECTION(.text),"ax",@progbits
ENTRY (STRLEN)
	movl	%edi, %eax
	movq	%rdi, %rdx
	vpxor	%xmm0, %xmm0, %xmm0
	/* Clear high bits from edi. Only keeping bits relevant to page
	   cross check.  */
	andl	$(PAGE_SIZE - 1), %eax
	/* Check if we may cross page boundary with one vector load.  */
	cmpl	$(PAGE_SIZE - VEC_SIZE), %eax
	ja	L(cross_page_boundary)

	/* Check the first VEC_SIZE bytes.  */
	VPCMPEQ	(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	/* If empty continue to aligned_more. Otherwise return bit
	   position of first match.  */
	testl	%eax, %eax
	jz	L(aligned_more)
	tzcntl	%eax, %eax
	VZEROUPPER_RETURN

	.p2align 4
L(first_vec_x1):
	tzcntl	%eax, %eax
	/* Safe to use 32 bit instructions as these are only called for
	   size = [1, 159].  */
	VZEROUPPER_RETURN

	.p2align 4
L(first_vec_x2):
	tzcntl	%eax, %eax
	/* Safe to use 32 bit instructions as these are only called for
	   size = [1, 159].  */
	VZEROUPPER_RETURN

	.p2align 4
L(first_vec_x3):
	tzcntl	%eax, %eax
	/* Safe to use 32 bit instructions as these are only called for
	   size = [1, 159].  */
	VZEROUPPER_RETURN

	.p2align 4
L(first_vec_x4):
	tzcntl	%eax, %eax
	/* Safe to use 32 bit instructions as these are only called for
	   size = [1, 159].  */
	VZEROUPPER_RETURN

	.p2align 5
L(aligned_more):
	/* Align data to VEC_SIZE - 1. This is the same number of
	   instructions as using andq with -VEC_SIZE but saves 4 bytes of
	   code on the x4 check.  */
	orq	$(VEC_SIZE - 1), %rdi/*!doubt*/
L(cross_page_continue):
	/* Check the first 4 * VEC_SIZE.  Only one VEC_SIZE at a time
	   since data is only aligned to VEC_SIZE.  */
	/* Load first VEC regardless.  */
	VPCMPEQ	1(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	testl	%eax, %eax
	jnz	L(first_vec_x1)

	VPCMPEQ	(VEC_SIZE + 1)(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	testl	%eax, %eax
	jnz	L(first_vec_x2)

	VPCMPEQ	(VEC_SIZE * 2 + 1)(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	testl	%eax, %eax
	jnz	L(first_vec_x3)

	VPCMPEQ	(VEC_SIZE * 3 + 1)(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	testl	%eax, %eax
	jnz	L(first_vec_x4)

	/* Align data to VEC_SIZE * 4 - 1.  */
	/* Compare 4 * VEC at a time forward.  */
	.p2align 4
L(loop_4x_vec):
	/* Save some code size by microfusing VPMINU with the load.
	   Since the matches in ymm2/ymm4 can only be returned if there
	   where no matches in ymm1/ymm3 respectively there is no issue
	   with overlap.  */
	vmovdqa	1(%rdi), %ymm1
	VPMINU	(VEC_SIZE + 1)(%rdi), %ymm1, %ymm2
	vmovdqa	(VEC_SIZE * 2 + 1)(%rdi), %ymm3
	VPMINU	(VEC_SIZE * 3 + 1)(%rdi), %ymm3, %ymm4

	VPMINU	%ymm2, %ymm4, %ymm5
	VPCMPEQ	%ymm5, %ymm0, %ymm5
	vpmovmskb %ymm5, %ecx

	subq	$-(VEC_SIZE * 4), %rdi
	testl	%ecx, %ecx
	jz	L(loop_4x_vec)


	VPCMPEQ	%ymm1, %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	subq	%rdx, %rdi
	testl	%eax, %eax
	jnz	L(last_vec_return_x0)

	VPCMPEQ	%ymm2, %ymm0, %ymm2
	vpmovmskb %ymm2, %eax
	testl	%eax, %eax
	jnz	L(last_vec_return_x1)

	/* Combine last 2 VEC.  */
	VPCMPEQ	%ymm3, %ymm0, %ymm3
	vpmovmskb %ymm3, %eax
	/* rcx has combined result from all 4 VEC. It will only be used
	   if the first 3 other VEC all did not contain a match.  */
	salq	$32, %rcx
	orq	%rcx, %rax
	tzcntq	%rax, %rax
	subq	$(VEC_SIZE * 2 - 1), %rdi
	addq	%rdi, %rax
	VZEROUPPER_RETURN


	.p2align 4
L(last_vec_return_x0):
	tzcntl	%eax, %eax
	subq	$(VEC_SIZE * 4 - 1), %rdi
	addq	%rdi, %rax
	VZEROUPPER_RETURN

	.p2align 4
L(last_vec_return_x1):
	tzcntl	%eax, %eax
	subq	$(VEC_SIZE * 3 - 1), %rdi
	addq	%rdi, %rax
	VZEROUPPER_RETURN

	/* Cold case for crossing page with first load.  */
	.p2align 4
L(cross_page_boundary):
	/* Align data to VEC_SIZE - 1.  */
	orq	$(VEC_SIZE - 1), %rdi
	VPCMPEQ	-(VEC_SIZE - 1)(%rdi), %ymm0, %ymm1
	vpmovmskb %ymm1, %eax
	/* Remove the leading bytes. sarxl only uses bits [5:0] of COUNT
	   so no need to manually mod rdx.  */
	sarxl	%edx, %eax, %eax
L(return_vzeroupper):
	ZERO_UPPER_VEC_REGISTERS_RETURN

END (STRLEN)
#endif
