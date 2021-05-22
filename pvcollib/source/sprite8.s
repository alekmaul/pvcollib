;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2020
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
;   Code by Amy Purple, last updated in 2013
;---------------------------------------------------------------------------------
	.module pvclsprite8
	
	; global from external entries / code
	
	; global from this module
	.globl _spr_collide
	
	.area   _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
 
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
; (byte *sprite1,                       ix+0
;  byte *sprite2,                       ix+2
;  unsigned sprite1_size_hor,           ix+4
;  unsigned sprite1_size_vert,          ix+6
;  unsigned sprite2_size_hor,           ix+8
;  unsigned sprite2_size_vert);         ix+10
; sizes decode as follows:
; lobyte - first pixel set
; hibyte - number of pixels set
;---------------------------------------------------------------------------------
_spr_collide:
	push    ix
	ld      ix,#4
	add     ix,sp
	ld      l,0(ix)
	ld      h,1(ix)
	ld      a,(hl)
	add     a,#32
	add     a,6(ix)
	ld      e,a
	ld      d,#0             ; de=vertical pos. sprite 1 + 32
	ld      l,2(ix)
	ld      h,3(ix)
	ld      a,(hl)
	add     a,#32
	add     a,10(ix)
	ld      l,a
	ld      h,#0             ; hl=vertical pos. sprite 2 + 32
	ld      b,11(ix)       ; b=number of pixels, sprite 2
	ex      de,hl
	or      a
	sbc     hl,de
	jr      nc,$1
	ld      b,7(ix)        ; swap sprites
	add     hl,de
	ex      de,hl
	or      a
	sbc     hl,de
$1:              
	ld      a,l
	cp      b
	jr      nc,$9

	ld      l,0(ix)
	ld      h,1(ix)
	inc     hl
	ld      e,(hl)
	inc     hl
	inc     hl
	ld      a,(hl)
	and     #128
	rrca
	rrca
	xor     #32
	add     a,4(ix)
	add     a,e
	ld      e,a
	ld      a,#0
	adc     a,a
	ld      d,a             ; de=horizontal pos. sprite 1 + 32
	ld      l,2(ix)
	ld      h,3(ix)
	inc     hl
	ld      c,(hl)
	inc     hl
	inc     hl
	ld      a,(hl)
	and     #128
	rrca
	rrca
	xor     #32
	add     a,8(ix)
	add     a,c
	ld      l,a
	ld      a,#0
	adc     a,a
	ld      h,a             ; hl=horizontal pos. sprite 2 + 32
	ld      b,9(ix)        ; b=number of pixels, sprite 2
	ex      de,hl
	or      a
	sbc     hl,de
	jr      nc,$2
	ld      b,5(ix)        ; swap sprites
	add     hl,de
	ex      de,hl
	or      a
	sbc     hl,de
$2:
	ld      a,h
	or      a
	jr      nz,$9
	ld      a,l
	cp      b
$9:
	ld      hl,#0
	adc     hl,hl
	pop     ix
	ret
