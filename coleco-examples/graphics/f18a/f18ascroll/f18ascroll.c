/*---------------------------------------------------------------------------------


	f18a test with a bitmap 
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "f18ascroll.h"

#include "gfxs.h"									// to add definition of graphics

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for bitmap example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	u8 yscr,xscr;
	
	// Check if f18a is available
	vdp_f18ainit();

	// Disable screen
	vdp_disablescr();

	// display a message regarding result of test
	vdp_f18aok=1;
	if (vdp_f18aok==1) {
		vdp_setreg(50,16);//turn off tile layer

		// put screen in mode 1 with no specific f18a stuffs
		vdp_f18asetmode1(0x30);
		
		vdp_ple2vram (TILP1ourvisiongfx, chrgen); 		   // characters Layer 1
		vdp_ple2vram (TILP2ourvisiongfx, chrgen+256*8);   // characters Layer 2
		vdp_ple2vram (TILP3ourvisiongfx, chrgen+256*8*2); // characters Layer 2
		vdp_ple2vram (COLourvisiongfx, coltab); 		   // colours  (attribute table in our ecm3 case)
		vdp_ple2vram (MAPourvisiongfx, chrtab);           // map
		vdp_f18asetpalette(PALourvisiongfx,64*2);
		
		vdp_enablescr();
		vdp_setreg(50,0);//turn on tile layer
	}
	else {
		// Put screen in text mode 2
		vdp_setmode2txt();
	
		// Put default char in VRAM and duplicate to all areas
		//  as we are going to write to line 8, it is in the second area
		vdp_setdefaultchar(FNTNORMAL);
		vdp_duplicatevram();
		vdp_fillvram(0x2000,0xf1,128*8); 	// Change color (or we will see nothing)
		vdp_enablescr();
	
		// Print text as we are not going to do something else
		vdp_putstring(8,5,"NO F18A SUPPORT...");
	}

	// Wait for nothing :P
	xscr=0; yscr=0;
	while(1) {
		// only for F18a
		if (vdp_f18aok==1) {
			// test keyboard
			if (joypad_1 & PAD_DOWN)  { yscr--; while (joypad_1 & PAD_DOWN) { vdp_waitvblank(1); } }
			if (joypad_1 & PAD_UP)    { yscr++; while (joypad_1 & PAD_UP) { vdp_waitvblank(1); } }
			if (joypad_1 & PAD_LEFT)  { xscr--; while (joypad_1 & PAD_LEFT) { vdp_waitvblank(1); } }
			if (joypad_1 & PAD_RIGHT) { xscr++; while (joypad_1 & PAD_RIGHT) { vdp_waitvblank(1); } }
			
			// check boundaries for y scroll
			if (yscr<0) { if (yscr>=-64) yscr=-191; }
			if (yscr>=192) yscr=0; 
			
			//scroll in x & y
			vdp_f18asetscrollx(1, xscr);
			vdp_f18asetscrolly(1, yscr);
		}

		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
