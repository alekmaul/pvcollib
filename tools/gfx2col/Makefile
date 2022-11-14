VERSION = "1.4.0"

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CC=gcc
CFLAGS=-g -O2 -Wall -D__BUILD_DATE="\"`date +'%Y%m%d'`\"" -D__BUILD_VERSION="\"$(VERSION)\""

SOURCES=gfx2col.c lodepng.c lodepic.c comptool.c comprle.c compdan1.c  compdan2.c  compdan3.c
OBJS=gfx2col.o  lodepng.o lodepic.o comptool.o comprle.o compdan1.o  compdan2.o  compdan3.o
EXE=gfx2col
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

gfx2col.o : gfx2col.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<

lodepng.o : lodepng.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<

lodepic.o : lodepic.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<

comptool.o : comptool.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<
	
#---------------------------------------------------------------------------------
clean:
	rm -f $(OBJS) $(EXE)$(EXT) *.tds

install:
	mkdir -p ../../devkitcol/tools && cp $(EXE)$(EXT) ../../devkitcol/tools/$(EXE)$(EXT)
