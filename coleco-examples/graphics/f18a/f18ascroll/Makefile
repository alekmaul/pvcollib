ifeq ($(strip $(PVCOLLIB_HOME)),)
$(error "Please create an environment variable PVCOLLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVCOLLIB_HOME "/c/colecodev">)")
endif

include ${PVCOLLIB_HOME}/devkitcol/col_rules

#---------------------------------------------------------------------------------
# ROMNAME is used in col_rules file
ROMNAME		:=	f18ascroll

.PHONY: bitmaps all
 
 #---------------------------------------------------------------------------------
all: bitmaps $(ROMNAME).rom
	
clean: cleanRom cleanGfx

#---------------------------------------------------------------------------------
ourvisiongfx.inc: ourvision.png
	@echo convert graphic ... $(notdir $@)
	$(GFXCONV) -cple -fpng -e3 -m $<
	
bitmaps: ourvisiongfx.inc
