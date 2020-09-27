; vdpex.s

	.module put_vram_ex
	
    .globl  _vdp_putvramex
    ; vdp_putvramex (offset,ptr,count,byte and,byte xor)
    
    .area _CODE
                
_vdp_putvramex:
    pop     hl
    exx
    pop     de
	
	; call	vdpwrite
	ld	a,e								; VRAM address setup
	out	(0xbf),a
	ld	a,d
	or	#0x40
	out	(0xbf),a
	; end call	
	
    pop     hl
    pop     bc
    pop     de
    push    de
    push    bc
    push    hl
    push    de
    exx
    push    hl
    exx
$1:
    push    bc
    ld      b,c
    ld      c,#0xbe ;; (1d47h) = 0beh
$2:
    ld      a,(hl)
    inc     hl
    and     e
    xor     d
    out     (c),a
    djnz    $2
    pop     bc
    ld      c,#0
    dec     b
    ret     z
    jp      p,$1
    ret
