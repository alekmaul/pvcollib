/*---------------------------------------------------------------------------------


	small game from coder Amy Purple : Easter Bunny
		made by her during 2007 MiniGame Competition 
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "bunny.h"

volatile u8 color_offset;
volatile u8 flag_effect;

/* game levels */

static u8 no_level;

/* game properties */
u8 speed;
const int gravity = 384;
const int friction[] = {0,80,320,200,500,290,410,300,1024};

const int vy_min = -2048;
const int vy_max = 2048;
const int vx_min = -1024;
const int vx_max = 1024;

typedef struct
{
        unsigned y;
        unsigned x;
        int vy;
        int vx;
        int speed;
        u8 state;
} player_t;

player_t player;

u8 nbr_keys_left;

static void updatey_(u8 y)
{
        sprites[0].y = y;
        sprites[1].y = y;
        y-=8;
        sprites[2].y = y;
        sprites[3].y = y;
}

static void updatey(void)
{
        u8 y;
        y = player.y>>8;
        updatey_(y);
}

static void updatex_(u8 x)
{
        sprites[0].x = x;
        sprites[1].x = x;
        sprites[2].x = x;
        sprites[3].x = x;
}

static void updatex(void)
{
        u8 x;
        x = player.x>>8;
        updatex_(x);
}

static void slide_sprite(void)
{
    u8 i = sprites[0].x;
    u8 j = player.x>>8;
    if (i<j) i++;
    if (i>j) i--;
    updatex_(i);
    i = sprites[0].y;
    j = player.y>>8;
    if (i<j) i++;
    if (i>j) i--;
    updatey_(i);
}

static void smoke(void)
{
        sprites[4].y = (player.y>>8)+2;
        sprites[4].pattern = 0x26;
        sprites[4].colour = 0x0f;
}

static void update_smoke(void)
{
        if (sprites[4].pattern == 0x2a)
        {
                sprites[4].y = (u8) 0xf0;
        }
        else
        {
                sprites[4].y -= 4;
                sprites[4].x = player.x>>8;
                sprites[4].pattern++;
        }
}

static void bird(void)
{
        sprites[5].x = 128;
        sprites[5].y = 0;
        sprites[5].pattern = 44;
        sprites[5].colour = 0x0f;
}

static void update_bird(void)
{
    u8 i;
    u8 j;
    i = sprites[5].y;
    j = sprites[0].y;
    if (i<j) i++;
    if (i>j) i--;
    sprites[5].y=i;
    i = sprites[5].x;
    j = sprites[0].x;
    if (i<j) { i++; sprites[5].pattern=44+(i&1); }
    if (i>j) { i--; sprites[5].pattern=46+(i&1); }
    sprites[5].x=i;
}

char bird_kill(void)
{
    u8 kill = 0;
    u8 dx = (sprites[0].x-sprites[5].x);
    u8 dy = (sprites[0].y-sprites[5].y);
	dx = (dx>0x7f) ? dx : dx&0x7f;
	dy = (dy>0x7f) ? dy : dy&0x7f;
    if ( dx < 3 && dy < 5) kill = (u8)-1;
    return kill;
}

