#ifndef _COSMOC_MAIN_
#define _COSMOC_MAIN_

#include <coleco.h>

extern void vdp_putvramex (unsigned offset,void *ptr,unsigned count,u8 and_mask,u8 xor_mask);
extern u8 check_collision (sprite_t *sprite1,sprite_t *sprite2,
                      unsigned sprite1_size_hor,unsigned sprite1_size_vert,
                      unsigned sprite2_size_hor,unsigned sprite2_size_vert);

extern const sound_t snd_table[];

/* various RLE-encoded tables (in tables.c) */
extern const u8 title_screen[];
extern const u8 intro_screen[];
extern const u8 stone_characters[];
extern const u8 sprite_patterns[];

#endif