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
; this is in antoher file because of snd_table not declared in lib and this function
;  is used in crtcol.s, so we need to disociate this one from other sound functions.

	.module pvclsound0
	

	; global from external entries / code

	; global from this module
	.globl _snd_settable,snd_settable
	
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
snd_settable:										; for crtcol entry
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
    