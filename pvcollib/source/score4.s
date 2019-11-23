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
	.module pvclscore3
	

	; global from external entries / code
	.globl _sys_utoa

	; global from this module
	.globl _sys_scostr
	
	.area _DATA

sco_data:
	.ds    10
	
	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_scostr:
	pop de
	pop hl
	pop bc
	push bc
	push hl
	push de
	
	push bc

	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	
	push hl
	
	ld bc,#sco_data+4
	
	push bc
	push de
	call _sys_utoa
	pop bc
	pop bc
	
	pop hl

	ld e,(hl)
	inc hl
	ld d,(hl)
	
	ld bc,#sco_data
	
	push bc
	push de
	call _sys_utoa
	pop bc
	pop bc
	
	xor a
	ld (sco_data+9),a
	
	pop bc
	ld hl,#9
	sbc hl,bc
	ld de,#sco_data
	add hl,de
	
	ret

