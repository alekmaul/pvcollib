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
	.module pvclvideo
	
	; global from external entries / code
	.globl default_vdpreg
	
	; global from this module
	.globl _vdp_setmode1txt
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
; WARNING : IT DOES USE DIRECT IO PORT VALUE
_vdp_setmode1txt:
	push    af
	push    bc
	push    de
	push    hl
	push    ix
	push    iy
	ld      a,(0x73c3)					; _get_reg_0
	and     #0xFD       				; reset M3
	ld      c,a
	ld      b,#0
	call    0x1FD9
	
	ld      a,(0x73C4)
	and     #0xA7       				; blank screen, reset M1 & M2
	or      #0x82      					; 16K, sprites 16x16
	ld      c,a
	ld      b,#1
	call    0x1FD9
  
	ld      bc,#0x0380  				; vdp_out(3,80h) ; COLTAB = 2000
	call    0x1FD9
  
	ld      bc,#0x0400  				; vdp_out(4,00h) ; CHRGEN = 0000
	call    0x1FD9
  
  	ld      bc,#0x053E       	  		; vdp_out(5,3Eh) sprites pattern to 0x1f00
	call    0x1FD9

	ld      hl,#0x1800                 ; clear screen
	ld      de,#0x0300                
	ld      a,#0x20
	call    0x1F82
	jp      default_vdpreg