static void update_state()
{
        u8 i;
        u8 c[2];
        u8 cx[2];
        u8 cy;
        int rest_y;
        u8 nbr = 2;

        cx[0] = sprites[0].x + 2;
        cx[0] >>= 3;
        cx[1] = sprites[0].x + 5;
        cx[1] >>= 3;

        if (cx[0]==cx[1]) nbr=1;

        cy = sprites[0].y+9;
        rest_y = cy & 7;
        rest_y <<=8;
        cy >>= 3;

        player.state = AIR;
        for (i=0;i<nbr;i++)
        {
                c[i] = vdp_getchar(cx[i],cy);
                if ((c[i]&0xf0)==0x10)
                {
                        c[i]++;
                        vdp_putchar(cx[i],cy,c[i]);
                        if ((player.state & AIR) && (player.vy>=0))
                        {
                                player.state = NONE;
                                player.y -= rest_y;
                                player.y &= 0xff00;
                                updatey();
                        }
                        player.state |= GRASS;
                }
                else
                {
                        if ((c[i]&0xfc)==0x0c)
                        {
                                if ((player.state & AIR) && (player.vy>=0))
                                {
                                        player.state = NONE;
                                        player.y -= rest_y;
                                        player.y &= 0xff00;
                                        updatey();
                                }
                                if (c[i]==0x0c)
                                {
                                        player.state |= ICE;
                                }
                                if (c[i]==0x0d)
                                {
                                        player.state |= GRASS;
                                }
                                if (c[i]==0x0e)
                                {
                                        player.state |= SAND;
                                }
                                if (c[i]==0x0f)
                                {
                                        player.vy = vy_min<<1;
                                        player.state = AIR;
                                        smoke();
                                        snd_startplay(3);
                                        i = nbr;
                                }
                        }
                }
        }
        i = sprites[0].x + 3;
        i >>= 3;
        cy = sprites[0].y - 6;
        cy >>= 3;
        c[0] = vdp_getchar(i,cy);
        c[1] = vdp_getchar(i,cy+1);
        if (c[0]==0x21)
        {
                vdp_putchar(i,cy,0x20);
                nbr_keys_left--;
                snd_startplay(1);
        }
        if (c[1]==0x21)
        {
                vdp_putchar(i,cy+1,0x20);
                nbr_keys_left--;
                snd_startplay(1);
        }
}

static void update_player_y(void)
{
        if (player.state & AIR)
        {
                player.vy += gravity;
                if (player.vy > vy_max) player.vy = vy_max;
        }
        else
        {
                player.vy = 0;
                if (joypad_1 & PAD_FIRE1)
                {
                        player.vy = vy_min;
                        player.state = AIR;
                        snd_startplay(2);
                }
        }
        player.y += player.vy;
        updatey();
}

static void update_player_x(void)
{
        int abs_vx = player.vx & 0x7fff;

        if (joypad_1 & (PAD_LEFT | PAD_RIGHT))
        {
                if (joypad_1 & PAD_LEFT) player.vx -= player.speed;
                if (joypad_1 & PAD_RIGHT) player.vx += player.speed;
        }
        else
        {
                if (abs_vx > 0)
                {
                        if ((player.state & AIR)!=AIR)
                        {
                                int f = friction[player.state];
                                if (player.vx<0)
                                {
                                        if (abs_vx > f)
                                        {
                                                player.vx += f;
                                        }
                                        else
                                        {
                                                player.vx = 0;
                                        }
                                }
                                else
                                {
                                        if (abs_vx > f)
                                        {
                                                player.vx -= f;
                                        }
                                        else
                                        {
                                                player.vx = 0;
                                        }
                                }
                        }
                }
        }

        if (player.vx < vx_min) player.vx = vx_min;
        if (player.vx > vx_max) player.vx = vx_max;

        player.x += player.vx;
        if (player.x < 0x1000 || player.x > 0xE800)
        {
                player.vx = -player.vx;
                player.x += player.vx;
        }
}

static void update_player(void)
{
        update_player_x();
        update_player_y();
        update_state();
        /* Set Player Sprite Pattern */
        if (player.vx>128)
        {
            player_sprite(4);
        }
        if (player.vx<128)
        {
            player_sprite(8);
        }
        if (player.vx==0)
        {
            player_sprite(0);
        }
        /* Set Player Sprite Position */
        updatey();
        updatex();
        /* update smoke animation */
        update_smoke();
        /* update bird animation */
        update_bird();
        /* update_eggs(); */
        /* Update Sprites on Screen */
        vdp_putvram(sprgen, sprites, 25);
}

static const u8 floor_pattern[] = {0x99,0x66,0xdd,0x77,0xdd,0x77,0x99,0x66};

/* Initialise characters graphics in VideoRAM */
static void init_graphics(void)
{
        /* Load characters pattern and color */
        vdp_rle2vram(PATTERNRLE,chrgen);
		vdp_putvram_repeat (0x0060,floor_pattern,8,7);

        vdp_duplicatevram();
        vdp_rle2vram(COLORRLE,0x2000);
        /* Set sprites pattern as characters pattern */
        vdp_setreg(6,0);
}

const u8 left_border[]={0x24,0x23};
const u8 right_border[]={0x25,0x26};

