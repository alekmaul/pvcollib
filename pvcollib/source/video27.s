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
	.module pvclvideo27
	

	; global from external entries / code
	
	; global from this module
	.globl _vdp_f18asetscrollx

	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_f18asetscrollx:
	pop	bc
	pop	hl
	push	hl							; value<<8 & layer
	push	bc

	ld		a,l							; get layer
	cp		#2
	jr		z,scxt2						; layer 2 selected
	
	ld		a, h						; VTO0	VTO1	VTO2	VTO3	VTO4	VPO0	VPO1	VPO2
	out		(0xBF),a
	ld		a, #0x9b					; 10011100	write to reg 0x1B
	out		(0xBF),a

	ret
	
scxt2:
	ld		a, l						; VTO0	VTO1	VTO2	VTO3	VTO4	VPO0	VPO1	VPO2
	out		(0xBF),a
	ld		a, #0x99					; 10011100	write to reg 0x19
	out		(0xBF),a

	ret
	