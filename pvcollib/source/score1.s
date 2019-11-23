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
	.module pvclscore1
	

	; global from external entries / code

	; global from this module
	.globl _sys_scoadd
	
	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_scoadd:
	pop de
	pop hl
	pop bc
	push bc
	push hl
	push de
	
	push hl
	
	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	ex de,hl
	add hl,bc
	ex de,hl
	ld c,(hl)
	inc hl
	ld b,(hl)
	ex de,hl
	or a
	ld de,#0x2710
_scoa1:
	sbc hl,de
	inc bc
	jr nc,_scoa1
	dec bc
	add hl,de
	ex de,hl
	pop hl
	ld (hl),e
	inc hl
	ld (hl),d
	inc hl
	ld (hl),c
	inc hl
	ld (hl),b
	ret


