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
	.module pvclconsole

	; global from external entries / code

	; global from this module
	.globl  _random
	.globl  _strlencol
	.globl _utoa

	.area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
count_sub:
    xor     a
$2:
    sbc     hl,bc
    inc     a
    jr      nc,$2
    dec     a
    add     hl,bc
    ret
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_random:
	call    #0x1ffd
	ld      a,r
	xor     l
	ld      l,a
	ret

;---------------------------------------------------------------------------------
_strlencol:
	pop	hl
	pop	de
	push	de
	push	hl
	ld	hl,#0x0000
$1:		ld	a,(de)
	or	a
	ret	z
	inc	hl
	inc	de
	jr	$1

;---------------------------------------------------------------------------------
_utoa:
    pop     bc
    pop     hl
    pop     de
    ;exx
    ;pop     bc
    ;push    bc
    ;exx
    push    de
    push    hl
    push    bc
    ld      bc,#10000
    call    count_sub
    ;exx
	;add     a,c
    ;exx
    add     a,#0x30
    ld      (de),a
    inc     de
    ld      bc,#1000
    call    count_sub
    ;exx
	;add     a,c
    ;exx
    add     a,#0x30
    ld      (de),a
    inc     de
    ld      bc,#100
    call    count_sub
    ;exx
	;add     a,c
    ;exx
    add     a,#0x30
    ld      (de),a
    inc     de
    ld      c,#10
    call    count_sub
    ;exx
	;add     a,c
    ;exx
    add     a,#0x30
    ld      (de),a
    inc     de
    ld      a,l
    ;exx
	;add     a,c
    ;exx
    add     a,#0x30
    ld      (de),a
    ret