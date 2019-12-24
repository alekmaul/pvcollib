# ColecoVision Specifications  
This is a **WIP**, I will completed it as long I'm working on the lib and EmulTwo emulator :)

About this Document  
====
**Colecovision Doc Version 1.0, last updated 12 December 2019 by Alekmaul**  
This document describes the I/O Map of Colecovision console, attempting to supply a very compact and mostly complete document about COLECO hardware. 

**Portar doc of MSX by Martin Korth for layout, sentences inspiration and MSX info too** http://problemkaputt.de/portar.htm  
**Datasheet for the TMS9918A and TMS9928A** http://www.cs.columbia.edu/~sedwards/papers/TMS9918.pdf  
**TMS9918 Programmer's Guide** http://www.madrigaldesign.it/creativemu/files/tms9918_guide.pdf  
**F18 support thread** https://atariage.com/forums/topic/207586-f18a-programming-info-and-resources/  
**"The new COLECOVISION FAQ" - Version 3.6** Copyright (c) 1994, 1995, 1996   Joseph M. Huber and James Carter.  
**"CV programming guide" - Version 1-k** Last update: October 26, 2005 Amy Purple.

**Copyright**  
This text may not be sold, or included in commercial software/hardware or firmware packages, or used or duplicated for other commercial purposes without the authors permission.  

I/O Port Summary
====
Most internal Coleco hardware is accessed by I/O instructions. In all cases only the lower 8 bits of the I/O addresses are relevant.  
### I/O Ports  
<pre>
80-9F (W) = Controls _ Set to keypad mode
80-9F (R) = Not Connected

A0-BF (W) = Video \___ A0 also decoded by video chip
A0-BF (R) = Video /

C0-DF (W) = Controls _ Set to joystick mode
C0-DF (R) = Not Connected

E0-FF (W) = Sound
E0-FF (R) = Controls _ A1 also decoded by chips  (A1=0 ctrl 1; A1=1 ctrl 2)
</pre>  

Memory
====
The CV uses 2 74138 3-8 line decoders to generate the memory and IO maps. As you would expect, memory is broken up into 8 8K blocks.  

### Memory Map
<pre>
0000-1FFF = BIOS ROM
2000-3FFF = Expansion port
4000-5FFF = Expansion port
6000-7FFF = RAM (1K mapped into an 8K spot)
8000-9FFF = Cart ROM 
A000-BFFF = Cart ROM 
C000-DFFF = Cart ROM      
E000-FFFF = Cart ROM 
</pre>

VDP I/O Ports
===
Port BE-BF are used by the internal VDP TMS9918a

### Internal VDP
<pre>
Port BE VRAM Data (Read/Write)  
Port BF VDP Status Registers (Read Only)  
Port BF 2nd Byte b7=0: VRAM Address setup (Write Only)  
Port BF 2nd Byte b7=1: VDP Register write (Write Only)  
</pre>
### External VDP F18A
to do  

Video Modes
===
This chapter describes the standard VRAM map for the different Colecovision video modes. Note that these default addresses can be changed by modifying VDP registers 2-6.

The VDP Video Modes is selected by the Bits M1-M3 of VDP Register 0 and 1. The relationship between the bits and the screen is:
<pre>
  M1 M2 M3 Screen format
  0  0  0  Half text  32x24             (Mode 0 - Graphic 1)
  1  0  0  Text       40x24             (Mode 1 - Text)
  0  0  1  Hi resolution 256x192        (Mode 2 - Graphic 2)
  0  1  0  Multicolour  4x4pix blocks   (Mode 3 - Multicolor)
</pre>
**Mode 0**: 256x192 pixels total, as 32x24 characters, pulled from 1 character set of 256 8x8 pixel characters. Each group of 8 characters in the character set has a 2-color limitation. For example, the characters "0" through "7" will all have the same color attributes.  
**Mode 1**: 240x192 pixels total, as 40x24 characters, pulled from 1 character set of 256 6x8 pixel characters. The entire character set has a 2-color limitation. This mode doesn't support sprites.  
**Mode 2**: 256x192 pixels total, as 32x24 characters, pulled from 3 character sets of 256 8x8 pixel characters. Each 8-pixel-wide line of a character in the character sets has a 2-color limitation. This mode provides a unique character for every character location on screen, allowing for the display of bitmapped images.  
**Mode 3**: 256x192 pixels total, 64x48 changeable virtual pixels, as 32x24 "semi-graphics" characters. These semi-graphics are defined in a special character set of 256 characters defined by 2x2 "fat-pixels". There are 4x4 pixels in each fat-pixel, but the pixels within a fat-pixel cannot be individually defined, although each fat-pixel can have its own color, hence the name of this mode (Multicolor). This mode is very blocky, and rarely used.  

