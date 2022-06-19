;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2022
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
	.module pvclvideo23
	
	; global from external entries / code
	
	; global from this module
	.globl _vdp_setscreenadr
	.globl _vdp_swapscreen
	
    .area _DATA
screen_name_table:
	.ds    2
    
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	.area _CODE
	
__vdp_setscreenadr:
    exx
    pop	hl
    exx
    pop	hl
    pop	de
    push	de
    push	hl
    exx
    push	hl
    exx

vssc1:
    ld	(screen_name_table),hl

    push	de

    push	ix
    push	iy
    ld	a,#2
    call	0x1FB8
    pop	iy
    pop	ix

    pop	de

    ld	(0x73F6),de

    ret

_vdp_swapscreen:
    ld	de,(screen_name_table)
    ld	hl,(0x73F6)
    jp	vssc1
