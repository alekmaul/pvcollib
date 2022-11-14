VERSION = "0.1.0"

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CC=gcc
CFLAGS=-g -O2 -Wall -D__BUILD_DATE="\"`date +'%Y%m%d'`\"" -D__BUILD_VERSION="\"$(VERSION)\""

SOURCES=bin2txt.c
OBJS=bin2txt.o
EXE=bin2txt
DEFINES = 
LIBS = 

ifeq ($(OS),Windows_NT)
        EXT=.exe
else
        EXT=
endif

#---------------------------------------------------------------------------------
all: $(EXE)$(EXT)

#---------------------------------------------------------------------------------
$(EXE)$(EXT) : $(OBJS)
	@echo make exe $(notdir $<) 
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
bin2txt.o : bin2txt.c
	@echo make obj $(notdir $<) 
	$(CC) $(CFLAGS) -c $<

#---------------------------------------------------------------------------------
clean:
	rm -f $(OBJS) $(EXE)$(EXT) *.tds

install:
	mkdir -p ../../devkitcol/tools && cp $(EXE)$(EXT) ../../devkitcol/tools/$(EXE)$(EXT)

