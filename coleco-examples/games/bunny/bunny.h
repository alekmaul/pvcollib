#ifndef _BUNNY_MAIN_
#define _BUNNY_MAIN_

#include <coleco.h>

#define NONE 0x00
#define ICE 0x01
#define GRASS 0x02
#define SAND 0x04
#define AIR 0x08

extern const sound_t snd_table[];

extern u8 read_next_code(u8 *item, u8 code);
extern void invincible(u8 i);
extern void player_sprite (u8 i);
extern void player_color (void);

/* Data from "graphics.c" to initialise graphics */
extern const u8 NAMERLE[];
extern const u8 PATTERNRLE[];
extern const u8 COLORRLE[];

extern const u8* levels[];

#endif
