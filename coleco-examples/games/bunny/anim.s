; anim.s

	.module animation
    
    .globl  _sprites
	; global from this code
    .globl _invincible
    ; void invincible (byte i);
	
	.area _CODE
                
stars_dx:
                .db #0x00,#0x05,#0x08,#0x0B,#0x0C,#0x0B,#0x08,#0x05
		.db #0x00,#0xFA,#0xF7,#0xF4,#0xF3,#0xF4,#0xF7,#0xFA
stars_dy:
                .db #0xEC,#0xEE,#0xF1,#0xF6,#0xFB,#0x02,#0x07,#0x0B
		.db #0x0C,#0x0B,#0x07,#0x02,#0xFB,#0xF6,#0xF1,#0xEE               
                
_invincible:
                pop bc
                pop de  ; e = i
                push de
                push bc
                ld a,e
                and #15
                push af ; i and 15
                xor a ; a = 0
                ld d,a
                pop af
                ld e,a
                ld hl,#stars_dx
                add hl,de
                push hl ; hl = &starts_dx + (i and 15)
                ld e,#16
                add hl,de
                push hl ; hl = &starts_dy + (i and 15)
                ld hl,#_sprites ; hl = &sprites[0].y
                ld b,(hl) ; b = sprites[0].y
                pop hl ; hl = &stars_dy + (i and 15)
                ld a,(hl) ; a = stars_dy[i&15]
                add a,b ; a += b
                ld (#_sprites+16),a ; sprites[4].y = a
                ld hl,#_sprites+1 ; hl = &sprites[0].x
                ld b,(hl) ; b = sprites[0].y
                pop hl ; hl = &starts_dx + (i and 15)
                ld a,(hl) ; a = stars_dx[i&15]
                add a,b ; a += b
                ld (#_sprites+17),a ; sprites[4].x = a
                ld a,#0x27
                ld (#_sprites+18),a ; sprites[4].pattern = 0x27h
                ld a,#0x05
                ld (#_sprites+19),a ; sprites[4].colour = 5 /* light blue */
                ld a,#0xd0
                ld (#_sprites+20),a ; sprites[5].y = 0xd0 /* stop sprites */
                ret
                
