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
	.module pvclvideo1_3

	; global from external entries / code
	.globl vdpwrite

	; global from this module
	.globl _vdp_dan32vram

	.area  _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
; dan3 algorithm by Amy Purple
; December 2017
_vdp_dan32vram:
    pop     bc							; return adress
    pop     hl							; gfx adress
    pop     de							; VRAM adress
    push    de
    push    hl
    push    bc

	call	vdpwrite

	ld		a, #0x80					; Init. Read bits

	;
	;	Explanation:
	;	DAN3 format like DAN2 format adds this extra step for a better fit with big offset values.
	;	However, the goal is a compression ratio comparable to stronger formats that need more RAM.
	;	Because of this, DAN2 is better for simple graphics, DAN3 is better for pixelarts.
	;

	;	Decode size for high offset values
	push	ix
	;	Set max offset size to 10 bits
	ld	ix, #getbite+3
dan3_offsetsize_loop:
	dec	ix
	dec	ix
	dec	ix
	call getbitdan
	jr	c,	dan3_offsetsize_loop

dan3_copy_byte:							; Copy literal byte
	ld	b,#0x01
dan3_literal2main:
	ld		c, #0xbe
dan3_literals_loop:
	outi
	inc		de
	jr	nz, dan3_literals_loop

    ; Main loop
dan3_main_loop:
	call	getbitdan                    ; check next bit
	jr	c,dan3_copy_byte

    ; Decode Exp-Golomb + Special Marker
	push	de
	ld	de, #0x0001
	ld	b,e
dan3_expgolomb_0:
	inc	b
	call	getbitdan			; check next bit
	jr      c, dan3_expgolomb_value
	bit	3,b
	jr	z, dan3_expgolomb_0

    ; Special Marker
	pop	de
	call	getbitdan
	jr	c, dan3_manyliterals
	pop	ix
	ret	; EXIT
dan3_manyliterals:
	ld      b, (hl)                 ; load counter value (8 bits)
	inc	hl
	inc	b
	jr	dan3_literal2main

dan3_expgolomb_value:
	dec	b
dan3_expgolomb_value_loop:
	call	getbite			; check next bit -> DE
	djnz	dan3_expgolomb_value_loop
	dec	e
	ld	b,e
	dec	e
	jr	z, dan3_offset1

	; D = 0, E = ??, B = LENGTH

    ; Decode Offset value
	ld	e,d				; e = 0
	call	getbitdan			; check next bit
	jr	nc, dan3_offset3
	call	getbitdan
	jr	nc, dan3_offset2
	call	get_highbits_e		; read some bits -> E
	inc	e
	ld	d,e				; D = E + 1
dan3_offset3:
	ld      e, (hl)			; load offset offset value (8 bits)
	inc     hl
	ex	af, af'
	ld	a,e
	add	a,#20			; Skip the short offsets covered by 5 bits ones
	ld	e,a
	jr	nc, dan3_offset_nocarry
	inc	d
dan3_offset_nocarry:	
	ex	af, af'
	jr	dan3_copy_from_offset
dan3_offset2:
	;
	;	Explanation:
	;	DAN3 format differs from DAN2 format by using 5 bits instead of 4 bits for small offset values.
	;	This allows for more variety of nearby similar data to copy from, which is great for pixelarts.
	;	However, simple graphics do not need more than a nibble ( 4 bits ) at this step typically.
	;	Conclusion: DAN3 format is good for pixelarts, DAN2 format is good for simple graphics.
	;
	call	get5bitse			; read 5 bits -> E
	jr	dan3_copy_from_offset
dan3_offset1:
	call	getbitdan
	jr	nc, dan3_copy_from_offset
	call	getbite
	inc	e

    ; Copy previously seen bytes
dan3_copy_from_offset:
	ex      (sp), hl                ; store source, restore destination
	push    hl                      ; store destination
	scf
	sbc     hl, de                  ; HL = source = destination - offset - 1
	pop     de                      ; DE = destination
	; BC = count
	; COPY BYTES
	ex	af,af'
	set	6,d
	ld	c,#0xbf
dan3_copybytes_loop:
	out	(c),l
	nop
	out	(c),h
	inc	hl
	nop
	nop
	in	a,(#0xbe)
	nop
	nop
	nop
	out	(c),e
	nop
	out	(c),d
	inc	de
	nop
	nop
	out	(#0xbe),a
	djnz dan3_copybytes_loop
	res	6,d
	ex	af,af'
	pop	hl		; restore source address (compressed data)
	jp	dan3_main_loop

gethighbitse:
	jp	(ix)

get5bitse:
	call	getbite						; Load next bit -> E
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
    