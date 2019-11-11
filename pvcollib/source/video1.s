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
	.module pvclvideo1
	
	; global from external entries / code
	
	; global from this module
	.globl _vdp_dan2vram
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
; dan1 algorithm by Amy Pruple
_vdp_dan2vram:
    pop     bc							; return adress
    pop     hl							; gfx adress
    pop     de							; VRAM adress
    push    de
    push    hl
    push    bc
		
	di
		
	ld		c, #0xBF					; Set Write in VRAM at DE
	out		(c), e
	set		6, d
	out		(c), d
	res		6, d

	ld		a, #0x80					; Init. Read bits

dan1_copy_byte:							; Copy literal byte
	ld		c, #0xbe
	outi
	inc		de

dan1_main_loop:							; Main loop
	call	getbitdan        	         ; check next bit
	jr		c,dan1_copy_byte
	
    push	de							; Elias gamma decoding + Special marker
    ld		de, #0x0001
    ld		b,d	

dan1_eliasgamma_0:
    inc		b
	call	getbitdan					; check next bit
	jr		c, dan1_eliasgamma_value
	bit		4,b
    jp		nz, dan1_special			; special marker "0000000000000000"
	jr		dan1_eliasgamma_0

dan1_eliasgamma_value_loop:
    call	getbite						; check next bit -> DE
    rl		d
	
dan1_eliasgamma_value:
	djnz		dan1_eliasgamma_value_loop
	push	de
	pop		bc							; BC = LENGTH
	
; Get Offset value
	ld		d, #0x00					; Reset Offset to #0x0000
	
; on len, goto TWO_OFFSETS, THREE_OFFSETS or FOUR_OFFSETS

	ex		af,af'
	ld		a,b
	or		a
	jr		z, dan1_bzero
	ld		a, #0x03

dan1_bzero:
	or		c
	ld		e, a
	ex		af, af'
	dec		e
	jr		z, dan1_offset2
	dec		e
	jr		z, dan1_offset3
	ld		e, #0x00

dan1_offset4:
	call		getbitdan				; check next bit
	jr		nc, dan1_offset3
    call		getnibblee				; get next nibble -> E
	inc		e
	ld		d,e							; D = E+1
	jr		dan1_offset3a
dan1_offset3:
	call		getbitdan				; check next bit
	jr		nc, dan1_offset2
dan1_offset3a:
    ld     	e, (hl)						; load offset offset value (8 bits)
    inc   	hl
	ex		af, af'
	ld		a,e
	add		a, #0x12
	ld		e,a
	jr		nc, dan1_offset3b
	inc		d
dan1_offset3b:
	ex		af,af'
	jr		dan1_copy_from_offset
dan1_offset2:
	call		getbitdan				; check next bit
	jr      	nc, dan1_offset1
    call		getnibblee				; get next nibble -> E
	inc		e
	inc		e
	jr		dan1_copy_from_offset
dan1_offset1:
	call		getbitdan				; check next bit
	rl		e
	
; Copy previously seen bytes
dan1_copy_from_offset:
    ex		(sp), hl                	; store source, restore destination
    push	hl                      	; store destination
	scf
    sbc		hl, de                  	; HL = source = destination - offset - 1
    pop		de                      	; DE = destination
										; BC = count
										; COPY BYTES
	ex		af,af'
	set		6,d
dan1_copybytes_loop:
	push	bc
	ld		c, #0xBF
	out		(c), l
	nop
	out		(c), h
	inc		hl
	nop
	nop
	in		a, (0xBE)
	nop
	nop
	nop
	out		(c), e
	nop
	out		(c), d
	inc		de
	nop
	nop
	out		(0xBE), a
	pop		bc
	dec		bc
	ld		a,b
	or		c
	jr		nz, dan1_copybytes_loop
	res		6,d
	ex		af,af'
    pop		hl							; restore source address (compressed data)
    jp		dan1_main_loop
	
dan1_special:
	pop		de
	
	ei
	ret									; exit

getnibblee:
    call	getbite						; Load next bit -> E
    call	getbite						; Load next bit -> E
    call	getbite						; Load next bit -> E
getbite:
    call	getbitdan					; Load next bit -> E
    rl      e	
	ret

getbitdan:
	add		a, a
  	ret		nz
	ld		a, (hl)
	inc		hl
	rla
	ret
