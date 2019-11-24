/*---------------------------------------------------------------------------------


	scoring functions demo
	-- alekmaul


---------------------------------------------------------------------------------*/
#include "scoring.h"

score_t scoretst,scoretst1;

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
	vdp_fillvram(0x2000,0xf1,128*8); // Change color (or we will see nothing)
	vdp_enablescr();
	
	// Clear the score
	scoretst.hi=18;scoretst.lo=900;
	sys_scoclear(&scoretst);
	vdp_putstring(2,2,"score hi xxxxx score lo xxxx");
	vdp_putstring(11,2,sys_str(scoretst.hi));vdp_putstring(26,2,sys_str(scoretst.lo));
	
	// Add a value
	sys_scoadd(&scoretst,0x4DB);
	vdp_putstring(2,4,"score hi xxxxx score lo xxxx");
	vdp_putstring(11,4,sys_str(scoretst.hi));vdp_putstring(26,4,sys_str(scoretst.lo));

	// Add more than 10000
	sys_scoadd(&scoretst,0x2710);
	vdp_putstring(2,6,"score hi xxxxx score lo xxxx");
	vdp_putstring(11,6,sys_str(scoretst.hi));vdp_putstring(26,6,sys_str(scoretst.lo));
	vdp_putstring(11,7,sys_scostr(&scoretst,8));

	// Compare two scores
	scoretst.hi=18;scoretst.lo=900;
	scoretst1.hi=17;scoretst1.lo=900;
	if (sys_scocmp(&scoretst,&scoretst1)==0xFF) vdp_putstring(2,12,"1 scores equals");
	else if (sys_scocmp(&scoretst,&scoretst1)==0)  vdp_putstring(2,12,"1 scoretst higher");
	else if (sys_scocmp(&scoretst,&scoretst1)==1)  vdp_putstring(2,12,"1 scoretst lower");
	
	scoretst1.hi=18;scoretst1.lo=901;
	if (sys_scocmp(&scoretst,&scoretst1)==0xFF)  vdp_putstring(2,13,"2 scores equals");
	else if (sys_scocmp(&scoretst,&scoretst1)==0)  vdp_putstring(2,13,"2 scoretst higher");
	else if (sys_scocmp(&scoretst,&scoretst1)==1)  vdp_putstring(2,13,"2 scoretst lower");
	
	scoretst1.hi=19;scoretst1.lo=900;
	if (sys_scocmp(&scoretst,&scoretst)==0xFF)  vdp_putstring(2,14,"3 scores equals");
	else if (sys_scocmp(&scoretst,&scoretst1)==0)  vdp_putstring(2,14,"3 scoretst higher");
	else if (sys_scocmp(&scoretst,&scoretst1)==1)  vdp_putstring(2,14,"3 scoretst lower");

	// Wait for nothing :P
	while(1) {
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
