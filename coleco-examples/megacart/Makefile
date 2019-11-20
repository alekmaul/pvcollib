# path to colecodev root directory (for emulators, devkitcol, pvcollib)
export DEVKITCOL := /c/colecodev/

# path to devkitcol root directory for compiler
export DEVKITSDCC := /c/colecodev/devkitcol

# change global path with PVcollib
export PATH	:=	$(DEVKITSDCC):$(PATH)

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITSDCC)),)
$(error "Please set DEVKITSDCC in your environment. export DEVKITSDCC=<path to>DEVKITSDCC")
endif

include $(DEVKITSDCC)/col_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	megacart
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	
LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
 
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
all	: bitmaps $(OUTPUT).rom
	@echo Generate rom ... $(notdir $@)
	
%.rom:
	@echo Linking ... $(notdir $@)
	$(LD) $(LDFLAGS) $(LIBOBJS) $(OFILES)
	$(LDMEGA) -icrtcol.ihx -b6 $(TARGET).rom
	mv crtcol.map $(TARGET).map

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).rom *.rel *.map *.lst *.lk *.ihx *.noi *.sym *.asm  \
		image1gfx.inc image1gfx.h image2gfx.inc image2gfx.h image3gfx.inc image3gfx.h

#---------------------------------------------------------------------------------
image1gfx.inc: image1.png
	@echo convert graphic 1 ... $(notdir $@)
	$(GFXCONV) -cdan -fpng -b -m $<

image2gfx.inc: image2.png
	@echo convert graphic 2 ... $(notdir $@)
	$(GFXCONV) -cdan -fpng -b -m $<

image3gfx.inc: image3.png
	@echo convert graphic 3 ... $(notdir $@)
	$(GFXCONV) -cdan -fpng -b -m $<
	
bitmaps: image1gfx.inc image2gfx.inc image3gfx.inc

#---------------------------------------------------------------------------------
gfxsb1.rel: gfxsb1.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank1 gfxsb1.c

gfxsb2.rel: gfxsb2.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank2 gfxsb2.c

gfxsb3.rel: gfxsb3.c 
	$(CC) $(CFLAGS) -c -mz80 --vc --no-std-crt0 --constseg bank3 gfxsb3.c
	
#---------------------------------------------------------------------------------
$(OUTPUT).rom	: $(OFILES)
