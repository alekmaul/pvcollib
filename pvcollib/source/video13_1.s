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
	.module pvclvideo13_1
	
	; global from external entries / code
	.globl calcoffset

	; global from this module
	.globl _vdp_putarea
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_putarea:
	exx
	pop	hl
	exx
	pop	hl
	pop	de
	call	calcoffset
	pop	bc
	push	bc
	push	de
	push	hl
	exx
	push	hl
	exx
loop:
	push	bc
	push	de
	push	hl
	ld	b,#0
	xor	a
	call	0x1fdf
	pop	hl
	pop	de
	pop	bc
	push	bc
	ld	b,#0
	add	hl,bc
	ex	de,hl
	ld	bc,#0x0020
	add	hl,bc
	ex	de,hl
	pop	bc
	djnz loop
	ret
	