;---------------------------------------------------------------------------------
;
;   Copyright (C) 2018-2026
;         Alekmaul 
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
    .module pvclvideo13_2
    
chrgen      .equ    0x0000      ; Pattern Generator Table (6144 bytes)
coltab      .equ    0x2000      ; Color Table (6144 bytes)

vdpdata     .equ    0xBE        ; VRAM data port (read/write)
vdpctrl     .equ    0xBF        ; VRAM control port / registers


    ; global from external entries / code

    ; global from this module
    .globl _vdp_putareabmp

    .area   _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
_vid13setwrt:
    push    af
    ld      a,l
    out     (vdpctrl),a
    ld      a,h
    or      #0x40               ; bit 6 = write
    out     (vdpctrl),a
    pop     af
    ret

;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
_vdp_putareabmp:
    push    ix
    ld      ix,#0
    add     ix,sp
 
    ; load source pointers into the alternate register bank
    exx
    ld      l,8(ix)
    ld      h,9(ix)             ; HL' = source pattern pointer
    ld      e,10(ix)
    ld      d,11(ix)            ; DE' = source color pointer
    exx
 
_cv_pa_rowloop:
    ; address of the first tile of the row: H=ytile, L=xtile*8
    ld      a,4(ix)             ; xtile (starting column, fixed)
    add     a,a
    add     a,a
    add     a,a
    ld      l,a
    ld      a,5(ix)             ; current ytile
    ld      h,a
 
    ld      b,6(ix)             ; B = column counter = w (tiles)
 
_cv_pa_colloop:
    ; ---- write 8 bytes of PATTERN for this tile ----
    call    _vid13setwrt               ; PATTBL=0x0000, address = HL directly
    exx
    ld      c,#vdpdata
    ld      b,#8
    otir                        ; copy 8 bytes, HL' (pattern) advances
    exx
 
    ; ---- write 8 bytes of COLOR for the same tile ----
    push    hl                  ; save tile address (pattern)
    ld      de,#coltab
    add     hl,de
    call    _vid13setwrt                ; VRAM color address for this tile
    pop     hl                  ; restore tile address
 
    exx
    ex      de,hl               ; HL=colorPtr, DE=patternPtr(already advanced)
    ld      c,#vdpdata
    ld      b,#8
    otir                        ; copy 8 bytes, HL (color) advances
    ex      de,hl               ; put back HL=patternPtr, DE=colorPtr(advanced)
    exx
 
    ; ---- next tile: address += 8, next column ----
    ld      de,#8
    add     hl,de
    djnz    _cv_pa_colloop
 
    ; ---- next tile row ----
    ld      a,5(ix)
    inc     a
    ld      5(ix),a             ; ytile++ (recycle stack location)
 
    ld      a,7(ix)
    dec     a
    ld      7(ix),a             ; h-- (recycle stack location)
    jr      nz,_cv_pa_rowloop
 
    pop     ix
    ret