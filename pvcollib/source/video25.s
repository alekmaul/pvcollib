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
	.module pvclvideo25
	

	; global from external entries / code
	.globl _vdp_disablenmi
	.globl _vdp_enablenmi
	
	; global from this module
	.globl _vdp_f18asetmode1

	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_f18asetmode1:
	pop	bc
	pop	hl
	push	hl
	push	bc

	push	hl							; flags
	
	call	 _vdp_disablenmi

	ld      bc,#0x0000            		; vdp_out(0,2) ; reset M3 & M4
	call    0x1FD9

	ld      bc,#0x0182
	call    0x1FD9						; vdp_out(1,82) 16K, sprite 16x16 , reset M2 & M1

	ld      bc,#0x0206       	  		; vdp_out(2,6) set name table chrtab to 0x1800
	call    0x1FD9
	ld      bc,#0x0380       	  		; vdp_out(3,80h) set colour table coltab to 0x2000
	call    0x1FD9
	ld      bc,#0x0400       	  		; vdp_out(4,0) set tiles table chrgen to 0x0000
	call    0x1FD9
	ld      bc,#0x0538       	  		; vdp_out(5,56) sprgen 0x1C00 
	call    0x1FD9
	ld      bc,#0x0605       	  		; vdp_out(6,5) sprites ecm 3  sprites data to 0x2800 = 10K  5*2K
	call    0x1FD9
	
	pop		hl
	ld		b,#0x31
	ld		c,l							; vdp_out(49,xxh) regarding parameters flags
	call    0x1FD9
	ld      bc,#0x0A09       	  		; vdp_out(10,0x09) set name table2  tiles ecm3/sprites ecm 3    0x2400 hidden screen
	call    0x1FD9
	ld      bc,#0x0b9c       	  		; vdp_out(11,0x9C); set colour table2 to 0x2700
	call    0x1FD9
  
	call 	_vdp_enablenmi
	
	ret
	