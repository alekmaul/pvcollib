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
	.module pvclvideo2
	
	; global from external entries / code
	
	; global from this module
	.globl _vdp_ple2vram
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_ple2vram:
    pop     bc
    pop     hl
    pop     de
    push    de
    push    hl
    push    bc
    
    push    ix
    
	di

	ld	a,e								; VRAM address setup
	out	(0xbf),a
	ld	a,d
	or	#0x40
	out	(0xbf),a

	ld	a,(hl)							; Initialization
	inc	hl
	exx
	ld	de,#0
    push    de
	add	a,a
	inc	a
	rl	e
	add	a,a
	rl	e
	add	a,a
	rl	e
	rl	e
	ld	hl,#modes
	add	hl,de
	ld	e,(hl)
    inc hl
    ld  d,(hl)
    push    de
    pop ix
    pop de
	ld	e,#1
	exx
	;ld	iy,#loop

literal:								; Main depack loop
    ld	c,#0xbe
	outi
	inc	de
loop:
    call    getbits
	jr	nc,literal

	exx									; Compressed data
	ld	h,d
	ld	l,e
getlen:
    call getbitsexx
	jr	nc,lenok
lus:
    call getbitsexx
	adc	hl,hl
	ret	c
    call getbitsexx
	jr	nc,lenok
    call getbitsexx
	adc	hl,hl
	jp	c,Depack_out
    call getbitsexx
	jp	c,lus
lenok:
    inc	hl
	exx
	ld	c,(hl)
	inc	hl
	ld	b,#0
	bit	7,c
	jp	z,offsok
	jp	(ix)

mode7:
    call    getbits
	rl	b
mode6:
    call    getbits
	rl	b
mode5:
    call    getbits
	rl	b
mode4:
    call    getbits
	rl	b
mode3:
    call    getbits
	rl	b
mode2:
    call    getbits
	rl	b
    call    getbits
	jr	nc,offsok
	or	a
	inc	b
	res	7,c
offsok:
    inc	bc
	push	hl
	exx
	push	hl
	exx
	ld	l,e
	ld	h,d
	sbc	hl,bc
	pop	bc
	push	af
vr2p9:
    ld	a,l
	out	(0xbf),a
	ld	a,h
	nop									; VDP timing
	nop									; VDP timing
	nop									; VDP timing
	out	(0xbf),a
	nop                     			; VDP timing
	nop									; VDP timing
	nop									; VDP timing
	in	a,(0xbe)
	ex	af,af'
	ld	a,e
	nop									; VDP timing
	nop									; VDP timing
	nop									; VDP timing
	out	(0xbf),a
	ld	a,d
	or	#0x40
	out	(0xbf),a
	ex	af,af'
	nop									; VDP timing
	nop									; VDP timing
	nop									; VDP timing
	out	(0xbe),a
	inc	de
	cpi
	jp	pe,vr2p9
	pop	af
	pop	hl
	;jp	(iy)
    jp loop

getbitsexx:
    exx
    call    getbits
    exx
    ret

getbits:
	add	a,a
	ret	nz
	ld	a,(hl)
	inc	hl
	rla
	ret

Depack_out:								; Depacker exit
    pop ix
	ei
	ret

modes:
	.dw	offsok
	.dw	mode2
	.dw	mode3
	.dw	mode4
	.dw	mode5
	.dw	mode6
	.dw	mode7