Foreground Sprites
===
OBJ Attributes (Sprite attribute) define 'OAM' data for up to 32 foreground sprites.  

Each entry consists of four bytes:
<pre>
  0: Y-pos, Vertical position (FFh is topmost, 00h is second line, etc.)
  1: X-pos, Horizontal position (00h is leftmost)
  2: Pattern number
  3: Attributes. b0-3:Color, b4-6:unused, b7:EC (Early Clock)
</pre>
If EC is set to 1, the X-pos is subtracted by 32 (can be used to place sprites particulary offscreen to the left.

When using 16x16 pixel sprites the lower two bits of the sprite number are ignored (should be zero).  
A 16x16 sprite logically consists of four 8x8 sprites, whereas the first 8x8 sprite is displayed in upperleft, the second one in lower left, the third in upper right, and the fourth in lower right.

If Y-pos is set to 208 (D0h), the sprite AND ALL FOLLOWING sprites are hidden! 

This behavior is the same with F18A device, in locked mode (compatible with original VDP).  
In F18A unlocked mode, for all modes and ECM modes settings:
<pre>
  Bit  Name       Expl.
  0-3  PS3-0      Palette select per sprite (b0=PS3, b3=PS0)
  4    SIZE       Size: 0 = global VR1 setting, 1 = 16x16
  5    FLIPY      Sprite flip Y axis 
  6    FLIPX      Sprite flip X axis
  7    F          V-Blank IRQ Flag (1=interrupt) (See also IE0 flag)
</pre>

VRAM Data Read/Write  
===
Port BE VRAM Data (Read/Write)  
Port BF VRAM Address setup (2nd Byte b7=0) (Write Only)  

**Port BEh, Accessing VRAM Data**  
Read data from VRAM, or write data to VRAM. In either case the VRAM read/write pointer is automatically incremented, allowing to read/write a stream of bytes without having to setup the pointer each time.  
  
**Port BFh, VRAM Address Pointer Setup**  
The VRAM read/write pointer is initalized by writing TWO BYTES to port BEh with BIT 7 CLEARED in the second byte.  
<pre>
  Byte 1/Bit 0-7  Lower bits of VRAM Pointer
  Byte 2/Bit 0-5  Upper bits of VRAM Pointer
  Byte 2/Bit 6    Desired VRAM Direction (0=Reading, 1=Writing)
  Byte 2/Bit 7    Must be "0" for VRAM Pointer setup
</pre>
This 14bit Pointer value allows to address 16Kbytes of VRAM 

VDP Status Registers
===
Port BF VDP Status Registers (Read Only)

The Colecovision includes only one VDP Status Register (Register 0), for F18A device VDP Status Registers 0-Fh exist.

Status register 0 (default):  
<pre>
  Bit  Name       Expl.
  0-4  SP4-0      Number for the 5th sprite (9th in screen 4-8) on a line (b0=SP4, b4=SP0)
  5    C          1 if overlapping sprites
  6    5S         1 if more than 4 sprites on a horizontal line (8 in screen 4-8)
  7    F          V-Blank IRQ Flag (1=interrupt) (See also IE0 flag)
</pre>
In mode 1-3 only 4 sprites can be displayed per line. Bit 6 indicates if too many sprites have been (attempted to be) displayed. If the bit is set, Bit 0-4 indicate the number of the sprite that wasn't displayed properly. If more than one sprite haven't displayed properly, then Bit 0-4 specify the first of these bad sprites.  
A sprite is overlapping another if a non-transparent pixel of a sprite hits a non-transparent pixel of another sprite.  
The IRQ flag in bit 7 gets set at the beginning of the VBlank period, if IE0 in VDP Register 1 is set (or gets set at a later time, while the IRQ flag is still set) then an interrupt is generated.  
The IRQ flag (bit 7) and the collision flag (bit 5) get cleared after reading Status register 0.  

VDP Register Write
===
### VDP Registers 00h-07h: Basic Colecovision Video Registers

**Register 00: VR0**  
<pre>
  Bit  Name       Expl.
  0    EXTVID     External video input       (0=input disable, 1=enable)
  1-2  M3-4       Mode M4 M3 (M4 only for F18A)
  3    0          Not Used
  4    IE1        H-Blank Interrupt Enable   (F18A, see also VDP Reg 13h)
  5-7  0          Not Used
</pre>

**Register 01: VR1**  
<pre>
  Bit  Name       Expl.
  0    MAG        Sprite zoom                (0=x1, 1=x2)
  1    SZ         Sprite size                (0=8x8, 1=16x16)
  2    0          Not Used
  3-4  M1-2       Mode M2 M1
  5    IE0        V-Blank Interrupt Enable   (0=Disable, 1=Enable)
  6    BLK        Screen output control      (0=Disable, 1=Enable)
  7    4K16K      VRAM size control          (0=4K, 1=16K, ignored for F18A)
</pre>

The video modes are:  
<pre>
Mode=M4 M3 M2 M1  
GM1 = 0  0  0  0  
GM2 = 0  1  0  0  
MCM = 0  0  1  0  
T40 = 0  0  0  1 (40 col)  
T80 = 1  0  0  1 (80 col)  
</pre>
M4 is only available for F18A device.  

**Register 2: VR2 Name Table Base Address, 1K boundaries**  
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name    0    0    0    0   A00  A01  A02  A03
</pre>
768-bytes per table for 24 rows  
960-bytes per table for 30 rows  

**Register 3: VR3 Color Table Base Address, 64-byte boundaries**  
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name   A00  A01  A02  A03  A04  A05  A06  A07
</pre>
32-bytes long for original color mode (default)  
For the F18A enhanced color modes this becomes Tile Attribute Table Base Address  
The Tile Attribute Table is 256 bytes long  
768 to 4K when in position-attribute mode depending on the size of the horz / vert pages.  

**Register 4: VR4 Pattern Generator Base Address, 2K boundaries** 
<pre>  
  Bit     7    6    5    4    3    2    1    0
  Name    0    0    0    0    0   A00  A01  A02
</pre>
2K for 256 tiles  
4K for 2-bit color mode, 6K for 3-bit color mode  

**Register 5: VR5 Sprite Attribute Table Base Address, 128-byte boundaries**  
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name    0   A00  A01  A02  A03  A04  A05  A06
</pre>
128-bytes (32 sprites, 4 bytes each)  

**Register 6: VR6 Sprite Pattern Generator Base Address, 2K boundaries** 
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name    0    0    0    0    0   A00  A01  A02
</pre>
2K for 256 patterns  
4K for 2-bit color mode, 6K for 3-bit color mode  

**Register 7: VR7 Text color / background color**  
<pre>
  Bit  Name       Expl.
  0-3  BG0-3      Background colour 
  4-7  FG0-3      Foreground colour
</pre>
The bits 0-3 and 4-7 can hold a number in the range of 0-15.  

The corresponding colours are:  
<pre>
  0 = Transparent         8 = Medium red
  1 = Black               9 = Light red
  2 = Medium green        10= Dark yellow
  3 = Light green         11= Light yellow
  4 = Dark blue           12= Dark green
  5 = Light blue          13= Magenta
  6 = Dark red            14= Gray
  7 = Cyan                15= White
</pre>

### VDP Registers 0Ah-39h: Additional VR10-VR57 F18A Video Registers  

**Register 0A: VR10 Name Table 2 Base Address, 1K boundaries** 
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name    0    0    0    0   A00  A01  A02  A03
</pre>

768-bytes per table for 24 rows  
960-bytes per table for 30 rows  

**Register 0B: VR11 Color Table 2 Base Address, 64-byte boundaries**
<pre>
  Bit     7    6    5    4    3    2    1    0
  Name   A00  A01  A02  A03  A04  A05  A06  A07
</pre>

Works the same as VR3 in Enhanced Color Modes / Position-Attribute Mode  

**Register 18: VR24 Extra palette-select bits for original color modes**  
<pre>
  Bit  Name       Expl.
  0-1  T1PS01-1   Tile 1 layer palette selector
  2-3  T2PS0-1    Tile 2 layer palette selector
  4-6  SPS0-1     Sprite palette selector
  6-7  0          Not used
</pre>

**Register 2F: VR47**  
<pre>
  Bit  Name       Expl.
  0-5  PR0-5      Palette register #
  6    AUTO INC   Auto inc palette index after each write
  7    DPM        Data-port mode, 1 to enable
</pre>

**Register 30: VR48 SIGNED two's-complement increment amount for VRAM address, defaults to 1**  
<pre>
  Bit  Name       Expl.
  0-6  INC0-6     Value
  7    SIGN-BIT   Signed bit  
</pre>

**Register 31: VR49** 
<pre>
  Bit  Name       Expl.
  0-1  ECMS0-1    ECM (S)prite
  2    0          Not Used
  3    Y_REAL     Real sprite Y-coord
  4-5  ECMT0-1    ECM (T)ile
  6    ROW30      24 / 30 tile rows (30 rows disables 0xD0 in SAL)
  7    TILE2_EN   Tile Map 2 enable, 1 to enable
</pre>

ECM = enhanced color mode for (T)ile and (S)prite  
<pre>
  0=nomal/original color
  1=1-bit color mode, tile attributes
  2=2-bit color mode, tile attributes
  3=3-bit color mode, tile attributes
</pre>

**Register 32: VR50**  
<pre>
  Bit  Name       Expl.
  0    TILE2_PRI  0 = TL2 always on top, 1 = TL2 vs sprite priority is considered
  1    POS_ATTR   Position vs name attributes		0 = per name attributes in ECMs, 1 = per position attributes
  2    SCANLINES  Show virtual scan lines			0 = no virtual scan lines, 1 = enable virtual scan lines
  3    RPTMAX     Report sprite max vs 5th sprite				0 = report 5th sprite, 1 = report max sprite (VR30)	
  4    TL1_OFF    0 = normal, 1 = disable GM1, GM2, MCM, T40, T80    	
  5    GPU_VTRIG  0 = disable, 1 = trigger GPU on VSYNC	
  6    GPU_HTRIG  0 = disable, 1 = trigger GPU on HSYNC    	
  7    VR_RESET   0 = ignored, 1 = reset all VDP registers to power-on defaults
</pre>

**Register 36: VR54**  
<pre>
  Bit  Name       Expl.
  0-7  GPU0-7     GPU address MSB
</pre>

**Register 37: VR55**  
<pre>
  Bit  Name       Expl.
  0-7  GPU8-15    GPU address LSB, writing resets and loads, then triggers the GPU
</pre>

**Register 38: VR56**  
<pre>
  Bit  Name       Expl.
  0-6  0          Not used
  7    GPUOP      GPU Operation performed when register is written: 0 = reset and load PC, 1 = trigger GPU to begin execution
</pre>

**Register 39: VR57**
<pre>
  Bit  Name       Expl.
  0-7  VALUE      Unlock: Enable registers over 7, write 000111xx to VR57 twice
</pre>

F18A Tile attribute byte in Enhanced Color Modes  
===
Color Table becomes Tile Attribute Table Base Address and goes from 32 to 256 or 2K (same as name table) bytes long.  <pre>
  Bit  Name       Expl.
  0-3  PS3-0      Palette selection (b3=PS0, b0=PS3)
  4    TRANS      When 1, colors with 0, 00 or 000 will be transparent instead of a palette index
  5    FLIPY      Tile flip over Y
  6    FLIPX      Tile flip over X
  7    PRI        Tile priority over sprite
</pre>

When T40/T80 Mode, ECM0, and Position-Based Attributes is Enabled									
<pre>
  Bit     7    6   5   4   3   2   1   0
  Name   FG0	FG1	FG2	FG3	BG0	BG1	BG2	BG3	T40/80 fg / bg color
</pre>


Colecovision Models  
===
**ColecoVision:**  
<pre>
             Resolution: 256 x 192
                    CPU: Z-80A
                   Bits: 8
                  Speed: 3.58 MHz
                    RAM: 8K
	      Video RAM: 16K (8x4116)
Video Display Processor: Texas Instruments TMS9928A
                Sprites: 32
                 Colors: 16
                  Sound: Texas Instruments SN76489AN; 3 tone channels, 1 noise
          Cartridge ROM: 8K/16K/24K/32K
</pre>

**ADAM:**  
<pre>
             Resolution: 256 x 192
                    CPU: Z-80A
                   Bits: 8
                  Speed: 3.58 MHz
	    Video Speed: 10.7 MHz
                    RAM: 64K (128K optional)
	      Video RAM: 16K (8x4116)
                    ROM: 8K
Video Display Processor: Texas Instruments TMS9928A
                Sprites: 32
                 Colors: 16
                  Sound: Texas Instruments SN76489AN; 3 tone channels, 1 noise
          Cartridge ROM: 8K/16K/24K/32K
	    Disk Drives: 2 * 160K (opt)
    Digital Data Drives: 2 * 256K
		  Modem: 300 Baud (opt)
	        Printer: 120 wpm Daisy Wheel, 16K buffer
		  Other: Serial/Parallel Port (opt), Auto Dialer (opt)
</pre>

JUMP functions   
===
JUMP TABLE  from Colecovion BIOS.  
Legend:
P (at the end): function specifically done for Pascal programs.  
<pre>
1F61 > 0300 : PLAY_SONGS
1F64 > 0488 : ACTIVATEP
1F67 > 06C7 : PUTOBJP
1F6A > 1D5A : REFLECT_VERTICAL
1F6D > 1D60 : REFLECT_HORIZONTAL
1F70 > 1D66 : ROTATE_90
1F73 > 1D6C : ENLARGE
1F76 > 114A : CONTROLLER_SCAN
1F79 > 118B : DECODER
1F7C > 1979 : GAME_OPT
1F7F > 1927 : LOAD_ASCII
1F82 > 18D4 : FILL_VRAM
1F85 > 18E9 : MODE_1
1F88 > 116A : UPDATE_SPINNER
1F8B > 1B0E : INIT_TABLEP
1F8E > 1B8C : GET_VRAMP
1F91 > 1C10 : PUT_VRAMP
1F94 > 1C5A : INIT_SPR_ORDERP
1F97 > 1C76 : WR_SPR_NM_TBLP
1F9A > 0F9A : INIT_TIMERP
1F9D > 0FB8 : FREE_SIGNALP
1FA0 > 1044 : REQUEST_SIGNALP
1FA3 > 10BF : TEST_SIGNALP
1FA6 > 1CBC : WRITE_REGISTERP
1FA9 > 1CED : WRITE_VRAMP
1FAC > 1D2A : READ_VRAMP
1FAF > 0655 : INIT_WRITERP
1FB2 > 0203 : SOUND_INITP
1FB5 > 0251 : PLAY_ITP
1FB8 > 1B08 : INIT_TABLE
1FBB > 1BA3 : GET_VRAM
1FBE > 1C27 : PUT_VRAM
1FC1 > 1C66 : INIT_SPR_ORDER
1FC4 > 1C82 : WR_SPR_NM_TBL
1FC7 > 0FAA : INIT_TIMER
1FCA > 0FC4 : FREE_SIGNAL
1FCD > 1053 : REQUEST_SIGNAL
1FD0 > 10CB : TEST_SIGNAL
1FD3 > 0F37 : TIME_MGR
1FD6 > 023B : TURN_OFF_SOUND
1FD9 > 1CCA : WRITE_REGISTER
1FDC > 1D57 : READ_REGISTER
1FDF > 1D01 : WRITE_VRAM
1FE2 > 1D3E : READ_VRAM
1FE5 > 0664 : INIT_WRITER
1FE8 > 0679 : WRITER
1FEB > 11C1 : POLLER
1FEE > 0213 : SOUND_INIT
1FF1 > 025E : PLAY_IT
1FF4 > 027F : SOUND_MAN
1FF7 > 04A3 : ACTIVATE
1FFA > 06D8 : PUTOBJ
1FFD > 003B : RAND_GEN 
</pre>

ROM Header  
===
All carts start at 8000h with a header that tells the BIOS what to do.  
<pre>
8000 - 8001:   If bytes are AAh and 55h, the CV will show a title screen
               and game name, etc.
               If bytes are 55h and AAh, the CV will jump directly to the
               start of code vector.

8002 - 8003:   Pointer to RAM copy of the sprite name table 
8004 - 8005:   Pointer to RAM sprite table
8006 - 8007:   Pointer to free buffer space in RAM
8008 - 8009:   Pointer to controller memory map
800A - 800B:   Pointer to start of code
800C - 800E:   Jmp to: RST 08h
800F - 8011:   Jmp to: RST 10h
8012 - 8014:   Jmp to: RST 18h
8015 - 8017:   Jmp to: RST 20h
8018 - 801A:   Jmp to: RST 28h
801B - 801D:   Jmp to: RST 30h
801E - 8020:   Jmp to: RST 38h
8021 - 8023:   JMP to: NMI (Vertical Blanking Interrupt from video chip)
8024 - nnnn:   String with two delemiters "/" as "LINE2/LINE1/YEAR"
</pre>

Data for the title screen is composed of 4 lines in the format:  
<pre>
+--------------+
| COLECOVISION |
|              |
|    LINE 2    |
|    LINE 3    |
|(c)xxxx COLECO|
+--------------+
</pre>

**Typical Screen**  
The '**ColecoVision**' line cannot be changed, as well as the '(C)xxxx Coleco' part of the bottom line.  Only the xxxx part can be changed.
The data is stored as one string with the '/' character (2Fh) used as a delimiter.  It signals the end of a line, and isn't printed.  
The lines are stored out of order like so:  
"LINE 3/LINE 2/xxxx"  There isn't an end-of-line delimiter, because the last line is always 4 characters (it's meant for a year like 1983)

