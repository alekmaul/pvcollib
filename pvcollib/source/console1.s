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
	.module pvclconsole1

	; global from external entries / code
	.globl _joypad_1
	.globl _joypad_2

	; global from this module
	.globl _sys_pause

	.area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
_sys_pause:
	ld	a,(0x73c4)
	push	af									; keep vdp_reg #1 in stack
	or	#0x20
	ld	c,a
	ld	b,#1
	call	0x1fd9								; enable nmi to update joypad_1 and joypad_2
	push	hl
_csp1:		
	ld	a,(_joypad_1)
	ld	h,a
	ld	a,(_joypad_2)
	or	h
	and	#0xf0
	jr	nz,_csp1
_csp2:		
	ld	a,(_joypad_1)
	ld	h,a
	ld	a,(_joypad_2)
	or	h
	and	#0xf0
	jr	z,_csp2
	pop	hl
	pop	af
	ld	c,a
	ld	b,#1
	jp	0x1fd9									; set back the vdp_reg #1 value
		
