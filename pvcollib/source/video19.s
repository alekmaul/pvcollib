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
	.module pvclvideo19
	
	; global from external entries / code
	.globl _sys_strlen
	.globl calcoffset
	
	; global from this module
	.globl _vdp_putstring
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_putstring:
	pop     bc
	pop     de
	call	calcoffset					; de = offset
	pop     hl
	push    hl
	push    de
	push    bc

	push    de
	push    hl
		
	call	_sys_strlen

	ld	b,#0
	ld	c,l								; bc = count
	pop     hl							; hl = pointer
	pop     de   						; de = offset
	ld      a,c
	jp      0x1FdF
