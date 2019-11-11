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
	.module pvclconsole2

	; global from external entries / code

	; global from this module
	.globl _sys_utoa
	.globl _sys_str

    .area _DATA
string_data:
.ds    6
	
	.area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
count_sub1:
    xor     a
_scsp1:  
    sbc     hl,bc
    inc     a
    jr      nc,_scsp1
    dec     a
    add     hl,bc
    add     a,#48 								; = ascii for number 0
    ld      (de),a
    inc     de
    ret

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_utoa:
    pop     bc
    pop     hl
    pop     de
    push    de
    push    hl
    push    bc
    ld      bc,#10000
    call    count_sub1
    ld      bc,#1000
    call    count_sub1
    ld      bc,#100
    call    count_sub1
    ld      c,#10
    call    count_sub1
    ld      a,l
    add     a,#48 ;; = ascii for number 0
    ld      (de),a
    ret
	
;---------------------------------------------------------------------------------
_sys_str:
	pop	hl
	pop	bc
	push	bc
	push	hl

	ld	hl,#string_data
	push	hl
	push	bc
	
	call	_sys_utoa

	pop	bc
	pop	bc

	xor	a
	ld	(string_data+5),a
	ld	hl,#string_data

	ret