static void decode_level(u8 *level)
{
	u8 x;
	u8 count;
	u8 item;
	u8 y=0;
	u8 *offset = level;
	nbr_keys_left = 0;
	count = read_next_code(&item, *offset++);
loop_y:
	vdp_waitvblank(1);
	vdp_putchar(0,y,left_border[0]);
	vdp_putchar(1,y,left_border[1]);
	x = 2;
loop_x:
	if (y==23 && item==0x20) item=0xf;
	if (item==0x22)
	{
		vdp_putchar(x,y-1,0x21);
		nbr_keys_left++;
	}
	if (item==0)
	{
		item=0x20;
		player.x = (unsigned) x;
		player.x <<= 11;
		player.y = (unsigned) y;
		player.y <<= 3;
		player.y--;
		player.y <<= 8;
		player.vx = 0;
		player.vy = 0;
		player.speed = 128;
		player.state = NONE;
	}
	vdp_putchar(x,y,item);
	count--;
	if (count==0) count = read_next_code(&item, *offset++);
	x++;
	if (x<30) goto loop_x;
	vdp_putchar(30,y,right_border[0]);
	vdp_putchar(31,y,right_border[1]);
	y++;
	if (y<24) goto loop_y;
}

static void anim_getready(void)
{
    u8 i;
    player_sprite(0);

    for (i=0;i<224;i++)
    {
        vdp_waitvblank(1);
        slide_sprite();
        invincible(i);
        vdp_putvram(sprgen, sprites, 25);
    }
    vdp_waitvblank(1);
    sprites[4].y = 0xd0;
    sprites[5].y = 0xd0;
    vdp_putvram(sprgen, sprites, 25);
}

static void game(void)
{
        u8 level_done = 0;
        decode_level((char*) levels[no_level&7]);
        snd_startplay(4);
        anim_getready();
        bird();
        /* Start BGSound */
        while(!level_done)
        {
                vdp_waitvblank(speed);
                update_player();
                if (nbr_keys_left == 0) {no_level++; level_done = (u8)-1;}
                if (bird_kill()) {snd_startplay(5); level_done=(u8)-1;}
                /* if (player.y >= 0xE000 && player.y <= 0xF000) level_done = -1; */
        }
        vdp_waitvblank(30);
        /* Stop All Sounds */
        snd_stopplay(1);
        snd_stopplay(2);
        snd_stopplay(3);
        vdp_waitvblank(1);
}

/* main function : starting point of any C program */
void main(void)
{
        snd_settable(snd_table);
        /* Default screen mode 2 setting is done in crtcv.as file */
        vdp_setmode2txt();
        /* Set Sprites to 8x8 pixels */
        spr_mode8x8();
        /* Initialise graphics and sounds */
        init_graphics();

        vdp_rle2vram(NAMERLE,0x1800);
        vdp_enablevdp();
    
        snd_startplay(6);
        snd_startplay(7);
        flag_effect = (u8)-1;
		vdp_enablenmi(); // NMI running from now till the end of the game!
        speed = 5 - sys_choice_keypad1(1,3); //choice_keypad1(1,3);
        flag_effect = 0;
        snd_stopplay(6);
        snd_stopplay(7);
    
        speed = 2;
        /* Set Player Sprite Color */
        player_color();
        /* Play game */
        no_level=0;
        while(1) game();
        /* END */
}

static const u8 colors[] = {
    0xd1,0xd1,0xd1,0x51,
    0xd1,0x51,0x51,0x51,
    0xc1,0x51,0xc1,0xc1,
    0xc1,0x21,0xc1,0x21,
    0x21,0x21,0x31,0x21,
    0x31,0x31,0x31,0xa1,
    0x31,0xa1,0xa1,0xa1,
    0x91,0xa1,0x91,0x91,
    0x91,0x81,0x91,0x81,
    0x81,0x81,0xd1,0x81,
    0xd1,0xd1,0xd1,0x51,
    0xd1,0x51,0x51,0x51,
    0xc1,0x51,0xc1,0xc1,
    0xc1,0x21,0xc1,0x21,
    };

void nmi(void)
{
    if (flag_effect)
    {
        color_offset++;
        if (color_offset > 39) color_offset = 0;
		vdp_putvram_repeat (0x2400,&colors[color_offset],8,12);
		vdp_putvram_repeat (0x2460,&colors[color_offset+8],8,12);    
    }    
}
