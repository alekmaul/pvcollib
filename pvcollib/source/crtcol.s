;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2019
;		Alekmaul 
;
;	This software is provided 'as-is', without any express or implied
;	warranty.  In no event will the authors be held liable for any
;	damages arising from the use of this software.
;
;	Permission is granted to anyone to use this software for any
;	purpose, including commercial applications, and to alter it and
;	redistribute it freely, subject to the following restrictions:
;
;	1.	The origin of this software must not be misrepresented; you
;		must not claim that you wrote the original software. If you use
;		this software in a product, an acknowledgment in the product
;		documentation would be appreciated but is not required.
;	2.	Altered source versions must be plainly marked as such, and
;		must not be misrepresented as being the original software.
;	3.	This notice may not be removed or altered from any source
;		distribution.
;
;---------------------------------------------------------------------------------

	; this module is completly based on Amy Purple (aka newcoleco) crt0.s module

	.module crtcol

  ; external sound table setting routine
	.globl snd_settable
	.globl _mus_stopplay
	.globl _mus_update

	; global from this code
	.globl snd_areas
	.globl _buffer32
	.globl _no_nmi
	.globl _vdp_status
	.globl _nmi_flag
	.globl _joypad_1
	.globl _keypad_1
	.globl _joypad_2
	.globl _keypad_2

	.globl _spinner_enabled
	.globl _spinner_1
	.globl _spinner_2

	.globl _vid_freq
	.globl _vid_frsw
	
	.globl _snd_mute
	
	.globl _mus_pointer
	.globl _mus_counter

	; global from C code
	.globl _main
	.globl _nmi

	.globl l__INITIALIZER
	.globl s__INITIALIZER
	.globl s__INITIALIZED

	;; Ordering of segments for the linker - copied from sdcc crt0.s
	.area _HOME
	.area _CODE
	.area _INITIALIZER
	.area _GSINIT
	.area _GSFINAL
        
	.area _DATA
	.area _INITIALIZED
	.area _BSEG
	.area _BSS
	.area _HEAP

	;; TABLE OF VARIABLES (IN RAM)
	.area	_DATA
_buffer32::
	.ds	32 ; buffer space 32    [7000-701F]
snd_addr::
	.ds	11 ; sound addresses    [7020-702A]
snd_areas::
	.ds	61 ; 6 sound slots + NULL (00h) [702B-...]
_no_nmi::
	.ds    1
_vdp_status::
	.ds    1
_nmi_flag::
	.ds    1
_joypad_1::
	.ds    1
_keypad_1::
	.ds    1
_joypad_2::
	.ds    1
_keypad_2::
	.ds    1	
_spinner_enabled::
	.ds	1
_vid_freq::
	.ds 1
_vid_frsw::
	.ds 1
_snd_mute::
	.ds 1
_mus_pointer:
    .ds 2
_mus_counter:
    .ds 2
	
