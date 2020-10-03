;---------------------------------------------------------------------------------
;
;	Copyright (C) 2018-2020
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
;   Updated by Amy Purple for optimization
;	Definition:
;	To start writing at a destination (DE) offset in VRAM.
;	Note:
;	A further optimization might be to make this routine a software interupt in the cart header
;
;---------------------------------------------------------------------------------
	.module pvclvideo3_1

	; global from external entries / code

	; global from this module
	.globl vdpwrite

	.area  _CODE

;---------------------------------------------------------------------------------
; Here begin routines that can't be call from programs
;---------------------------------------------------------------------------------
vdpwrite:
    ld      c,#0xbf
    out     (c),e
    set     6,d
    out     (c),d
	ret
