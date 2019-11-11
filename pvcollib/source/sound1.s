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
	.module pvclsound1
	

	; global from external entries / code
	.globl _snd_table

	; global from this module
	.globl _snd_stopplay
	
	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
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
    
