;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2019
;		Alekmaul 
;
;	This software is provided 'as-is', without any express or implied
;	warranty.  In no event will the authors be held liable for any
;	damages arising from the use of this software.
;
;	Permission is granted to anyone to use this software for any
;	purpose, including commercial applications, and to alter it and
;	redistribute it freely, subject to the following restrictions:
;
;	1.	The origin of this software must not be misrepresented; you
;		must not claim that you wrote the original software. If you use
;		this software in a product, an acknowledgment in the product
;		documentation would be appreciated but is not required.
;	2.	Altered source versions must be plainly marked as such, and
;		must not be misrepresented as being the original software.
;	3.	This notice may not be removed or altered from any source
;		distribution.
;
;---------------------------------------------------------------------------------
	.module pvclconsole

	; global from external entries / code

	; global from this module
	.globl _sys_random
	.globl _sys_randbyte

    .area _DATA
string_data:
.ds    6
	
	.area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
count_sub:
    xor     a
_scsp2:
    sbc     hl,bc
    inc     a
    jr      nc,_scsp2
    dec     a
    add     hl,bc
    ret
	
absdiff_max_min:								; HL = B, DE = A
	push	hl
	pop	bc
	sbc	hl,de
	jr	nc,amm1									; if B<A then swap HL=A-B, else HL=B-A
	push	bc
	pop	hl
	ex	de,hl
	sbc	hl,de
amm1:
	ret

rnd1:
	ld	bc,#0xffff								; to build AND_MASK in BC
	ld	a,h
	or  a
	jr	nz,rnd3
	ld	b,a
	ld	a,l
	jp	rnd3
rnd2:
	srl	b
	rr	c
rnd3:
	or	a
	rla
	jr	nc,rnd2									; HL = MAX-MIN, DE = MIN, BC = AND_MASK
	
	push	de
	ex	de,hl									; DE = MAX-MIN, BC = AND_MASK, MIN saved in stack
rnd4:
	push	de									; save MAX-MIN in stack
	push	bc									; save AND_MASK in stack
		
	call	_sys_random   						; Coleco Random Function
		
	pop	bc										; get back AND_MASK from stack
	pop	de										; get back MAX-MIN from stack

	ld	a,h										; apply AND_MASK
	and	b
	ld	h,a
	ld	a,l
	and	c
	ld	l,a

	or	a
	push	de
	ex	de,hl
	sbc	hl,de
	ex	de,hl
	pop	de
	jr	c,rnd4									; if random number > MAX-MIN then retry

	pop	de
	add	hl,de									; HL = random_number between [MIN,MAX]
	ret

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_random:
	call    #0x1ffd
	ld      a,r
	xor     l
	ld      l,a
	ret

_sys_randbyte:
	pop	hl
	pop	de
	push	de
	push	hl
		
	ld	l,d
	xor	a
	ld	h,a
	ld	d,a										; HL = B, DE = A
	call	absdiff_max_min
	jp	rnd1									; continue in rnd function and RET