_spinner_1 = 0x73eb
_spinner_2 = 0x73ec

	; CARTRIDGE HEADER (IN ROM), From Daniel Bienvenu post 
	; http://atariage.com/forums/topic/168314-coleco-cartridge-header-from-official-documentation/
	; $8000 : CARTRIDGE
	;       game type = bytes AA 55 , display CV logo title screen
	;       test type = bytes 55 AA , execute your game no delay
	;       otherwise it is invalid and display "INSERT CARTRIDGE"
	; $8002 : LOCAL_SPR_TBL
	;        it is the memory address where is your sprites table
	;        for their coordinates, patterns and colors.
	; $8004 : SPRITE_ORDER
	;        it is a table of usually maximum 32 bytes
	;        that simply say in which order each sprite entry
	;        should display. The corresponding initialisation function
	;        set the values as 0,1,2,3,4... which mean that the first
	;        entry (entry 0) is first, then the second entry (entry 1)
	;        is second to be disaplayed. By reordering these numbers, you
	;        affect the order of sprites, allowing to do flickering if needed.
	; $8006 : WORK_BUFFER
	;        it is usually a big memory RAM space that can be used by
  ;        some BIOS functions to calculate graphics manipulations
  ;        like mobile and semi-mobile objects, tiles rotations, etc.
  ; $8008 : CONTROLLER_MAP
  ;        it contains first 2 bytes telling if port-1 and port-2 are
  ;        enabled to be analyzed with the POLLER function. The following
  ;        values correspond to different fire buttons, stick direction,
  ;        keypad and spinner values.
  ; $800A : START_GAME
  ;        it is the address of the starting point of you code, your main code entry.
  ; $800C - $801B : RST_8H_RAM - RST_30H_RAM
  ;        Are usually jumps to sub-programs in the cartridge or to BIOS functions.  
  ; $801E : IRQ_INT_VECT
  ;        It should be a jump to a sub-routine saving all the PAIRs
  ;        (AF, BC, DE, HL, AF', BC' DE' and HL') and then jump to BIOS
  ;        to count spinner value $1F88 -or- something similar made by
  ;        the programmer. To avoid an interuption within an interuption
  ;        it is highly recommanded to use DI and EI opcodes to encapsulate
  ;        this part of the code. "IM 1" should be done as soon as possible
  ;        in game program, like the first thing at the main game entry point.
  ; $8021 : NMI_INT_VECT
  ;        It is the crucial part of all ColecoVision games. If you are not
  ;        using the CV logo screen, you can directly code your sequence
  ;        of actions to do while a non maskable interrupt (NMI) occurs.
  ;        To be safe, you should save all the regsiter PAIRs before
  ;        executing anything in the NMI function.
  ;        And most important, you should read the video status
  ;        Otherwise there will be no other non maskable interupt.
  ; $8024 : GAME_NAME
  ;        It is a string that contains first the name of the game
  ;        with usually the trademark symbol (characters 1E and 1F),
  ;        a slash as a separator, then the usual PRESENTS with  the original licensor,
  ;        a slash separator again, and finally the release year in 4 digits.

	.area _HEADER(ABS)
	.org 0x8000
	
	.db	0x55, 0xaa			; no default colecovision title screen => 55 AA
	.dw	0								; no copy of sprite table, etc.
	.dw	0								; all unused
	.dw	_buffer32				; work buffer
	.dw	0								; controller map
	.dw	start_program		; start address for game coding
	.db	0xc9,0,0				; no RST 08 support
	.db	0xc9,0,0				; no RST 10 support
	.db	0xc9,0,0				; no RST 18 support
	.db	0xc9,0,0				; no RST 20 support
	.db	0xc9,0,0				; no RST 28 support
	.db	0xc9,0,0				; no RST 30 support
	jp  _int_spinner            ; RST38 - spinner interrupt
	jp	_int_nmi

	;; CODE STARTS HERE WITH NMI
        .area _CODE

;; This is the real interrupt-driven entry point
_int_nmi:
        push	af
        ld	    a,	#1
        ld      (_nmi_flag),a           ; set NMI flag

        call    0x1fdc                  ; get VDP status
        ld      (_vdp_status),a

        ld      a,(_no_nmi)             ; check if nmi() should be
        or      a                       ;  called
        jp      nz,nmi_exit
        inc     a
        ld      (_no_nmi),a
        
        ld      a,(_vid_frsw) ; update flag for frequency
        inc	    a
        ld		(_vid_frsw),a
                
        push    bc
        push    de
        push    hl
        push    ix
        push    iy
        ex      af,af'
        push    af
        exx
        push    bc
        push    de
        push    hl
        call    0x1f76                   ; update controllers
        ld      a,(0x73ee)
        and		#0x4f
        ld      (_joypad_1),a
        ld      a,(0x73ef)
        and		#0x4f
        ld      (_joypad_2),a
        ld      a,(0x73f0)
        and		#0x4f
        ld      (_keypad_1),a
        ld      a,(0x73f1)
        and		#0x4f
        ld      (_keypad_2),a
        call    decode_controllers
        call    _nmi                    ; call C function
		ld		a,(_snd_mute)
        and		#0x01
        jr 		nz, $1101

		call _mus_update				; play music if needed

        call    0x1f61                   ; play sounds
        call    0x1ff4                   ; update snd_addr with snd_areas
        
        ld      a,(_vid_freq)           ; if 50Hz, setting redo music for sagaruo
        sub 	#0x32
        jr 		nz, $1101
        ld      a,(_vid_frsw)           ; only recall if 1 cycle per 3
        and		#0x03
        jr      nz, $1101
        call    0x1f61                   ; play sounds
        call    0x1ff4                   ; update snd_addr with snd_areas

