/****************************************************************************/
/**                                                                        **/
/**                                 cch.c                                  **/
/**                                                                        **/
/** ColecoVision Cosmo Challenge game main module                          **/
/**                                                                        **/
/** Copyright (C) Marcel de Kogel 1997                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/
#include "cosmoc.h"

/* when his health bar reaches 0, one dies */
static u8 player1_health,player2_health;
/* incremented every interrupt */
static u8 nmi_count;
/* if 1, the title and intro screens have been displayed */
static u8 game_running;
/* 0=one player, 1=two players */
static u8 game_mode;

/* scrolling stones buffer */
static u8 stone_vram[96*2];
/* next stone type to put in buffer */
static u8 stonea,stoneb;

/* scroll stone stuff one character */
static void scroll_stones (void)
{
 register u8 a;
 /* scroll old buffer */
 sys_memcpyb (stone_vram,stone_vram+1,95);
 sys_memcpyb (stone_vram+97,stone_vram+96,95);
 /* check new stones */
 if (!stonea)
 {
  a=sys_random();
  if (a<16) stonea=((a&3)<<2)+128+16+32;
 }
 if (!stoneb)
 {
  a=sys_random();
  if (a<16) stoneb=((a&3)<<2)+3+32;
 }
 /* put new stones in buffer */
 if (stonea)
 {
  stone_vram[31]=stonea-32;
  stone_vram[31+32]=stonea;
  stone_vram[31+64]=stonea+32;
  stonea++;
  if ((stonea&3)==0) stonea=0;
 }
 else
  stone_vram[31]=stone_vram[31+32]=stone_vram[31+64]=96;
 if (stoneb)
 {
  stone_vram[96]=stoneb-32;
  stone_vram[128]=stoneb;
  stone_vram[160]=stoneb+32;
  stoneb--;
  if ((stoneb&3)==3) stoneb=0;
 }
 else
  stone_vram[96]=stone_vram[128]=stone_vram[160]=96;
}

/* The NMI routine. Gets called 50 or 60 times per second */
void nmi (void)
{
 static u8 stone_count;
 register u8 i;
 nmi_count++;
 /* if intro screen is being displayed, return immediately */
 if (!game_running) return;
 /* Update sprites */
 vdp_putvram(sprgen+8*4,sprites,(32-8)*4);
 /*
 update_sprites (32-8,sprgen+8*4);
 */
 
 /* update stone display */
 switch (stone_count)
 {
  case 6:
   /* scroll 2 pixels */
   vdp_putvramex (chrtab+256,stone_vram,96,0xff,128);
   vdp_putvramex (chrtab+384,stone_vram+96,96,0xff,128);
   break;
  case 4:
   /* scroll 4 pixels */
   vdp_putvramex (chrtab+256,stone_vram,96,0xff,16);
   vdp_putvramex (chrtab+384,stone_vram+96,96,0xff,16);
   break;
  case 2:
   /* scroll 6 pixels */
   vdp_putvramex (chrtab+256,stone_vram,96,0xff,128|16);
   vdp_putvramex (chrtab+384,stone_vram+96,96,0xff,128|16);
   break;
  case 1:
   /* update buffer, will be uploaded next call */
   scroll_stones ();
   break;
  case 0:
   /* upload stone stuff to vram */
   vdp_putvram (chrtab+256,stone_vram,96);
   vdp_putvram (chrtab+384,stone_vram+96,96);
   stone_count=8;
   break;
 }
 stone_count--;
 /* update stars */
 if ((nmi_count&1)==0)
 {
  i=((nmi_count>>1)&7)+16;
  sprites[i].y=sys_random();
  sprites[i].x=sys_random();
  sprites[i].pattern=28+(sys_random()&4);
  sprites[i].colour=sys_random()&15;
 }
}

const sprite_t sprites_init[]=
{
 /* ship A */
 { 166,60,4,15 },
 { 166,60,8,4 },
 { 166,60,12,8 },
 /* ship B */
 { 8,180,16,15 },
 { 8,180,20,4 },
 { 8,180,24,8 },
 /* 5 bullets each */
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 { 207,0,0,11 },
 /* 8 stars */
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 },
 { 207,0,28,14 }
};

/* to prevent sprites being displayed on the status bar */
const sprite_t default_sprgen[]=
{
 { -9,0,0,0 },
 { -9,0,0,0 },
 { -9,0,0,0 },
 { -9,0,0,0 },
 { 183,0,0,0 },
 { 183,0,0,0 },
 { 183,0,0,0 },
 { 183,0,0,0 } // Overflow Warning Making no sense!
};

void show_player1_health_bar (u8 n)
{
 register u8 i;
 if (n && player1_health)
 {
  vdp_fillvram (chrtab+736,1,player1_health);
  i=32-player1_health;
  if (i)
   vdp_fillvram (chrtab+736+player1_health,2,i);
 }
 else
  vdp_fillvram (chrtab+736,2,32);
}

