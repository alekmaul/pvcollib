;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018
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
	
	; global from this module
	.globl  _vdp_waitvblank
	.globl  _vdp_enablenmi
	.globl  _vdp_disablenmi
	.globl  _vdp_enablevdp
	.globl  _vdp_disablevdp
	.globl  _vdp_setmode1txt
	.globl  _vdp_setmode2txt
	.globl  _vdp_setmode2bmp
	.globl  _vdp_putstring
  .globl  _vdp_setchar	
	.globl  _vdp_setdefaultchar
	.globl  _vdp_fillvram
	.globl  _vdp_duplicatevram
	
	.area   _CODE
		
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
	call    0x08C0       ; calc offset by Coleco bios
  push    hl
  ld      hl,(0x73f6)  ; hl = Name Table Offset
  add     hl,de        ; INPUT : d = y, e = x
  ex      de,hl        ; de = NAME + y * 32 + x
  pop     hl
  ret

;---------------------------------------------------------------------------------
indir:
	jp  (hl)

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
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
  and	#0xA7                ; blank screen, reset M1 & M3
  or	#0x82                ; 16K, sprites 16x16
  ld      c,a
  ld      b,#1
  call    0x1FD9
  ld      bc,#0x03FF         ; vdp_out(3,ffh)
  call    0x1FD9
  ld      bc,#0x0403         ; vdp_out(4,3)
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
_vdp_setchar:
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
$1: 
    call    indir
    djnz    $1
    ret

normal:
	exx
  ld      b,#8
$2:      
  outi
  nop
  nop
  jp      nz,$2
  exx
  ret

italic:
  exx
  ld      b,#4
$3:
  ld      a,(hl)
  inc     hl
  rrca
  and     #0x7f
  out     (c),a
  djnz    $3
  ld      b,#4
$4:
  outi
  nop
  nop
  jp      nz,$4
  exx
  ret

bold:
  exx
  ld      b,#8
$5:
  ld      a,(hl)
  inc     hl
  ld      d,a
  rrca
  and     #0x7f
  or      d
  out     (c),a
  djnz    $5
  exx
  ret

bold_italic:
  exx
  ld      b,#4
$6:
  ld      a,(hl)
  inc     hl
  ld      d,a
  rrca
  and     #0x7f
  or      d
  rrca
  and     #0x7f
  out     (c),a
  djnz    $6
  ld      b,#4
$7:
  ld      a,(hl)
  inc     hl
  ld      d,a
  rrca
  and     #0x7f
  or      d
  out     (c),a
  djnz    $7
  exx
  ret

upload_procs:
	.dw    normal
  .dw    italic
  .dw    bold
  .dw    bold_italic

;---------------------------------------------------------------------------------
_vdp_setdefaultchar:
		pop	bc
		pop	hl
		push	hl
		push	bc

		push	hl		; flags

		ld	hl,(0x73F8) 	; offset in VRAM for charset pattern
		ld	bc,#0x00E8	; offset for chr# 29 (00e8h = 29*8)
		add	hl,bc
		push	hl

		ld	h,#128-29	; characters 29 to 128
		ld	l,#29		; start at chr# 29
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
  call		calcoffset	; de = offset
  pop     hl
  push    hl
	push    de
  push    bc

  push    de
  push    hl
		
  call	_strlencol

  ld	b,#0
  ld	c,l	; bc = count
  pop     hl	; hl = pointer
  pop     de   	; de = offset
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
    
  ld      c,#0x80      ; vdp_out(1,c0h)
  call    0x1FD9         ; BLACK OUT NO INTERUPT 

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