$1101:
        pop     hl
        pop     de
        pop     bc
        exx
        pop     af
        ex      af,af'
        pop     iy
        pop     ix
        pop     hl
        pop     de
        pop     bc
		
        xor     a
        ld      (_no_nmi),a

nmi_exit:
        ld	    a,(_spinner_enabled)
        or	    a
        jr	    z,nmi_end
        ei
nmi_end:
        pop     af
        ret

keypad_table::
	.db    0xff,8,4,5,0xff,7,11,2,0xff,10,0,9,3,1,6,0xff

; joypads will be decoded as follows:
; bit
; 0     left
; 1     down
; 2     right
; 3     up
; 4     button 4
; 5     button 3
; 6     button 2
; 7     button 1
; keypads will hold key pressed (0-11), or 0xff
decode_controllers:
	ld      ix, #_joypad_1
	call    decode_controller
	inc     ix
	inc     ix
decode_controller:
	ld      a,0(ix)
	ld      b,a
	and     #0x40
	rlca
	ld      c,a
	ld      a,b
	and     #0x0f
	or      c
	ld      b,a
	ld      a,1(ix)
	ld      c,a
	and     #0x40
	or      b
;;;;;
;;	ld      0(ix),a
;;;;;
	ld      b,a
	ld      a,c
	cpl
	and    #0x0f
	cp      #8
	jr      nz,no_button_3
	ex      af,af'
	ld      a,b
	or      #0x20
	ld      b,a
	ex      af,af'
no_button_3:
	cp      #4
	jr      nz,no_button_4
	ex      af,af'
	ld      a,b
	or      #0x10
	ld      b,a
	ex      af,af'
no_button_4:
	ld      0(ix),b
;;;;
	ld      a,c
	cpl
	and    #0x0f
	ld      e,a
	ld      d,#0
	ld      hl,#keypad_table
	add     hl,de
	ld      a,(hl)
	ld      1(ix),a
	ret

_int_spinner:
	push    af
	push    hl
	call    0x1f88
	pop     hl
	pop     af
	ei
	reti
	
start_program:
	im       1                      ; interrupt mode -> rst 38h
	di

	xor     a                       ; clear carry
	ld      bc,#0x3b8				; ram size left
	ld      hl,#0x7000				; starting from 7000
	ld      de,#0x7001
	ld      (hl),a
	ldir                            ; zero-fill bss

	ld	    a,#1                    ; this is after we zero fill, should still be early
	ld      (_no_nmi),a             ; don't process NMI during setup (tursi)


	call gsinit						; Initialize global variables.

	ld	h,#0 						; set dummy sound table
	call snd_settable

	ld      hl,#0x0033              ; initialise random generator
	ld      (0x73c8),hl
                                    ; set screen mode 2 text
	call    0x1f85                  ; set default VDP regs 16K
	ld      de,#0x4000              ; clear VRAM
	xor     a
	ld      l,a
	ld      h,a
	call    0x1f82
	
	ld 		a, (0x0069)				; initialise video frequency
	ld      (_vid_freq),a

	call 	_mus_stopplay			; initialise music entry

  ; re-enable NMIs (tursi)
	xor     a
	ld      (_no_nmi),a
	ld      (_snd_mute),a

	in      a,(#0xbf)               ; clear VDP status 	
	ld      (_vdp_status),a         ; and save it
	
	jp      _main					; call main rountine
	
	.area _GSINIT
gsinit::
	ld      bc, #l__INITIALIZER
	ld	a,b
	or	a,c
	jr	z, gsinit_next
	ld      de, #s__INITIALIZED
        ld      hl, #s__INITIALIZER

	ldir
gsinit_next:


	.area _GSFINAL
	ret
	;
