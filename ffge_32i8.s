[bits 64]
default rel

global ffge_32i8

section .rodata

const_one	dw 1

section .text

ffge_32i8:
	push	rbx
	push	r12

	mov	rax, rdx		; fr flags
	or	rax, 0xff
	mov	rbx, rsi		; matrix m
	xor	rcx, rcx		; pivot column
	mov	r12, rdi		; size n
	shl	r12, 5
	vpbroadcastd ymm9, [const_one]	; dv
	vpxor	ymm10, ymm10

	xor	rsi, rsi
	xor	rdi, rdi
.pivot_col:

	mov	r10, rcx
	add	r10, 32
	mov	r11, r10		; r10 = r11 = j+1

	mov	rsi, rcx
	imul	rsi, r12
	add	rsi, rcx		; rsi = j*n + j
	
.li:
	mov	rdi, r10
	imul	rdi, r12
	add	rdi, rcx		; rdi = i*n + j
.lk:
	mov	r9, r10
	imul	r9, r12
	add	r9, r11			; r9 = i*n + k
	mov	rdx, rcx
	imul	rdx, r12
	add	rdx, r11		; rdx = j*n + k

	vmovdqa	ymm0, [rbx + rsi]
	vmovdqa	ymm1, [rbx + rdi]
	vpmulld ymm0, ymm0, [rbx + r9]
	vpmulld ymm1, ymm1, [rbx + rdx]
	vpsubd	ymm0, ymm0, ymm1
	
	vmovdqa	[rbx + r9], ymm0
		
	add	r11, 32
	cmp	r11, r12
	jl	.lk

	vmovdqa	[rbx + rdi], ymm10

	add	r10, 32
	cmp	r10, r12
	jl	.li

	vmovdqa [rbx + rsi], ymm9
	
	add	rcx, 32
	cmp	rcx, r12
	jl	.pivot_col	


	vzeroupper
	pop	r12
	pop	rbx
	ret
