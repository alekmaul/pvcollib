; dcode.s

	.module read_next_code
    
	; global from this code
    .globl _read_next_code
    ; byte count = read_next_code(byte *item, byte code);
	
	.area _CODE

_read_next_code:
                pop bc
                pop hl ; hl = item variable pointer
                pop de ; de = code
                push de
                push hl
                push bc
                ld a,e
                bit 7,a
                jr z, spaces
                inc a
                jr z, egg
                sub #1
                rrca
                rrca
                rrca
                rrca
                rrca
                and #7
                sub #7
                jr nz, stable_ground
                inc a
stable_ground:
                add a,#15
                ld (hl),a
                ld a,e
                and #0x1F
                inc a
                jr rnc_end
egg:
                ld (hl),#0x22
                inc a
                jr rnc_end
spaces:
                or a
                jr nz, spaces1
                ld (hl),#0x00
                inc a
                jr spaces2
spaces1:
                ld (hl),#0x20
                sub #1
                jr z, spaces2
                add a,#2
spaces2:
rnc_end:
                ld l,a
                xor a
                ld h,a
                ret
