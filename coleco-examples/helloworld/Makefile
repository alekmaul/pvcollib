# path to colecodev root directory (for emulators, devkitcol, pvcollib)
export DEVKITCOL := /c/colecodev/
#export DEVKITCOL := /z/vboxshared/DropboxSvnClient/trunk/colecodev/pvcollib/pvcollib

# path to devkitcol root directory for compiler
export DEVKITSDCC := /c/colecodev/devkitcol
#export DEVKITSDCC := /z/vboxshared/DropboxSvnClient/trunk/colecodev/pvcollib/devkitcol

# change global path with SGDK
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
TARGET		:=	helloworld
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
# Name of text file containing build number.
BUILD_NUMBER_FILE=buildnumber.txt
BUILD_INCLUD_FILE=buildnum.h
BUILD_MAIN_FILE=main.c
 
#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(SFILES:.s=.rel) $(CFILES:.c=.rel) 
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

.PHONY: all
 
#---------------------------------------------------------------------------------
all	:	$(OUTPUT).rom
	@echo Generate rom ... $(notdir $@)
	
clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).rom *.rel *.map *.lst *.lk *.ihx *.noi *.sym *.asm

#---------------------------------------------------------------------------------


#---------------------------------------------------------------------------------
$(OUTPUT).rom	: $(OFILES)