So, if we want to see the following:  
<pre>
+--------------+
| COLECOVISION |
|              |
|   MY GAME!   |
|    BY: ME    |
|(c)1995 COLECO|
+--------------+
</pre>

We would use the string:  
<pre>
"BY: ME!/MY GAME!/1995"
</pre>
Remember, we cannot change the "(c)xxxx COLECO" part, only the xxxx in the middle of the line.
The lines are self-centering on the screen.  
Altho the BIOS ROM has both upper-case and lower-case characters in the character set, only upper-case is supported.  

All printable characters are based on the ASCII character set:  
(all values in hex)  
<pre>
00-1C:     Blank
1D   :     (c)    (Copyright symbol)
1E-1F:     tm     (TradeMark symbol, uses 2 chars side-by-side)
20-2E: (respectively)  space ! " # $ % & ' ( ) * + , - .
2F   :     Delimiter- used to end a line, not printable
30-39:     0-9
3A-40: (respectively)  : ; < = > ? @
41-5A:     A-Z
5B-5F: (respectively)  [ \ ] ^ _
</pre>
The chars # 60-8F are the 4-char blocks that make up the '**COLECOVISION**' name at the top, arranged like so:  
<pre>
         6062 6466 686A 6C6E 7072 7476 787A 7C7E 8082 8486 888A 8C8E
         6163 6567 696B 6D6F 7173 7577 797B 7D7F 8183 8587 898B 8D8F

          C    O    L    E    C    O    V    I    S    I    O    N
         (purple)  (orange)   (pink)   (yellow)   (green)   (blue)
