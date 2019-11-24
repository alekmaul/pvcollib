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
	.module pvclscore2
	

	; global from external entries / code

	; global from this module
	.globl _sys_scocmp
	
	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_scocmp:
	pop bc
	pop de
	pop hl
	push hl
	push de
	push bc
	
	push hl
	push de
	
	inc hl
	inc hl
	ld c,(hl)
	inc hl
	ld b,(hl)

	ex de,hl

	inc hl
	inc hl
	ld a,(hl)
	inc hl
	ld h,(hl)
	ld l,a
	
	or a
	sbc hl,bc
	
	jr c,_sclow1
	jr nz,_sclow2
	
	pop de
	pop hl
	push hl
	push de
	
	ld c,(hl)
	inc hl
	ld b,(hl)

	ex de,hl

	ld a,(hl)
	inc hl
	ld h,(hl)
	ld l,a
	
	sbc hl,bc
	
	jr c,_sclow1
	jr nz,_sclow1
	
	ld hl,#0x00FF
	jr _sclow3
	
_sclow2:
	ld hl,#0x0000
	jr _sclow3
_sclow1:
	ld hl,#0x0001
_sclow3:
	pop bc
	pop bc
	ret
				