; --------------------------------------------------------------------------- ;
; ffge_32i4.s: Perform FFGE on 4 packed matrices.                             ;
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

global ffge_32i4

section .rodata

const_one	dw 1

section .text

	extern	ffge_pivmtr_32i

ffge_32i4:
	push	rbx
	push	r12
	push	r13

	mov	r13, rdx		; fr flags
	mov	rbx, rsi		; matrix m
	xor	rcx, rcx		; pivot column
	mov	r12, rdi		; size n
	shl	r12, 4
	vpbroadcastd ymm9, [const_one]	; dv
	vpxor	ymm10, ymm10

.pivot:
	push	rcx
	push	r9
	push	r10
	push	r11

	mov	rdi, r12
	shr	rdi, 4
	mov	rsi, rbx
	mov	rdx, rcx
	mov	rcx, 4
	call	ffge_pivmtr_32i

	pop	r11
	pop	r10
	pop	r9
	pop	rcx

	mov	r10, rcx
	add	r10, 16
	mov	r11, r10		; r10 = r11 = j+1
	mov	rsi, rcx
	imul	rsi, r12
	add	rsi, rcx		; rsi = j*n + j
.li:	mov	rdi, r10
	imul	rdi, r12
	add	rdi, rcx		; rdi = i*n + j
.lk:	mov	r9, r10
	imul	r9, r12
	add	r9, r11			; r9 = i*n + k
	mov	rdx, rcx
	imul	rdx, r12
	add	rdx, r11		; rdx = j*n + k

	vmovdqa	xmm0, [rbx + rsi]
	vmovdqa	xmm1, [rbx + rdi]
	vpmulld xmm0, xmm0, [rbx + r9]
	vpmulld xmm1, xmm1, [rbx + rdx]
	vpsubd	xmm0, xmm0, xmm1

	vmovdqa	[rbx + r9], xmm0
	add	r11, 16
	cmp	r11, r12
	jl	.lk

	vmovdqa	[rbx + rdi], xmm10
	add	r10, 16
	cmp	r10, r12
	jl	.li

	vmovdqa [rbx + rsi], xmm9
	add	rcx, 16
	cmp	rcx, r12
	jl	.pivot

	vzeroupper
	pop	r13
	pop	r12
	pop	rbx
	ret
