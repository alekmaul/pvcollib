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
	.module pvclvideo
	
	; global from external entries / code
	.globl  _nmi_flag
	.globl  _strlencol
	.globl  _buffer32
	.globl _spr_enable
	.globl _spr_clear
	
	; global from this module
	.globl _vdp_setreg
	.globl _vdp_waitvblank
	.globl _vdp_enablenmi
	.globl _vdp_disablenmi
	.globl _vdp_enablevdp
	.globl _vdp_disablevdp
	.globl _vdp_setmode1txt
	.globl _vdp_setmode2txt
	.globl _vdp_setmode2bmp
	.globl _vdp_putstring
	.globl _vdp_setchar	
	.globl _vdp_getchar
	.globl _vdp_setdefaultchar
	.globl _vdp_fillvram
	.globl _vdp_duplicatevram
	.globl _vdp_enablescr
	.globl _vdp_disablescr
	.globl _vdp_putvram
	.globl _vdp_rle2vram
	.globl _vdp_ple2vram
	.globl _vdp_dan2vram
	
	.area  _CODE
		
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
writevdpreg1:
	ld      c,a
	ld      b,#1
	jp      0x1fd9
	
default_setmode2:
	ld      bc,#0x0002             ; vdp_out(0,2) ; set mode 2
	call    0x1FD9

default_vdpreg:
	ld      a,#2
	ld      hl,#0x1800
	call    0x1FB8
	pop     iy
	pop     ix
	pop     hl
	pop     de
	pop     bc
	pop     af
	ret

;---------------------------------------------------------------------------------
calcoffset:
	call    0x08C0       			; calc offset by Coleco bios
	push    hl
	ld      hl,(0x73f6)  			; hl = Name Table Offset
	add     hl,de        			; INPUT : d = y, e = x
	ex      de,hl        			; de = NAME + y * 32 + x
	pop     hl
	ret

;---------------------------------------------------------------------------------
indir:
	jp  (hl)

;---------------------------------------------------------------------------------
vdpwrite:
    ld      c,#0xbf 				; (1d43h) = 0bfh
    out     (c),e
    set     6,d
    out     (c),d
	ret
	
;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_setreg:
    pop     hl
    pop     de
    push    de
    push    hl
    ld b,e
    ld c,d
    jp      0x1fd9
	
