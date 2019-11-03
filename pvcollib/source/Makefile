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

S_SRC	:=	console.s pad.s sound.s sprite.s video.s 

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
	@echo Making entry point ... $(notdir $<)
	$(AS) $(ASFLAGS) -o crtcol.rel $<

collib.lib: $(OFILES)
	@echo Making lib ... $(notdir $@)
	$(LK) collib.lib $(OFILES)

all: $(COLOBJS)
	@mv *.lib ../lib
	@mv crtcol.rel ../lib

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -f *.rel *.sym *.lst
	@rm -f $(TOPDIR)/lib/*
