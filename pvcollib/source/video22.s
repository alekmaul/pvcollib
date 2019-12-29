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
	.module pvclvideo22
	
	; global from external entries / code
	.globl default_setmode2
	
	; global from this module
	.globl _vdp_setmode2txt
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
; WARNING : IT DOES USE DIRECT IO PORT VALUE
_vdp_setmode2txt:
	push    af
	push    bc
	push    de
	push    hl
	push    ix
	push    iy
	
	ld      a,(0x73C4)					; 73C4 -> Copy of VDP1
	and		#0xA7                     	; blank screen, reset M1 & M2 10100111 -> 10000010
	or 		#0x82                     	; 16K, sprites 16x16
	ld      c,a
	ld      b,#1
	call    0x1FD9
	
	ld      bc,#0x039F         		    ; vdp_out(3,9fh)
	call    0x1FD9
	
	ld      bc,#0x0403        		    ; vdp_out(4,3)
	call    0x1FD9
	
	ld      bc,#0x0607       	  		; vdp_out(6,7) sprites data to 0x3800
	call    0x1FD9

	jp      default_setmode2
