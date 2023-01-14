ifeq ($(strip $(PVCOLLIB_HOME)),)
$(error "Please create an environment variable PVCOLLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVCOLLIB_HOME "/c/colecodev">)")
endif

include ${PVCOLLIB_HOME}/devkitcol/col_rules

#---------------------------------------------------------------------------------
# ROMNAME is used in col_rules file
ROMNAME		:=	megacart

#---------------------------------------------------------------------------------
# each file must be export with correct bank sorting
export OFILES	:= megacart.rel \
	gfxsb1.rel gfxsb2.rel gfxsb3.rel
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# BANKs - all defined the same way, we just need to declare them
BANKS = -Wl-b_bank1=0xc000 -Wl-b_bank2=0xc000 -Wl-b_bank3=0xc000 
LDFLAGS += $(BANKS)

#---------------------------------------------------------------------------------
# For  64K Megacart start on $fffc upto SLOT_4 $ffff 
# For 128K Megacart start on $fff8 upto SLOT_7 $ffff
# for 256K one I start on $fff0 upto SLOT_15 at $ffff

.PHONY: bitmaps all
 
#---------------------------------------------------------------------------------
all: bitmaps $(ROMNAME).rom
	@echo Linking Megarom ...
	@cp $(ROMNAME).map crtcol.map 
	$(LDMEGA) -icrtcol.ihx -b6 $(ROMNAME).rom

clean: cleanRom cleanGfx

#---------------------------------------------------------------------------------
image1gfx.inc: image1.png
	@echo convert graphic 1 ... $(notdir $@)
	$(GFXCONV) -cdan1 -fpng -b -m $<

image2gfx.inc: image2.png
	@echo convert graphic 2 ... $(notdir $@)
	$(GFXCONV) -cdan1 -fpng -b -m $<

image3gfx.inc: image3.png
	@echo convert graphic 3 ... $(notdir $@)
	$(GFXCONV) -cdan1 -fpng -b -m $<
	
bitmaps: image1gfx.inc image2gfx.inc image3gfx.inc

#---------------------------------------------------------------------------------
gfxsb1.rel: gfxsb1.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank1 gfxsb1.c

gfxsb2.rel: gfxsb2.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank2 gfxsb2.c

gfxsb3.rel: gfxsb3.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank3 gfxsb3.c