void show_player2_health_bar (u8 n)
{
 register u8 i;
 if (n && player2_health)
 {
  vdp_fillvram (chrtab,1,player2_health);
  i=32-player2_health;
  if (i)
   vdp_fillvram (chrtab+player2_health,2,i);
 }
 else
  vdp_fillvram (chrtab,2,32);
}

void player1_is_hit (int spritenr)
{
 sprites[spritenr].y=207;
 if (player1_health)
 {
     player1_health--;
     if (player1_health==4) {
         /* play sound low health */
         snd_startplay(1);
     }
     snd_startplay(3);
     /* play shound hit */
  show_player1_health_bar(1);
 }
}

void player2_is_hit (int spritenr)
{
 sprites[spritenr].y=207;
 if (player2_health)
 {
     player2_health--;
     if (player2_health==4) {
         /* play sound low health */
         snd_startplay(1);
     }
     snd_startplay(3);
     /* play shound hit */
  show_player2_health_bar(1);
 }
}

void stone_is_hit (int spritenr)
{
  sprites[spritenr].y=207;
  snd_startplay(4);
}

u8 check_stone (u8 spritenr)
{
 register unsigned n;
 n=(sprites[spritenr].y)>>3;
 n-=8;
 if (n&0xf8) return 0;
 if (n==3 || n==7) return 0;
 if (n&4) --n;
 n<<=5;
 n+=(sprites[spritenr].x>>3);
 return stone_vram[n]!=96;
}

void _fill_vram (unsigned offset,u8 value,unsigned count)
{
 vdp_disablenmi();
 vdp_fillvram (offset,value,count);
 vdp_enablenmi();
}

static void vdp_showpicture(void *picture)
{
	vdp_disablescr();
	vdp_disablenmi();
	vdp_rle2vram(vdp_rle2vram(picture,coltab),chrgen);
	vdp_enablescr();
	vdp_enablenmi();
}

static void title (void)
{
 /* initialise name table */
 vdp_setmode2bmp();
 /* show title screen */
 vdp_showpicture(title_screen);
 sys_pause();
 /* show intro screen */
 vdp_showpicture(intro_screen);
 sys_pause();
}

static const u8 red_font[8]= { 0x61,0x61,0x81,0x81,0x81,0x91,0x91,0x91 };
static const u8 yellow_font[8]= { 0xa1,0xa1,0xb1,0xb1,0xb1,0xf1,0xf1,0xf1 };

void init_vdp (void)
{
 u8 i;
 vdp_setmode2bmp();
 /* Disable Interrupt */
 vdp_disablenmi();
 /* special color for the middle section */   
 vdp_setreg(3,0xbf); 
 /* Upload first 8 sprites */
 vdp_putvram (sprgen,default_sprgen,sizeof(default_sprgen));
 /* Fill colour table */
 vdp_putvram_repeat (coltab,yellow_font,8,0); // 0 = 256
 vdp_putvram_repeat (coltab+'0'*8,red_font,8,10);
 /* Clear Characters Pattern Table */
 vdp_fillvram (chrgen,0x00,0x0800);
 /* Upload ASCII character patterns */
 vdp_setdefaultchar(FNTBOLD_ITALIC); // (29,128-29,chrgen+29*8,BOLD_ITALIC);
 /* Health indicator */
 vdp_fillvram (chrgen+9,0x55,6);
 vdp_fillvram (coltab+8,0x90,16);
 /* Duplicate Charset to the other 2 screen section */
 vdp_duplicatevram();
 /* Upload stones character definitions */
 vdp_rle2vram (vdp_rle2vram(stone_characters,coltab+256*8),chrgen+256*8);
 /* Upload sprite patterns */
 vdp_rle2vram (sprite_patterns,sprtab);
 /* clear screen display */
 vdp_fillvram (chrtab,32,768);
 /* Fill part 2 of name table */
 vdp_fillvram (chrtab+256,96,256);
 /* Scroll in stones */
 for (i=32;i!=0;--i) scroll_stones ();
 /* Blue screen border */
 vdp_setreg(7,0xf4);
 /* turn display on */
 vdp_enablescr();
 /* enable NMI */
 vdp_enablenmi();
}

void choice (void)
{

 _fill_vram (chrtab+0,1,32);
 _fill_vram (chrtab+736,1,32);
 _fill_vram (chrtab+18*32,0,64);
 
 vdp_putstring(8,18,"1 - One player ");
 vdp_putstring(8,19,"2 - Two players");
 /* wait until all keys released */
 while (keypad_1!=0xff || keypad_2!=0xff);
 /* get choice */
 game_mode=255;
 while (game_mode==255)
 {
  if (keypad_1==1 || keypad_2==1) game_mode=0;
  if (keypad_1==2 || keypad_2==2) game_mode=1;
 }
 
 _fill_vram (chrtab+18*32,0,64);
 
}

