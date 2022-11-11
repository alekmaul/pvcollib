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
	.module pvclsound4
	

	; global from external entries / code
	.globl _mus_pointer
	.globl _mus_counter

	; global from this module
    .globl  _mus_nextplay
    .globl  _mus_startplay
    .globl  _mus_stopplay
    .globl  _mus_update

	.area _CODE
    
;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------


;---------------------------------------------------------------------------------
; Here begin routines that can be call from programs
;---------------------------------------------------------------------------------
nomusic_track:
    .dw 0

;
_mus_nextplay:
    pop bc
    pop de
    push de
    push bc
next_music:
    ld hl,#_mus_pointer
    ld (hl),e
    inc hl
    ld (hl),d
    ret

;
_mus_startplay:
    pop bc
    pop de
    push de
    push bc
    call next_music
    jp trigger_sounds

;
_mus_update:
    ld hl,#_mus_counter+1
    ld a,(hl)
    dec hl
    or a
    jr nz,update_counter
    ld a,(hl)
    or a
    jp z,trigger_sounds
update_counter:
    ld a,(hl)
    sub #1
    ld (hl),a
    inc hl
    ld a,(hl)
    sbc a,#0
    ld (hl),a
    ret
trigger_sounds:
    ld hl,#_mus_pointer
    ld e,(hl)
    inc hl
    ld d,(hl)
    ex de,hl
    ld e,(hl)
    inc hl
    ld d,(hl)
    xor a
    ;; CASE == 0000 -> it's END OF DATA marker
    sub d
    jr nz, trigger_sounds1
    sub e
    ret z
trigger_sounds1:
    ;; CASE >= 8000 -> it's a new _mus_pointer
    bit 7,d
    jr z, trigger_sounds2
    ld hl,#_mus_pointer
    ld (hl),e
    inc hl
    ld (hl),d
    jr trigger_sounds
trigger_sounds2:
    ;; _mus_counter = (uint) data[_mus_pointer];
    dec de
    ld hl,#_mus_counter
    ld (hl),e
    inc hl
    ld (hl),d
    ld hl,#_mus_pointer
    ld e,(hl)
    inc hl
    ld d,(hl)
    call stop_music
    ex de,hl
    ;; hl = address;
    inc hl
    inc hl
    ld a,(hl)
    rlca
    rlca
    and #3
    inc a
    ld b,a
    ;; b is element of {1,2,3,4}
trigger_sounds4:
    ld a,(hl)
    inc hl
    and #0x3f
    ;; MISSING : start sound #a
    
    push bc
    push de
    push hl

    ld      b,a
    call	0x1ff1

;    push af
;    sub #1
;    ld c,a
;    ld b,#0
;    ld hl,(#0x7020)
;    rlc c
;    rlc c
;    add hl,bc
;    ld e,(hl)
;    inc hl
;    ld d,(hl)
;   push de
;    inc hl
;    ld e,(hl)
;    inc hl
;    ld d,(hl)
;    pop hl ;; de = @ Sound Area, hl = @ sound data
;    ld a,(hl)
;    inc hl
;    ld c,a
;    and #0xc0
;    ld b,a
;    pop af
;    and #0x3f
;    or b
;    ld (de),a
;    inc de
;    inc de
;    inc de ;; de = @ Sound Area + 3
    
;    ld a,c
;    bit 5,a
;    jr z, $5
;    inc de
;    inc de
;    and #0x1f
;    ld (de),a
;    jr $6    

;$5:
;    cp #0x02 ; case noise with volumen sweep
;    jr nz, $7
;    dec hl
;$7:
;    ldi
;    ldi
;    ldi
;    bit 0,a
;    jr z,$8
;    ldi
;    ldi
;$10:
;    bit 1,a
;    jr z,$9
;    ldi
;    ldi
;    dec de
;    dec de
;$9:
;    dec de
;    dec de
;    dec de
;    dec de
;$6:
;    dec de
;    dec de
;    ex de,hl
;    ld (hl),d
;    dec hl
;    ld (hl),e
;    ex de, hl

    pop hl
    pop de
    pop bc
    
    djnz trigger_sounds4
    ex de,hl
    ;; update _mus_pointer
    ld hl,#_mus_pointer
    ld (hl),e
    inc hl
    ld (hl),d

    jp 0x0295 ;; NOT AN APPROVED COLECO BIOS ENTRY, BUT DONE ANYWAY

;$8:
;    inc de
;    inc de
;    jr $10

;    
_mus_stopplay:
    ld hl,#_mus_pointer
    ld de,#nomusic_track
    ld (hl),e
    inc hl
    ld (hl),d
    ; inc hl
    ; xor a
    ; ld (hl),a
    ; inc hl
    ; ld (hl),a    
    call stop_music
    jp 0x0295 ;; NOT AN APPROVED COLECO BIOS ENTRY, BUT DONE ANYWAY

stop_music:
    ;; Inactive All 4 Sound Areas
    ld b,#4
    xor a
    ld hl,#0x702b ; Address for the 1st Sound Area
stop_music3:
    ld (hl),#0xff
    inc hl
    inc hl
    inc hl
    inc hl
    ld (hl),#0xf0
    inc hl
    ld (hl),a
    inc hl
    inc hl
    ld (hl),a
    inc hl
    ld (hl),a
    inc hl
    inc hl
    djnz stop_music3
    ret
