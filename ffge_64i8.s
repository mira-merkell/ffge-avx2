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
	push	r14
	push	r13
	push	r12
	push	rbx
	push	rbp	
	mov	rbp, rsp
	sub	rsp, 0x40

	mov	-0x08[rbp], rdi		; size_t n
	mov	qword -0x10[rbp], 0	; size_t pc: pivot column index

	mov	r12, rdi
	shl	r12, 6			; row length in bytes
	mov	r13, r12
	imul	r13, rdi		; packed matrix size

	mov	rbx, rsi		; int64_t *m

	xor	rcx, rcx		; pivot row offset
	xor	rax, rax		; pivot col offset
	vpxor	ymm15, ymm15
.l0:
	push	rax
	push	rcx
	mov	rdi, -0x08[rbp]
	mov	rsi, rbx
	mov	rcx, -0x10[rbp]
	mov	rdx, -0x10[rbp]
	mov	r8, 8
	call 	ffge_pivmtr_64i
	pop	rcx
	pop	rax

	mov	rsi, rcx
	add	rsi, rax

	mov	r8, rcx
	add	r8, r12
.l1:	
	mov	r9, rax
	add	r9, 0x40
.l2:
	add	r9, 0x40
	jb	.l2


	add	r8, r12
	cmp	r8, r12
	jb	.l1

	add	qword -0x10[rbp], 1
	add	rax, 0x40 
	add	rcx, r12
	cmp	rcx, r13
	jb	.l0

	vzeroupper
	xor	rax, rax

	mov	rsp, rbp
	pop	rbp
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	ret

