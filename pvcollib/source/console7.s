;---------------------------------------------------------------------------------
;
;	Copyright (C) 2021
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
;   Based on Oscar Toledo G. code from sgm_test.rom
;---------------------------------------------------------------------------------
	.module pvclconsole7
	

	; global from external entries / code
	
	; global from this module
	.globl _sys_sgmok
	.globl _sys_sgminit

    .area _DATA
_sys_sgmok::
	.ds    1
		
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
_AY8910TESTWR::
    .db 0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x38,0x00,0x00,0x00,0x00,0x00,0x00
_AY8910TESTRD::
    .db 0xFF,0x0F,0xFF,0x0F,0xFF,0x0F,0x1F,0x3F,0x1F,0x1F,0x1F,0xFF,0x0F,0x0F
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_sys_sgminit:
	push	af
	
    ld		hl,#_sys_sgmok              ; currently no SGM available
	ld		(hl), #0x00

    ld      hl, #0x2000                 ; we are going 1st to check if we have an Adam computer 
    ld      bc, #0x4022                 ; So we test 64 bytes in ram memory at $2000
_ssgad1:
    ld      (hl), c
    inc     h
    inc     c
    djnz    _ssgad1

    ld      hl, #0x2000                 ; after writing, checking memory
    ld      bc, #0x4022
_ssgad2:
    ld      a, (hl)
    cp      c                           ; is RAM ok ?
    jr      nz, _ssgnr2                 ; nope, try to activate expension RAM
    inc     h
    inc     c
    djnz    _ssgad2
    jr _ssgad5                          ; it is an Adam computer, don't activate SGM RAM

_ssgnr2:
    ex      af, af'                     
    ld      a, #0x01
    out     (0x53), a                   ; activate ram write #1 in port $53
    nop                                 ; do a pause
    nop
    
    ld      hl, #0x2000                 ; write again memory from $2000-$7fff for 24k ram
_ssgad3:
    ld      a, l
    add     a, h
    cpl
    ld      (hl), a
    inc     hl
    ld      a, h
    cp      #0x80                       ; are we at the end of additional ram ?
    jr      nz, _ssgad3

    ld      hl, #0x2000                 ; and again, we test
_ssgad4:
    ld      a, l
    add     a, h
    cpl
    cp      (hl)                        ; is RAM ok ?
    jr      nz, _ssqad99                ; nope, no SGM available, go out
    inc     hl
    ld      a, h
    cp      #0x80                       ; are we at the end of additional ram ?
    jr      nz, _ssgad4
    
    ld		hl,#_sys_sgmok              ; It is ColecoVision system with  SGM available
	ld		(hl), #0x01
    jr      _ssqad99

_ssgad5:
    ld      hl, #_AY8910TESTWR          ; now we are going to check AY to know if it is a SGM for Adam computer
    ld      bc, #0x0E00
    push    hl
_ssgad6:
    ld      a, c
    out     (0x50), a                   ; write 0 to control port
    push    ix                          ; do a small pause
    pop     ix
    ld      a, (hl)
    out     (0x51), a                   ; write value to data port
    push    ix                          ; do a small pause
    pop     ix
    inc     hl
    inc     c
    djnz    _ssgad6

    pop     hl
    ld      de, #_AY8910TESTRD          ; now we test if it is ok
    ld      bc, #0x0E00
_ssgad7:
    ld      a, c
    out     (0x50), a                   ; write 0 to control port
    push    ix                          ; do a small pause
    pop     ix
    in      a, (0x52)                   ; read value from data port
    push    ix                          ; do a small pause
    pop     ix
    ex      de, hl
    and     (hl)                        ; normaly it does nothing as it is same value
    ex      de, hl
    cp      (hl)                        ; is it ok, really same value ?
    jr      nz, _ssqad99                ; nope, no SGM connected
    inc     de
    inc     hl
    inc     c
    djnz    _ssgad7
    
    ld		hl,#_sys_sgmok              ; It is Adam system with  SGM available
	ld		(hl), #0x01

_ssqad99:
    pop		af
	ret
