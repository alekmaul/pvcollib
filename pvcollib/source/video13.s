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
	.module pvclvideo13
	
	; global from external entries / code
	.globl  _buffer32
	
	; global from this module
	.globl _vdp_duplicatevram
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_duplicatevram:
	ld      bc,(0x73C4)
	ld      b,#1
	push    bc
    
	ld      c,#0x80    			  ; vdp_out(1,c0h)
	call    0x1FD9       		  ; BLACK OUT NO INTERUPT 

	ld      hl,(0x73F8)
	ld      b,#128

loop_nbr:
	push    bc
    
	ld      a,l
	out     (0xBF),a
	ld      a,h
	out     (0xBF),a
	push    hl
    
	ld      bc,#0x20BE
	ld      hl,#_buffer32
	inir
    
	pop     hl
	ld      de,#0x4800
	add     hl,de
	ld      a,l
	out     (0xBF),a
	ld      a,h
	out     (0xBF),a
	ld      de,#0xB820
	add     hl,de
	push    hl
    
	ld      bc,#0x20BE
	ld      hl,#_buffer32
	otir
    
	pop     hl
	pop     bc
	djnz    loop_nbr
    
	pop     bc
	call    0x1FD9
    
	ret
	