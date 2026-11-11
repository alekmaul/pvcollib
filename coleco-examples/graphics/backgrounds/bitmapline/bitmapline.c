/*---------------------------------------------------------------------------------


	Bitmap mode 2 demo : Bresenham lines, tiles and pixels
	-- uses the bitmap2.s module (cv_linebmp, cv_putarea, cv_setpixel ...)


---------------------------------------------------------------------------------*/
#include "bitmapline.h"

//---------------------------------------------------------------------------------
// 16x16 smiley = 2x2 tiles, 8 bytes per tile, tiles stored row by row
const u8 smileypat[32] = {
	// tile 0
	0x01,0x0F,0x18,0x30,0x66,0x46,0x46,0xC0,
	// tile 1
	0x80,0xF0,0x18,0x0C,0x66,0x62,0x62,0x03,
	// tile 2
	0xC0,0x48,0x44,0x63,0x30,0x18,0x0F,0x01,
	// tile 3
	0x03,0x12,0x22,0xC6,0x0C,0x18,0xF0,0x80,
};

// one color byte (foreground<<4 | background) per line of each tile : light yellow on black
const u8 smileycol[32] = {
	0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,
	0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,
	0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,
	0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1,0xB1
};

//---------------------------------------------------------------------------------
// The NMI routine. Gets called 50 or 60 times per second 
// nothing to update for this example
void nmi (void) {
}

//---------------------------------------------------------------------------------
// Draw one line, then wait a frame to see the drawing animation.
// The NMI status read would break a VDP address setup in progress, so we
// disable it during the drawing (see bitmap2.h)
void drawline(u8 x0, u8 y0, u8 x1, u8 y1) {
	vdp_disablenmi();
	vdp_linebmp(x0, y0, x1, y1);
	vdp_enablenmi();
	vdp_waitvblank(1);
}

//---------------------------------------------------------------------------------
void main (void) {
	u8 i;

	// Put screen in bitmap mode 2 : patterns at 0x0000, colors at 0x2000
	vdp_disablescr();
	vdp_setmode2bmp();

	// All patterns to 0 and white on black everywhere, then change colors of the
	//  2nd (light red) and 3rd (cyan) screen thirds
	vdp_fillvram(0x0000, 0x00, 0x1800);  // efface les motifs (3 tiers)
	vdp_fillvram(0x2000, 0xF1, 0x800);   // couleurs du 1er tiers (blanc sur noir)
	vdp_fillvram(0x2800, 0x91, 0x800);   // 2e tiers
	vdp_fillvram(0x3000, 0x71, 0x800);   // 3e tiers
	vdp_enablescr();

	// A frame around the screen
	drawline(0,   0,   255, 0);
	drawline(255, 0,   255, 191);
	drawline(255, 191, 0,   191);
	drawline(0,   191, 0,   0);

	// String art : left edge -> bottom edge, one line per frame
	for (i = 0; i < 24; i++) {
		drawline(0, i * 8, i * 11, 191);
	}

	// Both diagonals
	drawline(0, 0, 255, 191);
	drawline(255, 0, 0, 191);

	// Wait for nothing :P
	while(1) {
		// Wait Vblank
		vdp_waitvblank(1);
	}
	// startup module will issue a soft reboot if it comes here 
}
