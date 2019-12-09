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
	.module pvclvideo23
	

	; global from external entries / code
	.globl _vdp_disablenmi
	.globl _vdp_enablenmi
	.globl _vdp_putvram
	.globl _vdp_fillvram
	
	; global from this module
	.globl _vdp_f18aok
	.globl _vdp_f18ainit

    .area _DATA
_vdp_f18aok::
	.ds    1
		
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
_GPUTEST::
	.db 0x02, 0x00, 0x12, 0x34     				; LI R0,>1234    get the test value into register 0
	.db 0xC8, 0x00, 0x1C, 0x10	     			; MOV R0,@>1C10 <-- this is the target test adr in VDP RAM
	.db 0x03, 0x40           					; IDLE       This puts the GPU back to sleep
	.db 0x00, 0x00           					; DATA 0x0000					

_vdp_read2:
	ld		a, l								; send LSB of address (no delay needed)
	out		(0xbf), a

	ld		a, h								; send MSB of address (delay needed before read)
	out		(0xbf), a
	nop											; (maybe overkill here, but this function doesn't need to be fast)
	nop
	nop
	nop

	in		a, (0xbe)							; get the MSB (delay needed before next read)
	ld		b, a
	ld		c, #0x00							; shift up to MSB (could be optimized to a move, 1uS)
	nop											; (maybe overkill here, but this function doesn't need to be fast)
	nop
	nop
	nop

	in		a, (0xbe)							; ret |= 0x00be; get the LSB
	ld		e, a
	ld		d, #0x00
	ld		a, c
	or		a, e
	ld		l, a
	ld		a, b
	or		a, d
	ld		h, a
	ret
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_f18ainit:
	push	af
	
	call	_vdp_disablenmi
	ld		bc, #0x3280							; reset and lock F18A (or corrupt VDPR2 if 9928A)
	call    0x1FD9								; vdp_out(50,0x80)

	ld		bc, #0x391c							; write unlock value to register 57 (or corrupt VDPR1)
	call    0x1FD9								; vdp_out(57,0x1c)

	ld		bc, #0x391c							; repeat to unlock. This is the sequence that mustn't be interrupted
	call    0x1FD9								; vdp_out(57,0x1c)

	ld		hl, #0x000C							; we copy a little GPU program (9900 Asm) to the VDP and try to execute it. 
	push 	hl									; if it's a TMS9928A, nothing will happen. Otherwise it will change a byte of
	ld		hl, #_GPUTEST						; VDP RAM for us
	push	hl
	ld		hl, #0x1c00
	push	hl									; VDPRAM_TEST_ADR 0x1c00  // if you change this, also change the test address in the GPU code (0x1c10)
	call	_vdp_putvram						; put_vram(VDPRAM_TEST_ADR, GPUTEST, 10);
	pop		af
	pop		af
	pop		af

	ld		bc, #0x361c							; MSB in register 54
	call    0x1FD9								; vdp_out(54, VDPRAM_TEST_ADR>>8)

	ld		bc, #0x3700							; LSB in register 55 - GPU starts!
	call    0x1FD9								; vdp_out(55, VDPRAM_TEST_ADR&0xff)

	nop
	nop
	nop
	nop
	
	ld		hl, #0x1c10
	push	hl
	call	_vdp_read2
	pop		af

	ld		a, l								; check if we have 0x1234
	sub		a, #0x34
	jr		NZ,_vs182
	ld		a, h
	sub		a, #0x12
	jr		NZ,_vs182

	ld		hl,#_vdp_f18aok
	ld		(hl), #0x01
	jr		_vs184
_vs182:	
	ld		hl,#_vdp_f18aok
	ld		(hl), #0x00
_vs184:
	call	_vdp_enablenmi

	pop		af
	ret
