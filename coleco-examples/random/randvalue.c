/*---------------------------------------------------------------------------------


	random value demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "randvalue.h"

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for Input example
void nmi (void) {
}

//---------------------------------------------------------------------------------
void main (void) {
	// Put screen in text mode 2
	vdp_setmode2txt();
	
	// Put default char in VRAM and duplicate to all areas
	//  as we are going to write to line 10, it is in the second area
	vdp_setdefaultchar(FNTNORMAL);
	vdp_duplicatevram();
	vdp_enablescr();
	
	// Change color (or we will see nothing)
	vdp_fillvram(0x2000,0xf1,128*8);
	
	// Print at 10,text for the random number
	vdp_putstring(4,10,"RANDOM NUMBER=");
	
	// Wait for nothing :P
	while(1) {
		// Update display with random number and wait pad 1
		utoa(random(),buffer32);
		vdp_putstring(18,10,buffer32);
		
		// Wait Fire and then Vblank
		while ((joypad_1 & PAD_FIRE1)==0) { vdp_waitvblank(1); }
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