void main (void)
{
  u8 i;
  u8 old_joypad_1,old_joypad_2;
  snd_settable(snd_table);
  title ();
  init_vdp ();
  /* set game_running flag, enables scrolling stones and blinking stars */
  game_running=1;
  /* clear NMI counter */
  nmi_count=255;
options:
  choice ();
play_again:
  player1_health=player2_health=32;
  show_player1_health_bar(1);
  show_player2_health_bar(1);
  sys_memcpyb (sprites,sprites_init,sizeof(sprites_init));
  old_joypad_1 = 8;
  old_joypad_2 = (sys_randbyte(0,1) == 0) ? 2 : 8;
  vdp_putstring(11,18,"Get ready");
  snd_startplay(9);
  snd_startplay(10);
  snd_startplay(11);
  vdp_waitvblank(150);
  
  _fill_vram (chrtab+18*32,0,64);
  
  old_joypad_1=0;
  old_joypad_2=0;
  while (1)
  {
   /* Wait for a VBLANK */
   vdp_waitvblank(1);
   if ((nmi_count&31)==0)
   {
    if (player1_health<5 || player2_health<5)
    {
     if (player1_health<5) show_player1_health_bar(nmi_count&32);
     if (player2_health<5) show_player2_health_bar(nmi_count&32);
    }
   }
   /* Let computer play */
   if (!game_mode)
   {
    /* direction, slightly random */
    joypad_2=old_joypad_2&0x0f;
    i=sys_random();
     if (i<24)
     {
         if (sprites[0].x<sprites[3].x)
         {
             joypad_2 = 8;
         }
         else
         {
             joypad_2 = 2;
         }
     }
    /* fire ? */
    if ((nmi_count&7)==0) joypad_2|=0x10;
   }
   /* Parse joysticks */
   if (joypad_1&2)
   {
    if (sprites[0].x<241)
     sprites[1].x=sprites[2].x=(++sprites[0].x);
   }
   if (joypad_1&8)
   {
    if (sprites[0].x)
     sprites[1].x=sprites[2].x=(--sprites[0].x);
   }
   if (joypad_2&2)
   {
    if (sprites[3].x<241)
     sprites[4].x=sprites[5].x=(++sprites[3].x);
   }
   if (joypad_2&8)
   {
    if (sprites[3].x)
     sprites[4].x=sprites[5].x=(--sprites[3].x);
   }
   if ((joypad_1^old_joypad_1)&joypad_1&0xf0)
   {
    for (i=6;i<11;++i) if (sprites[i].y==207) break;
    if (i<11)
    {
     sprites[i].y=sprites[0].y;
     sprites[i].x=sprites[0].x+7;
     snd_startplay(2);
    }
   }
   if ((joypad_2^old_joypad_2)&joypad_2&0xf0)
   {
    for (i=11;i<16;++i) if (sprites[i].y==207) break;
    if (i<16)
    {
     sprites[i].y=sprites[3].y+14;
     sprites[i].x=sprites[3].x+7;
     snd_startplay(2);
    }
   }
   old_joypad_1=joypad_1;
   old_joypad_2=joypad_2;
   if (player1_health && player2_health)
   {
    for (i=6;i<11;++i)
    {
     if (sprites[i].y!=207)
     {
      if (check_stone(i)) {
         stone_is_hit (i);
      }
      if (check_collision(sprites+i,sprites+3,0x303,0x303,0x1000,0x0e00)) {
         player2_is_hit (i);
      }
	  sprites[i].y-=4;
      if (sprites[i].y>192) sprites[i].y=207; // the warning makes no sense
     }
    }
    for (i=11;i<16;++i)
    {
     if (sprites[i].y!=207)
     {
      if (check_stone(i))
       stone_is_hit (i);
      if (check_collision(sprites+i,sprites+0,0x303,0x303,0x1000,0x0e02))
       player1_is_hit (i);
	   sprites[i].y+=4;
      if (sprites[i].y>192) sprites[i].y=207; // the warning makes no sense
     }
    }
   }
   else break;
  }
  i = 0;
  if (player1_health) {
      i = 3;
  }
  snd_startplay(5);
  snd_startplay(6);
  snd_startplay(7);
  snd_startplay(8);
  sprites[i].y=sprites[i+1].y=sprites[i+2].y=207;
  spr_clear(); // PROBLEM!!! it's supposed to be clear_sprites (start=6,count=10);
  vdp_putstring(9,18,"1 - Play again");
  vdp_putstring(9,19,"2 - Options   ");
  vdp_putstring(9,20,"3 - Intro     ");
  while ((keypad_1==0 || keypad_1>3) &&
         (keypad_2==0 || keypad_2>3));
  if (keypad_1==0 || keypad_1>3) i=keypad_2;
  else i=keypad_1;
  
  _fill_vram (chrtab+18*32,0,3*32);
  
  if (i==1) goto play_again;
  if (i==2) goto options;
  /* startup module will issue a soft reboot */
}
