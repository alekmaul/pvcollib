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
	.module pvclvideo3
	
	; global from external entries / code
	
	; global from this module
	.globl _vdp_rle2vram
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
vdpwrite:
    ld      c,#0xbf 				; (1d43h) = 0bfh
    out     (c),e
    set     6,d
    out     (c),d
	ret
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_rle2vram:
    pop     bc
    pop     hl
    pop     de
    push    de
    push    hl
    push    bc
	call		vdpwrite
    ld      c,#0xbe 					; (1d47h) = 0beh
vr2v0:
    ld      a,(hl)
    inc     hl
    cp      #0xff
    ret     z
    bit     7,a
    jr      z,vr2v2
    and     #0x7f
    inc     a
    ld      b,a
    ld      a,(hl)
    inc     hl
vr2v1:
    out     (c),a
    nop
    nop
    djnz    vr2v1
    jr      vr2v0
vr2v2:      
    inc     a
    ld      b,a
vr2v3:            
    outi
    jr      z,vr2v0
    jp      vr2v3
