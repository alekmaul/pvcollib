;---------------------------------------------------------------------------------
;
;   Copyright (C) 2018-2026
;       Alekmaul 
;
;   This software is provided 'as-is', without any express or implied
;   warranty.  In no event will the authors be held liable for any
;   damages arising from the use of this software.
;
;   Permission is granted to anyone to use this software for any
;   purpose, including commercial applications, and to alter it and
;   redistribute it freely, subject to the following restrictions:
;
;   1.  The origin of this software must not be misrepresented; you
;       must not claim that you wrote the original software. If you use
;       this software in a product, an acknowledgment in the product
;       documentation would be appreciated but is not required.
;   2.  Altered source versions must be plainly marked as such, and
;       must not be misrepresented as being the original software.
;   3.  This notice may not be removed or altered from any source
;       distribution.
;
;---------------------------------------------------------------------------------
    .module pvclvideo30

vdpdata     .equ    0xBE        ; VRAM data port (read/write)
vdpctrl     .equ    0xBF        ; VDP control port / registers

    ; global from external entries / code
    
    ; global from this module
    .globl _vdp_linebmp
    
    .area _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_linebmp::
    push    ix
    ld      ix,#0
    add     ix,sp
    ld      d,4(ix)
    ld      e,5(ix)
    ld      h,6(ix)
    ld      l,7(ix)
    call    _cv_linebmp_r
    pop     ix
    ret
 
_cv_linebmp_r:
    ld      a,h                 ; dx = |x1-x0|, sx = +/-1 -> B
    sub     d
    ld      b,#1
    jr      nc,lb_xp
    neg
    ld      b,#0xFF
lb_xp:
    ld      h,a                 ; H = dx
    ld      a,l                 ; dy = |y1-y0|, sy = +/-1 -> C
    sub     e
    ld      c,#1
    jr      nc,lb_yp
    neg
    ld      c,#0xFF
lb_yp:
    ld      l,a                 ; L = dy
    push    bc                  ; sx,sy  -> H',L'
    push    hl                  ; dx,dy  -> D',E'
    exx
    pop     de
    pop     hl
    ld      a,d
    cp      e
    jr      c,lb_ysetup         ; dx < dy -> major Y axis
    ld      b,d                 ; counter = major + 1 (256 if major = 255 : DJNZ ok)
    inc     b
    srl     a
    ld      c,a                 ; error = major/2
    exx
    jp      lb_xloop
lb_ysetup:
    ld      a,d                 ; swap : D' = dy (major), E' = dx (minor)
    ld      d,e
    ld      e,a
    ld      b,d
    inc     b
    ld      a,d
    srl     a
    ld      c,a
    exx
    jp      lb_yloop
 
; ---- major X axis : x always advances, y advances sometimes ----
lb_xloop:
    ; ---- turn on pixel (D=x, E=y) : VRAM read-modify-write ----
    ld      a,e                 ; low address = (x & 0xF8) | (y & 7)
    and     #0x07
    ld      c,a
    ld      a,d
    and     #0xF8
    or      c
    ld      c,a                 ; C = low byte of VRAM address
    ld      a,e                 ; high address = y >> 3
    rrca
    rrca
    rrca
    and     #0x1F
    ld      b,a                 ; B = high byte of VRAM address
    ld      hl,#lb_masktab      ; mask = table[x & 7]  (independent of alignment)
    ld      a,d
    and     #0x07
    add     a,l
    ld      l,a
    adc     a,h
    sub     l
    ld      h,a
    ld      a,(hl)
    ld      l,a                 ; L = mask
    ld      a,c                 ; read address
    out     (#vdpctrl),a
    ld      a,b
    out     (#vdpctrl),a
    in      a,(#vdpdata)
    or      l
    ld      h,a                 ; H = new byte
    ld      a,c                 ; write address
    out     (#vdpctrl),a
    ld      a,b
    or      #0x40
    out     (#vdpctrl),a
    ld      a,h
    out     (#vdpdata),a
    exx
    djnz    lb_xstep            ; B'-- ; more steps?
    exx
    ret
lb_xstep:
    ld      a,c
    add     a,e                 ; error + minor
    jr      c,lb_xw
    cp      d
    jr      nc,lb_xw            ; error >= major ?
    ld      c,a                 ; no minor step
    ld      a,h                 ; x += sx
    exx
    add     a,d
    ld      d,a
    jp      lb_xloop
lb_xw:
    sub     d
    ld      c,a
    ld      a,l                 ; y += sy
    exx
    add     a,e
    ld      e,a
    exx
    ld      a,h                 ; x += sx
    exx
    add     a,d
    ld      d,a
    jp      lb_xloop
 
; ---- major Y axis : y always advances, x advances sometimes ----
lb_yloop:
    ; ---- turn on pixel (D=x, E=y) : VRAM read-modify-write ----
    ld      a,e                 ; low address = (x & 0xF8) | (y & 7)
    and     #0x07
    ld      c,a
    ld      a,d
    and     #0xF8
    or      c
    ld      c,a                 ; C = low byte of VRAM address
    ld      a,e                 ; high address = y >> 3
    rrca
    rrca
    rrca
    and     #0x1F
    ld      b,a                 ; B = high byte of VRAM address
    ld      hl,#lb_masktab      ; mask = table[x & 7]  (independent of alignment)
    ld      a,d
    and     #0x07
    add     a,l
    ld      l,a
    adc     a,h
    sub     l
    ld      h,a
    ld      a,(hl)
    ld      l,a                 ; L = mask
    ld      a,c                 ; read address
    out     (#vdpctrl),a
    ld      a,b
    out     (#vdpctrl),a
    in      a,(#vdpdata)
    or      l
    ld      h,a                 ; H = new byte
    ld      a,c                 ; write address
    out     (#vdpctrl),a
    ld      a,b
    or      #0x40
    out     (#vdpctrl),a
    ld      a,h
    out     (#vdpdata),a
    exx
    djnz    lb_ystep
    exx
    ret
lb_ystep:
    ld      a,c
    add     a,e
    jr      c,lb_yw
    cp      d
    jr      nc,lb_yw
    ld      c,a
    ld      a,l                 ; y += sy
    exx
    add     a,e
    ld      e,a
    jp      lb_yloop
lb_yw:
    sub     d
    ld      c,a
    ld      a,h                 ; x += sx
    exx
    add     a,d
    ld      d,a
    exx
    ld      a,l                 ; y += sy
    exx
    add     a,e
    ld      e,a
    jp      lb_yloop
 
; bit mask from most to least significant, indexed by (x & 7) (ROM)
lb_masktab:
    .db     0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01