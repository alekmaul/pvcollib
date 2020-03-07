.SECONDARY:
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITCOL)),)
$(error "Please set DEVKITCOL in your environment. export DEVKITCOL=<path to>DEVKITCOL)
endif

include $(DEVKITSDCC)/col_rules

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
C_SRC	:=	

CFLAGS	:=	

S_SRC	:=	console.s console1.s console2.s console3.s console4.s \
	pad.s \
	sound0.s sound.s sound1.s sound2.s sound3.s \
	sprite.s sprite1.s sprite2.s sprite3.s sprite4.s sprite5.s sprite6.s sprite7.s \
	score.s score1.s score2.s score3.s score4.s \
	video.s video1.s video2.s video3.s video4.s video5.s video6.s video7.s video8.s video9.s \
	video10.s video11.s  video12.s video13.s video13_1.s video14.s video15.s video16.s video17.s video18.s video19.s \
	video20.s video21.s video22.s video23.s video24.s video25.s video26.s video27.s video28.s

ASFLAGS	:=	

INCLUDES	:=	. ../include 

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= 
 
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(DEVKITSDCC) 

export INCLUDE	:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					-I$(CURDIR)/$(BUILD)

CFLAGS	+=	$(INCLUDE)

export COLOBJS = collib.lib crtcol.rel
export OFILES	:=	$(C_SRC:.c=.rel) $(S_SRC:.s=.rel)


#---------------------------------------------------------------------------------
crtcol.rel: crtcol.s 
#	@echo Making entry point ... $(notdir $<)
	$(AS) $(ASFLAGS) -o crtcol.rel $<

collib.lib: $(OFILES)
#	@echo Making lib ... $(notdir $@)
	$(LK) collib.lib $(OFILES)

all: $(COLOBJS)
	@mv *.lib ../lib
	@mv crtcol.rel ../lib

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -f *.rel *.sym *.lst
	@rm -f $(TOPDIR)/lib/*
