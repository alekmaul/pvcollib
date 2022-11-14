VERSION = "1.0.0"

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
#CC=/d/MinGW32/bin/gcc
CC=gcc
CFLAGS=-g -O2 -Wall -D__BUILD_DATE="\"`date +'%Y%m%d'`\"" -D__BUILD_VERSION="\"$(VERSION)\""

SOURCES=cvmkcart.c
OBJS=cvmkcart.o
EXE=cvmkcart
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
	
cvmkcart.o : cvmkcart.c
	@echo make obj $(notdir $<) 
	$(CC) $(CFLAGS) -c $<

#---------------------------------------------------------------------------------
clean:
	rm -f $(OBJS) $(EXE)$(EXT) *.tds

install:
	mkdir -p ../../devkitcol/tools && cp $(EXE)$(EXT) ../../devkitcol/tools/$(EXE)$(EXT)

