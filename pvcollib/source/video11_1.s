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
	.module pvclvideo11_1
	
	; global from external entries / code
	
	; global from this module
	.globl _vdp_putvram_repeat
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_putvram_repeat:
	exx
	pop     hl
	pop     de

	di
                                        ; call	vdpwrite
	ld	a,e								; VRAM address setup
	out	(0xbf),a
	ld	a,d
	or	#0x40
	out	(0xbf),a                        ; end call
	
	
	exx
	pop     hl
	pop     de
	push	de
	push    hl
	exx
	push    de
	push    hl
	exx
    ld      c,#0xbe                     ; (1d47h) = 0beh
$1:
	ld      b,e
	push    hl
$2:
	outi
	nop
	nop
	jp      nz,$2
	pop     hl
	dec     d
	jr      nz,$1
	ei
	ret
    