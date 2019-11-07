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
	.module pvclsound
	

	; global from external entries / code
	.globl _snd_table
	.globl snd_areas

	; global from this module
	.globl _snd_settable,snd_settable
	.globl _snd_startplay
	.globl _snd_stopplay
	.globl _snd_stopall
	.globl _snd_isplaying
	
	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_snd_settable:
    pop de
    pop hl
    push hl
    push de
snd_settable:
	ld	b,#6
    ld a,h
    or a
    jr nz, call_snd_table
    ld hl,#snd_table_dummy
call_snd_table:
    jp 0x1fee

snd_table_dummy:
    .dw _snd_dummy, #0x702b				; SOUNDAREA1
_snd_dummy:
    .db #0xff
    
;---------------------------------------------------------------------------------
_snd_startplay:
    pop     bc
    pop     de
    push    de
    push    bc
    push    ix
    push    iy

    ld      b,e
    call	0x1ff1

    pop     iy
    pop     ix
    ret

;---------------------------------------------------------------------------------
_snd_stopplay:
    pop     bc
    pop     de
    push    de
    push    bc

    ld      a,e   ; a = song#

    ld      b,a   ; b = song#
    ld      hl,#_snd_table-2   			; calcul the right sound slot
    ld      de,#0x0004
sstp1:
    add	hl,de
    djnz	sstp1

    ld      b,a   ; b = song#

    ld      e,(hl)           			; get the sound slot addr.
    inc	hl
    ld      d,(hl)
    ex      de,hl

    ld      a,(hl)           			; get the song# currently in the sound slot
    and     #0x3f

    cp	b                				; compare with the song# we are looking for
    jr	nz,sstp2            			; if not the same song# -> do nothing

    ld      (hl),#0xff
sstp2:
    ret
    
;---------------------------------------------------------------------------------
_snd_stopall:

	ld	b,#5
	ld	de,#0x000a
	ld	hl,#snd_areas
ssta1:
	ld	(hl),#0xff
	add	hl,de
	djnz	ssta1
	
	jp	0x1fd6
	
;---------------------------------------------------------------------------------	
_snd_isplaying:
    pop     bc
    pop     de
    push    de
    push    bc

    ld      a,e   						; a = song#

    ld      b,a   						; b = song#
    ld      hl,#_snd_table-2   			; calcul the right sound slot
    ld      de,#0x0004
ssip1:
    add	hl,de
    djnz	ssip1

    ld      b,a   						; b = song#

    ld      e,(hl)           			; get the sound slot addr.
    inc	hl
    ld      d,(hl)
    ex      de,hl

    ld      a,(hl)           			; get the song# currently in the sound slot
    xor     #0xff						; 0 -> no more sound

    ld			l,a					    ; return value
    ret
	