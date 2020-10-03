/*---------------------------------------------------------------------------------


	animated sprite demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "animatedsprite.h"

#include "gfxs.h"									// to add definition of graphics

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
	// update sprite display during nmi, if you enable sprites
	if (spr_enable)
		spr_update();
}

//---------------------------------------------------------------------------------
void main (void) {
	u8 xp,yp,idpac,tpo;
	u16 pattern;
	
	// Put screen in text mode 2
	vdp_disablescr();
	
	vdp_setmode2txt();
	
	// Put default char in VRAM and duplicate to all areas
	//  as we are going to write to line 10, it is in the second area
	vdp_setdefaultchar(FNTNORMAL);
	vdp_duplicatevram();
	vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)

	// Put sprite character
	vdp_putvram (sprtab,TILpacspritegfx,SZTILpacspritegfx); // sprite characters

    // Enable screen and nmi to allow display and sprite management
    vdp_enablescr();
	vdp_enablenmi();
	
	// Put sprite in middle of screen with color light yellow, 0 because we have only one pattern (index 0)
	xp=8*16;yp=8*12;pattern=0;
	idpac=spr_getentry();
	spr_set(idpac,xp,yp,11,pattern);
	tpo=0;
	
	// enable sprite display
	spr_enable = 1;
	
	// Wait for nothing :P
	while(1) {
		if (joypad_1 & PAD_RIGHT) { 
			if (xp<(256-16)) { xp++; spr_setx(idpac,xp); } 
		}
		if (joypad_1 & PAD_LEFT)  { 
			if (xp>0) { xp--; spr_setx(idpac,xp); } 
		}
		if (joypad_1 & PAD_DOWN)  { 
			if (yp<(192-16)) {  yp++; spr_sety(idpac,yp); } 
		}
		if (joypad_1 & PAD_UP)    { 
			if (yp>0) { yp--; spr_sety(idpac,yp); } 
		}
		// manage sprite pattern with little tempo
		tpo++;
		if (tpo>3) {
			tpo=0;
			pattern+=4;
			if (pattern>4*3) pattern=0;
			spr_setpat(idpac,pattern);
		}

		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
