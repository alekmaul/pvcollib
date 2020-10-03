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
;---------------------------------------------------------------------------------
	.module pvclconsole5_1

	; global from external entries / code
    .globl _keypad_2

	; global from this module
	.globl _sys_choice_keypad2

	.area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_choice_keypad2:
	pop	af
	pop de
	push de
	push af
choice:
	ld	hl,#_keypad_2
	ld	c,(hl)
	ld	a,d
	sub	a,c
	jr	c,	choice
	ld	a,c
	sub a,e
	jr	c,	choice
endchoice:
	ld	hl,#_keypad_2
	ld	a,(hl)
	sub	a,#15	                ; PAD_KEYNONE
	jr	c, endchoice
	ld	l,c
	ret
    