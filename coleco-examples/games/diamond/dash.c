/*---------------------------------------------------------------------------------


	small grame from coder Amy Purple : Diamond Dash
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "dash.h"

#include "gfxs.h"											// to add definition of graphics
#include "snds.h"											// to add definition of sounds

const signed char holes1_dx[]={-1,0,1,-2,-1,0,1,2,-2,-1,0,1,0};	// holes type #1
const signed char holes1_dy[]={-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,1};

const signed char holes2_dx[]={-2,-1,0,-2,-1,0,1,-2,-1,0,1,0,1};	// holes type #2
const signed char holes2_dy[]={-2,-2,-2,-1,-1,-1,-1,0,0,0,0,1,1};

const char *const holes_dx[]={holes1_dx,holes2_dx};			// holes type table
const char *const holes_dy[]={holes1_dy,holes2_dy};
const char holes_size = 13;									// holes size


const u8 game_over[] = {33,34,35,36,37,38,0};				// Char array for "GAME OVER"


int dynamite; 												// Number of dynamite 
unsigned diamant;											// Score 
unsigned timer;												// Timer

const unsigned int diamant_score[] = {1,3,5};				// Diamonds value
u8 nombre_diamants;											// Number of diamonds needed to finish the level 

const u8 minimum_diamants[]={85,75,60,45,35};

typedef struct {											// Classic coordinate structure
	char x;
	char y;
	signed char dx;
	signed char dy;
} coorxy;

coorxy player; 												// Player : coordinate
coorxy one_dynamite;										// Dynamite : coordinate
u8 dynamite_countdown;										// Dynamite : countdown before explosion

//---------------------------------------------------------------------------------
// Initialise characters graphics in VideoRAM 
void init_graphics(void) {
	// Load characters pattern and color
	vdp_rle2vram(PATTERNRLE,0x0000);
	vdp_duplicatevram();
	vdp_rle2vram(COLORRLE,0x2000);
	
	// Set sprites pattern as characters pattern
	vdp_setreg(6,0);
}

//---------------------------------------------------------------------------------
// Show score, timer and number of dynamites
void show_score(void) {
	vdp_putstring(6,23,sys_str(diamant));
	vdp_putstring(14,23,sys_str(timer));
	vdp_putstring(23,23,sys_str(dynamite));
}

//---------------------------------------------------------------------------------
// Add diamonds function
void add_diamond(u8 nombre, u8 diamond_type) {
	char x,y,k;
	signed char dx,dy;
loop2:
	x = sys_randbyte(1,30);
	y = sys_randbyte(4,21);
	if (vdp_getchar(x,y)!=1) goto loop2;
	
	for (dx=-1;dx<2;dx++) {
		for (dy=-1;dy<2;dy++) {
			k = vdp_getchar(x+dx,y+dy);
			if (k==2) goto loop2;
		}
	}
	vdp_putchar(x,y,2+diamond_type);
	nombre--;
	if (nombre!=0) goto loop2;
}

//---------------------------------------------------------------------------------
// Add holes function
void add_holes(u8 nombre) {
	char x,y,k,type;
	char dx,dy,size;

loop3:
	x = sys_randbyte(3,28);
	y = sys_randbyte(8,21);
	type = sys_random()&1;
	size=holes_size;
	while(size--!=0) {
		dx=(holes_dx[type])[size];
		dy=(holes_dy[type])[size];
		k = vdp_getchar(x+dx,y+dy);
		if (k!=1) goto loop3;
	}
	size=holes_size;
	while(size--!=0) {
		dx=(holes_dx[type])[size];
		dy=(holes_dy[type])[size];
		vdp_putchar(x+dx,y+dy,2);
	}
	nombre--;
	// add dynamite pack in the last two holes
	if (nombre==2) vdp_putchar(x,y,7);
	if (nombre<2) vdp_putchar(x,y,31);
	// loop
	if (nombre!=0) goto loop3;
}

//---------------------------------------------------------------------------------
// Draw a new mountain 
void new_mountain(void) {
	// Blank screen
	vdp_disablevdp();										
	vdp_rle2vram(NAMERLE,0x1800);							// Load default mountain screen
	
	// Initialise player's coordinate and sprite pattern
	player.x = 22;	
	player.y = 2;
	player.dx = 0;
	player.dy = 0;
	spr_setpat(0,64);
	spr_setcol(0,15);
	
	add_holes(6);											// Add 6 holes
	add_diamond(51,1);										// Add 51 green diamonds	
	add_diamond(23,2);										// Add 23 red diamonds
	add_diamond(11,3); 										// Add 11 white diamonds 
	
	// Show screen 
	vdp_enablevdp();
}

//---------------------------------------------------------------------------------
// Show sprite 
void show_sprite(void) {
	// wait 3 vertical retrace to slowdown the execution 
	vdp_enablenmi();
	vdp_waitvblank(3);
	vdp_disablenmi();
	// Put sprites table directly in VideoRAM 
	vdp_putvram(sprgen,sprites,8);
}

//---------------------------------------------------------------------------------
// Update player on screen
void show_player(u8 n) {
	u8 x,y;
	u8 j;
	
	// convert player's x and y position for normal sprite's position
	x = player.x<<3;
	y = (player.y<<3)-1;
	
	// Switch player's sprite pattern
	sprites[0].pattern ^=1;
	
	// If it's during game 
	if (n==0) {
		// Then animate sprites player movement
		for (j=0;j<2;j++) {
			if (sprites[0].x<x) sprites[0].x +=4;
			if (sprites[0].x>x) sprites[0].x -=4;
			if (sprites[0].y<y) sprites[0].y +=4;
			if (sprites[0].y>y) sprites[0].y -=4;
			show_sprite();
		}
	}
	else {
		// Otherwise, simply set sprites table with player's coordinate
		sprites[0].x=x;
		sprites[0].y=y;
		show_sprite();
	}
}

char move_player(void) {
	u8 j,k;
	char x,y;
	
	// Animate player
	show_player(0);
	
	// Put a "cavern" character on player
	vdp_putchar(player.x, player.y,2);
	
	// Get joystick in port#1 
	j = joypad_1;
	
	// Reset player's direction x and y 
	player.dx = 0;
	player.dy = 0;
	
	// Set new player's direction x and y 
	if (j & PAD_UP) player.dy=-1;
	if (j & PAD_DOWN) player.dy=1;
	if (j & PAD_LEFT) player.dx=-1;
	if (j & PAD_RIGHT) player.dx=1;
	
	// If player doesn't move then set 2nd player's sprite pattern, otherwise, set 1st player's sprite pattern 
	if ((player.dx==0) && (player.dy==0)) sprites[0].pattern |= 2; else sprites[0].pattern &= 0xfd; 
	
	// If player press fire 
	if ( (j & PAD_FIRE1) && (dynamite_countdown==0)) {
		// Set new countdown and dynamite coordinate 
		dynamite_countdown=10;
		one_dynamite.x=player.x;
		one_dynamite.y=player.y;
		// Decrease number of player's dynamite 
		dynamite--;
		// If player had no dynamite then return "stop game" flag 
		if (dynamite<0) return 0;
		
		// Play sound#4 : "schhhhhh" 
		snd_startplay(4);
	}
	
	// Check if there is something in front of the player 
	x = player.x + player.dx;
	y = player.y + player.dy;
	k = vdp_getchar(x,y);
	if ((k>1 && k<7)|| k==31) {
		// Update player's coordinate 
		player.x = x; player.y=y;
		// If it's a diamond, increase score and play sound #1 : "beep" 
		if (k>2 && k<6) {
			diamant += diamant_score[k-3]; snd_startplay(1);
			if (--nombre_diamants==0) {
				vdp_putchar(23,2,6); snd_startplay(5); snd_startplay(6);
			} 
		}
		// If it's a dynamite pack, increase number of player's dynamite by 3 and play sound #2 
		if (k==31) {
			dynamite += 3; snd_startplay(2);
		}
		// If it's the exit, return "stop game" flag 
		if (k==6) {
			return 0;
		}
	}
	// Return "continue game" flag 
	return -1;
}

// Do a square hole 
void square_hole(u8 x, u8 y, u8 size) {
	u8 i,j,k;
	char dx=0,dy=0;
	char n = size >> 1;

	// Play sound #2 : "bonk" 
	snd_startplay(2);

	// Add rocks in mountain 
	vdp_enablenmi();
	for (k=0;k<5;k++) {
		while (vdp_getchar(dx,dy)!=2) {  
			dx = sys_randbyte(0,31);
			dy = sys_randbyte(2,22);
		}
		vdp_disablenmi();
		vdp_putchar(dx,dy,1);
		vdp_enablenmi();
	}
	vdp_disablenmi();

	dx = 0;
	// Erase whatever it was directly in the center of this futur hole 
	vdp_putchar(x,y,2);
	// Do square hole 
	x -= n;
	y -= n;
	for (i=0;i<size;i++) {
		for (j=0;j<size;j++) {
			k = vdp_getchar(x,y);
			// If detect a barrel of powder, keep note of its coordinate 
			if (k==7) {dx=x; dy=y;}
			if (k==1) vdp_putchar(x,y,2);
			y++;
		}
		x++;
		y-=size;
	}
	/* If a barrel of powder was detected, do a big square hole */
	if (dx!=0) square_hole(dx, dy, 7);
}

