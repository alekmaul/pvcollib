#export PATH	:=	/c/msys/bin:$(PATH)

export PATH := /c/MinGW32/bin/:/c/msys/bin:$(PATH)
#export PATH := /h/MinGW32/bin/:/e/ndsdev/msys/bin:$(PATH)

VERSION = "1.0.0"

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
#CC=/d/MinGW32/bin/gcc
CC=gcc
CFLAGS=-g -O2 -Wall -D__BUILD_DATE="\"`date +'%Y%m%d'`\"" -D__BUILD_VERSION="\"$(VERSION)\""

SOURCES=cvmkcart.c
OBJS=cvmkcart.o
EXE=cvmkcart.exe
DEFINES = 
LIBS = 
#---------------------------------------------------------------------------------
all: $(EXE)

#---------------------------------------------------------------------------------
$(EXE) : $(OBJS)
	@echo make exe $(notdir $<) 
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
cvmkcart.o : cvmkcart.c
	@echo make obj $(notdir $<) 
	$(CC) $(CFLAGS) -c $<

#---------------------------------------------------------------------------------
clean:
	rm -f *.o *.exe *.tds

