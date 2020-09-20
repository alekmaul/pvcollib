; setsprit.s

	.module setsprite
    .globl  _sprites
	; global from this code
    .globl _player_sprite,_player_color
    ; void player_sprite (byte i);
    ; void player_color (void);
	
	.area _CODE

_player_sprite:
                pop bc
                pop de
                push de
                push bc
                ld a,e
                add a,#2
                ld (#_sprites+2),a
                inc a ;add a,1
                ld (#_sprites+6),a
                sub #3
                ld (#_sprites+10),a
                inc a ;add a,1
                ld (#_sprites+14),a
                ret
                
                ; void player_color (void);
_player_color:
                ld a,#14
                ld (#_sprites+3),a
                ld (#_sprites+11),a
                inc a ;add a,1
                ld (#_sprites+7),a
                sub #2
                ld (#_sprites+15),a
                ret