</pre>

What's intresting, is when these are in the title lines, they show up in their respective colours!  All other printable chars are white.  
Chars 90-FF are all blank

Cartridge Memory Mappers  
===
**Magecart from Bryan Edewaard** (created in August 2005)  
Megacart EPROMs are broken up into 16k pages, and are banked by accessing the last 64 addresses of the cartridge space (0xFFC0 - 0xFFFF).  
Accessing 0xFFFF maps the top 16k of the EPROM (or page 0), 0xFFFE maps the second-to-last 16k (or page 1), 0xFFFD maps the third-to-last 16k (or page 2), etc.  
Megacarts are limited to a maximum size of 1MB.

The first 16K of the cartridge (CV 8000h-BFFFh) is fixed and will always contain the highest/last 16K segment of the EPROM.  
Therefore, this data will appear twice (CV 8000h-BFFFh and C000h-FFFFh) if the last bank is selected.

It is recommended that the following addresses be read to select banks:  
<pre>
128K -  8 banks - FFF8h-FFFFh.
256K - 16 banks - FFF0h-FFFFh.
512K - 32 banks - FFE0h-FFFFh.
 1MB - 64 banks - FFC0h-FFFFh.
</pre>
Remember that strobing FFFFh places the same data in both halves of the cartridge memory



# END  