_vdp_waitvblank:
	pop     hl
	pop     de
	push    de
	push    hl
	ld      a,(#0x73c4)       ; check if NMI enabled
	and     #0x20
	jr      z,vwvb3
vwvb1:              
	ld      a,e             ; NMI enabled, check _nmi_flag
	or      d
	ret     z
	xor     a
	ld      (_nmi_flag),a
vwvb2:      
	ld      a,(_nmi_flag)
	or      a
	jr      z,vwvb2
	dec     de
	jr      vwvb1
vwvb3:      
	call    #0x1fdc           ; NMI disabled, check VDP status
vwvb4:      
	ld      a,e
	or      d
	ret     z
vwvb5:      
	call    #0x1fdc
	rlca
	jr      nc,vwvb5
	dec     de
	jr      vwvb4
	
;---------------------------------------------------------------------------------
_vdp_enablenmi:
	ld      a,(#0x73c4)
	or      #0x20
	call    writevdpreg1
	jp      0x1fdc
	
;---------------------------------------------------------------------------------
_vdp_disablenmi:
	ld      a,(#0x73c4)
	and     #0xdf
	jp writevdpreg1

;---------------------------------------------------------------------------------
_vdp_enablevdp:
	ld		a,(0x73c4)
	or		#0x40
	jp		writevdpreg1

;---------------------------------------------------------------------------------
_vdp_disablevdp:
	ld		a,(0x73c4)
	and	#0xbf
	jp		writevdpreg1

;---------------------------------------------------------------------------------
; WARNING : IT DOES USE DIRECT IO PORT VALUE
_vdp_setmode1txt:
	push    af
  push    bc
  push    de
  push    hl
  push    ix
  push    iy
  ld      a,(0x73c3)	; _get_reg_0
  and     #0xFD       ; reset M2
  ld      c,a
  ld      b,#0
  call    0x1FD9
  ld      a,(0x73C4)
  and     #0xA7       ; blank screen, reset M1 & M3
  or      #0x82       ; 16K, sprites 16x16
  ld      c,a
  ld      b,#1
  call    0x1FD9
  ld      bc,#0x0380  ; vdp_out(3,80h) ; COLTAB = 2000
  call    0x1FD9
  ld      bc,#0x0400  ; vdp_out(4,00h) ; CHRGEN = 0000
  call    0x1FD9
  ld      hl,#0x1800                 ; clear screen
  ld      de,#0x0300                
  ld      a,#0x20
  call    0x1F82
  jp      default_vdpreg

;---------------------------------------------------------------------------------
; WARNING : IT DOES USE DIRECT IO PORT VALUE
_vdp_setmode2txt:
	push    af
	push    bc
	push    de
	push    hl
	push    ix
	push    iy
	ld      a,(0x73C4)
	and	#0xA7                     	; blank screen, reset M1 & M3
	or 	#0x82                     	; 16K, sprites 16x16
	ld      c,a
	ld      b,#1
	call    0x1FD9
	ld      bc,#0x039F              ; vdp_out(3,9fh)
	call    0x1FD9
	ld      bc,#0x0403              ; vdp_out(4,3)
	call    0x1FD9
	jp      default_setmode2

;---------------------------------------------------------------------------------
;WARNING : IT DOES USE DIRECT IO PORT VALUE
_vdp_setmode2bmp:
	push    af
  push    bc
  push    de
  push    hl
  push    ix
  push    iy
  ld	a,(0x73C4)
  and	#0xA7             		   ; blank screen, reset M1 & M3
  or	#0x82             		   ; 16K, sprites 16x16
  ld      c,a
  ld      b,#1
  call    0x1FD9
  ld      bc,#0x03FF         		; vdp_out(3,ffh)
  call    0x1FD9
  ld      bc,#0x0403         		; vdp_out(4,3)
  call    0x1FD9
  xor     a
  out	(0xBF),a
  ld      a,#0x18
  set     6,a
  out	(0xBF),a
  ld      d,#3
vsm2b1:
	xor     a
vsm2b2:
	out     (0XBE),a
  nop
  inc     a
  jp      nz,vsm2b2
  dec     d
  jp      nz,vsm2b1
  jp      default_setmode2

;---------------------------------------------------------------------------------
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
	jp      nz,vscp2
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
	jp      nz,vscp4
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


_vdp_putchar:
	pop     hl
	pop     de
	push    de
	push    hl
	ld      hl, #4
	add     hl, sp
	call    calcoffset
	ld      bc, #0x0001
	ld      a,#1
	jp      0x1fdf

_vdp_getchar:
	pop     hl
	pop     de
	push    de
	push    hl
	ld      hl, #0x0000
	push    hl
	add     hl, sp
	call    calcoffset
	ld      bc, #0x0001
	ld      a,#1
	call    0x1fe2
	pop     hl
	ret

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

	call	_vdp_setchar

	pop bc
	pop bc
	pop bc

	ret

;---------------------------------------------------------------------------------
_vdp_putstring:
	pop     bc
	pop     de
	call	calcoffset					; de = offset
	pop     hl
	push    hl
	push    de
	push    bc

	push    de
	push    hl
		
	call	_strlencol

	ld	b,#0
	ld	c,l								; bc = count
	pop     hl							; hl = pointer
	pop     de   						; de = offset
	ld      a,c
	jp      0x1FdF

;---------------------------------------------------------------------------------
_vdp_fillvram: 
	exx
	pop     hl
	exx
	pop     hl
	pop     bc
	pop     de
	push    de
	push    bc
	push    hl
	exx
	push    hl
	exx
	ld      d,e
	ld      e,b
	ld      a,c
	jp      0x1f82
	
;---------------------------------------------------------------------------------
_vdp_duplicatevram:
	ld      bc,(0x73C4)
	ld      b,#1
	push    bc
    
	ld      c,#0x80    			  ; vdp_out(1,c0h)
	call    0x1FD9       		  ; BLACK OUT NO INTERUPT 

	ld      hl,(0x73F8)
	ld      b,#128

loop_nbr:
	push    bc
    
	ld      a,l
	out     (0xBF),a
	ld      a,h
	out     (0xBF),a
	push    hl
    
	ld      bc,#0x20BE
	ld      hl,#_buffer32
	inir
    
	pop     hl
	ld      de,#0x4800
	add     hl,de
	ld      a,l
	out     (0xBF),a
	ld      a,h
	out     (0xBF),a
	ld      de,#0xB820
	add     hl,de
	push    hl
    
	ld      bc,#0x20BE
	ld      hl,#_buffer32
	otir
    
	pop     hl
	pop     bc
	djnz    loop_nbr
    
	pop     bc
	call    0x1FD9
    
	ret

;---------------------------------------------------------------------------------
_vdp_enablescr:
	call 	_vdp_enablevdp
	call 	_vdp_enablenmi
	ld		hl,#_spr_enable
	ld		(hl),#00   

	ret
	
_vdp_disablescr:
	ld 		hl,#1
	push	hl
	call 	_vdp_waitvblank
	pop		af
	call 	_spr_clear
	call 	_vdp_disablevdp
	call 	_vdp_disablenmi
	ld		hl,#_spr_enable
	ld		(hl),#00   

	ret

;---------------------------------------------------------------------------------
_vdp_putvram:
    exx
    pop     hl
    exx
    pop     de
    pop     hl
    pop     bc
    push    bc
    push    hl
    push    de
    exx
    push    hl
    exx
    ; - Patch to fix a curious bug -
    ld      a,c
    or	a
    jp	z,0x1fdf
    inc	b
    ; - End Patch -
    jp	0x1fdf
	
;---------------------------------------------------------------------------------
_vdp_rle2vram:
    pop     bc
    pop     hl
    pop     de
    push    de
    push    hl
    push    bc
	call		vdpwrite
    ld      c,#0xbe 					; (1d47h) = 0beh
vr2v0:
    ld      a,(hl)
    inc     hl
    cp      #0xff
    ret     z
    bit     7,a
    jr      z,vr2v2
    and     #0x7f
    inc     a
    ld      b,a
    ld      a,(hl)
    inc     hl
vr2v1:
    out     (c),a
    nop
    nop
    djnz    vr2v1
    jr      vr2v0
vr2v2:      
    inc     a
    ld      b,a
vr2v3:            
    outi
    jr      z,vr2v0
    jp      vr2v3
	
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
	out	(0xbf),a
	nop                     			; VDP timing
	in	a,(0xbe)
	ex	af,af'
	ld	a,e
	nop			; VDP timing
	out	(0xbf),a
	ld	a,d
	or	#0x40
	out	(0xbf),a
	ex	af,af'
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
