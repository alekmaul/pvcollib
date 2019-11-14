#export PATH	:=	/c/msys/bin:$(PATH)
export PATH := /c/MinGW32/bin/:/c/msys/bin:$(PATH)

VERSION = "1.1.0"

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CC=gcc
CFLAGS=-g -O2 -Wall -D__BUILD_DATE="\"`date +'%Y%m%d'`\"" -D__BUILD_VERSION="\"$(VERSION)\""

SOURCES=gfx2col.c lodepng.c comptool.c
OBJS=gfx2col.o  lodepng.o comptool.o
EXE=gfx2col.exe
DEFINES =
LIBS =
#---------------------------------------------------------------------------------
all: $(EXE)

#---------------------------------------------------------------------------------
$(EXE) : $(OBJS)
	@echo make exe $(notdir $<)
	$(CC) $(CFLAGS) $(OBJS) -o $@

gfx2col.o : gfx2col.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<

lodepng.o : lodepng.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<

comptool.o : comptool.c
	@echo make obj $(notdir $<)
	$(CC) $(CFLAGS) -c $<
	
#---------------------------------------------------------------------------------
clean:
	rm -f *.o *.exe *.tds


