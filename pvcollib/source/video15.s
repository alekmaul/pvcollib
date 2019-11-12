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
	.module pvclvideo15
	
	; global from external entries / code

	
	; global from this module
	.globl _vdp_setcharex
	.globl _vdp_setdefaultchar
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
indir:
	jp  (hl)

	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
;---------------------------------------------------------------------------------
_vdp_setdefaultchar:
	pop	bc
	pop	hl
	push	hl
	push	bc

	push	hl							; flags

	ld	hl,(0x73F8)						; offset in VRAM for charset pattern
	ld	bc,#0x00E8						; offset for chr# 29 (00e8h = 29*8)
	add	hl,bc
	push	hl

	ld	h,#128-29						; characters 29 to 128
	ld	l,#29							; start at chr# 29
	push	hl

	call	_vdp_setcharex

	pop bc
	pop bc
	pop bc

	ret
	
_vdp_setcharex:
	pop     de
	pop     hl
	exx
	pop     de
	ld      a,(0x1d43)
	ld      c,a

	out     (c),e
	set     6,d
	out     (c),d

	pop     bc
	ld      a,c
	push    bc
	exx
	push    hl
	push    hl
	push    de
	ld      c,h
	ld      h,#0
	add     hl,hl
	add     hl,hl
	add     hl,hl
	ld      de,(0x006a)
	add     hl,de
	ld      de,#-65*8
	add     hl,de
	exx
	ld      hl,#upload_procs
	and     #3
	add     a,a
	add     a,l
	ld      l,a
	ld      a,#0
	adc     a,h
	ld      h,a
	ld      a,(hl)
	inc     hl
	ld      h,(hl)
	ld      l,a
	exx
	ld      a,c
	exx
	ld      b,a
	exx
	ld      a,(0x1d47)
	ld      c,a
	exx
vscp1: 
    call    indir
    djnz    vscp1
    ret

normal:
	exx
	ld      b,#8
vscp2:      
	outi
	nop
	nop
	jr      nz,vscp2
	exx
	ret

italic:
	exx
	ld      b,#4
vscp3:
	ld      a,(hl)
	inc     hl
	rrca
	and     #0x7f
	out     (c),a
	djnz    vscp3
	ld      b,#4
vscp4:
	outi
	nop
	nop
	jr      nz,vscp4
	exx
	ret

bold:
	exx
	ld      b,#8
vscp5:
	ld      a,(hl)
	inc     hl
	ld      d,a
	rrca
	and     #0x7f
	or      d
	out     (c),a
	djnz    vscp5
	exx
	ret

bold_italic:
	exx
	ld      b,#4
vscp6:
	ld      a,(hl)
	inc     hl
	ld      d,a
	rrca
	and     #0x7f
	or      d
	rrca
	and     #0x7f
	out     (c),a
	djnz    vscp6
	ld      b,#4
vscp7:
	ld      a,(hl)
	inc     hl
	ld      d,a
	rrca
	and     #0x7f
	or      d
	out     (c),a
	djnz    vscp7
	exx
	ret

upload_procs:
	.dw    normal
	.dw    italic
  	.dw    bold
	.dw    bold_italic