// Update dynamite countdown and effect 
void update_dynamite(void) {
	// If a dynamite is on screen 
	if (dynamite_countdown!=0) {
		// Decrease countdown counter 
		dynamite_countdown--;

		// Update dynamite animation on screen 
		if (dynamite_countdown&1) {
			vdp_putchar(one_dynamite.x,one_dynamite.y,29);
		}
		else {
			vdp_putchar(one_dynamite.x,one_dynamite.y,30);
		}
		// If dynamite countdown = 0, then do a small square hole 
		if (dynamite_countdown==0) {
			square_hole(one_dynamite.x,one_dynamite.y,3);
		}
	}
}

// Game engine 
void game(void) {
	u8 mountains_limit = 5;
	dynamite=0; diamant=0;
	
	while(1) {
		dynamite = 20;
		new_mountain();
		
		// Decrease number of mountain to do 
		mountains_limit--;
		
		// Set number of diamonds needed to exit level 
		nombre_diamants = minimum_diamants[mountains_limit];
		timer=2000;
		show_player(1);

		// Game loop
		while(move_player()) {
			if (timer!=0) timer--;
			show_score();
			update_dynamite();
		}

		// Animate player for the last time 
		show_player(0);

		// "Game Over" when player had no more dynamite 
		if (dynamite<0)	{
			snd_startplay(3); 
			vdp_putstring(13,11,game_over);
			vdp_enablenmi();
			sys_pause();
			vdp_disablenmi();
		}
		else {
			// If there still a timer bonus 
			while (timer>9) {
				// Update timer and score 
				timer-=10;
				diamant++;
				// Play sound#1 : "beep" 
				snd_startplay(1);
				// Update score and timer on screen 
				show_score();
				// Wait a short time to slowdown the execution and to let the "beep" sound playing a bit 
				vdp_enablenmi();
				vdp_waitvblank(2);
				vdp_disablenmi();
			} 

			// If there still a timer bonus 
			while (dynamite--!=0) {
				// Update score
				diamant+=5;
				// Play sound#1 : "bonk" 
				snd_startplay(2);
				// Update score and timer on screen 
				show_score();
				// Wait a short time to slowdown the execution and to let the "beep" sound playing a bit 
				vdp_enablenmi();
				vdp_waitvblank(12);
				vdp_disablenmi();
			}

			// Play winner music 
			snd_startplay(5);
			snd_startplay(6); 

			// Wait until music ends 
			vdp_enablenmi();
			vdp_waitvblank(75);
			vdp_disablenmi(); 

			// Go to next mountain : if it's the last mountain, redo the last one 
			if (mountains_limit==0) mountains_limit++;
		}
	}
}
