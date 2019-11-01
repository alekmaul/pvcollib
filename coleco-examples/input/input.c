/*---------------------------------------------------------------------------------


	Pad demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "input.h"

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
	
	// Wait for nothing :P
	while(1) {
		// Update display with current pad 1 values
		if (joypad_1 & PAD_RIGHT) vdp_putstring(9,10,"RIGHT PRESSED"); 
		if (joypad_1 & PAD_LEFT)  vdp_putstring(9,10,"LEFT PRESSED "); 
		if (joypad_1 & PAD_DOWN)  vdp_putstring(9,10,"DOWN PRESSED "); 
		if (joypad_1 & PAD_UP)    vdp_putstring(9,10,"UP PRESSED   "); 
		if (joypad_1 & PAD_FIRE1) vdp_putstring(9,10,"FIRE1 PRESSED"); 
		if (joypad_1 & PAD_FIRE2) vdp_putstring(9,10,"FIRE2 PRESSED"); 
		if (joypad_1 & PAD_FIRE3) vdp_putstring(9,10,"FIRE3 PRESSED"); 
		if (joypad_1 & PAD_FIRE4) vdp_putstring(9,10,"FIRE4 PRESSED"); 

		// Update display with current keyboard pad 1 values
		if (keypad_1 == PAD_KEY0)     vdp_putstring(9,12,"KEY 0 PRESSED"); 
		if (keypad_1 == PAD_KEY1)     vdp_putstring(9,12,"KEY 1 PRESSED"); 
		if (keypad_1 == PAD_KEY2)     vdp_putstring(9,12,"KEY 2 PRESSED"); 
		if (keypad_1 == PAD_KEY3)     vdp_putstring(9,12,"KEY 3 PRESSED"); 
		if (keypad_1 == PAD_KEY4)     vdp_putstring(9,12,"KEY 4 PRESSED"); 
		if (keypad_1 == PAD_KEY5)     vdp_putstring(9,12,"KEY 5 PRESSED"); 
		if (keypad_1 == PAD_KEY6)     vdp_putstring(9,12,"KEY 6 PRESSED"); 
		if (keypad_1 == PAD_KEY7)     vdp_putstring(9,12,"KEY 7 PRESSED"); 
		if (keypad_1 == PAD_KEY8)     vdp_putstring(9,12,"KEY 8 PRESSED"); 
		if (keypad_1 == PAD_KEY9)     vdp_putstring(9,12,"KEY 9 PRESSED"); 
		if (keypad_1 == PAD_KEYSTAR)  vdp_putstring(9,12,"STAR PRESSED "); 
		if (keypad_1 == PAD_KEYSHARP) vdp_putstring(9,12,"SHARP PRESSED"); 

		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
