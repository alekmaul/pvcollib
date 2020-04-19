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
	.module pvclvideo24
	

	; global from external entries / code
	.globl _vdp_disablenmi
	.globl _vdp_enablenmi
	
	; global from this module
	.globl _vdp_f18asetpaletteentry

	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_f18asetpaletteentry:
    exx
    pop     hl
    exx
    pop     de											; data 
    pop     hl											; entry
    pop     bc                                          ; count
    push    bc
    push    hl
    push    de
    exx
    push    hl
    exx

	ld 		b,c											; to have the right counter
	push 	bc											; because not preserved in nmi fonc
	call	_vdp_disablenmi
	pop 	bc

	ld		a, #0xc0									; Data Port Mode, auto-inc=1, palette 0 1100000
	or		l
	out		(0xBF),a
	ld		a, #0xaf									; 10101111	write to reg 0x2F
	out		(0xBF),a
	nop
	nop
	nop
	nop
	nop
	
vfsp1:
	ld		a,(de)
	out		(0xbe), a									; put palette data
    nop
    nop
	inc		de
	ld		a,(de)
	out		(0xbe), a									; put palette data
    nop
    nop
	inc		de
	djnz    vfsp1
	
	xor     a											; Data Port Mode OFF
	out		(0xBF),a
	ld		a, #0xaf
	out		(0xBF),a									; write to reg 0x2F
	nop
	nop
	nop
	nop
	nop

	call	_vdp_enablenmi

	ret
