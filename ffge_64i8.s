; --------------------------------------------------------------------------- ;
; ffge_64i8.s: Perform FFGE on 8 packed matrices.                             ;
;                                                                             ;
; Copyright 2024 ⧉⧉⧉                                                          ;
;                                                                             ;
; This program is free software: you can redistribute it and/or modify it     ;
; under the terms of the GNU General Public License as published by the       ;
; Free Software Foundation, either version 3 of the License, or (at your      ;
; option) any later version.                                                  ;
;                                                                             ;
; This program is distributed in the hope that it will be useful, but         ;
; WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  ;
; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    ;
; for more details.                                                           ;
;                                                                             ;
; You should have received a copy of the GNU General Public License along     ;
; with this program.  If not, see <https://www.gnu.org/licenses/>.            ;
; --------------------------------------------------------------------------- ;
[bits 64]
default rel

global ffge_64i8

section .rodata


section .text

	extern	ffge_pivmtr_64i

ffge_64i8:
	push	rbx
	push	r12
	push	rdx

	mov	r12, rdi
	mov	rbx, rsi

	xor	rax, rax
	xor	rcx, rcx
	vpxor	ymm10, ymm10
.l_pc_in:
	push	rax
	push	rcx
	push	r8
	push	r9
	mov	rdi, r12
	mov	rsi, rbx
	mov	rdx, rax
	mov	r8, 8
	call 	ffge_pivmtr_64i
	mov	r10, rax
	pop	r8
	pop	r9
	pop	rcx
	pop	rax
	test	r10, r10
	jne	.l_pc_out
	
	mov	r8, rax
	add	r8, 1

	mov	rsi, rax
	imul	rsi, r12
	add	rsi, rcx
	shl	rsi, 6
	vmovdqa	ymm2, [rbx + rsi]
	vmovdqa	ymm6, 32[rbx + rsi]
.l1:
	mov	r9, rcx
	add	r9, 1

	mov	rdi, rax
	imul	rdi, r12
	add	rdi, r9
	shl	rdi, 6
	vmovdqa	ymm3, [rbx + rdi]
	vmovdqa	ymm7, 32[rbx + rdi]
.l2:

	mov	rsi, r8
	imul	rsi, r12
	add	rsi, r9
	shl	rsi, 6
	vpmuldq ymm0, ymm2, [rbx + rsi]
	vpmuldq ymm4, ymm6, 32[rbx + rsi]
	; TODO: modulo prime

	mov	r10, r8
	imul	r10, r12
	add	r10, rcx
	shl	r10, 6
	vpmuldq	ymm1, ymm3, [rbx + r10]
	vpmuldq	ymm5, ymm7, 32[rbx + r10]

	vpsubq	ymm0, ymm0, ymm1
	vpsubq	ymm4, ymm4, ymm5
	vmovdqa	[rbx + rsi], ymm0
	vmovdqa	32[rbx + rsi], ymm4
	inc	r9
	cmp	r9, r12
	jb	.l2

	vmovdqa [rbx + rdi], ymm10
	vmovdqa 32[rbx + rdi], ymm10
	inc	r8
	cmp	r8, r12
	jb	.l1
	
	inc	rax

.l_pc_out:
	inc	rcx
	cmp	rcx, r12
	jb	.l_pc_in

	pop	rdx
	test	rdx, rdx
	jz	.rnk_null
	mov	[rdx], rax
.rnk_null:
	sub	r12, rax
	xor	rax, rax
	test	r12, r12
	setz	al

	vzeroupper
	pop	r12
	pop	rbx
	ret

