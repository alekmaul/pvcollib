# path to colecodev root directory (for emulators, devkitcol, pvcollib)
export DEVKITCOL := /c/colecodev/
#export DEVKITCOL := /z/vboxshared/DropboxSvnClient/trunk/colecodev/pvcollib

# path to devkitcol root directory for compiler
export DEVKITSDCC := /c/colecodev/devkitcol
#export DEVKITSDCC := /z/vboxshared/DropboxSvnClient/trunk/colecodev/pvcollib/devkitcol

#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITCOL)),)
$(error "Please set DEVKITCOL in your environment. export DEVKITCOL=<path to>DEVKITCOL)
endif

export TOPDIR	:=	$(CURDIR)

export PVCOLLIB_MAJOR	:= 1
export PVCOLLIB_MINOR	:= 0
export PVCOLLIB_PATCH	:= 0


VERSION	:=	$(PVCOLLIB_MAJOR).$(PVCOLLIB_MINOR).$(PVCOLLIB_PATCH)


.PHONY: release clean all docs 

all: include/coleco/libversion.h release docs dist-bin

#-------------------------------------------------------------------------------
release: lib
#-------------------------------------------------------------------------------
	make -C source all

#-------------------------------------------------------------------------------
lib:
#-------------------------------------------------------------------------------
	mkdir lib

#-------------------------------------------------------------------------------
clean:
#-------------------------------------------------------------------------------
	make -C source clean

#-------------------------------------------------------------------------------
dist-src:
#-------------------------------------------------------------------------------
	@tar -cjf pvcollib-src-$(VERSION).tar.bz2 source include Makefile pvcollib_license.txt pvcollib.dox
	@mv *.tar.gz2 ..

#-------------------------------------------------------------------------------
dist-bin: 
#-------------------------------------------------------------------------------
	@tar -cjf pvcollib-$(VERSION).tar.bz2 include lib docs pvcollib_license.txt
	@tar -cjf devkitcol-$(VERSION).tar.bz2 ../devkitcol ../emulators/put_emulators_here.txt
	@tar -cjf colexamples-$(VERSION).tar.bz2 ../coleco-examples
	@mv *.tar.bz2 ..

dist: dist-bin dist-src

#-------------------------------------------------------------------------------
install: dist-bin
#-------------------------------------------------------------------------------
	mkdir -p $(DEVKITCOL)/PVCOLLIB
	bzip2 -cd pvcollib-$(VERSION).tar.bz2 | tar -x -C $(DEVKITCOL)/PVCOLLIB

#---------------------------------------------------------------------------------
docs:
#---------------------------------------------------------------------------------
	doxygen PVCOLLIB.dox

#---------------------------------------------------------------------------------
include/coleco/libversion.h : Makefile
#---------------------------------------------------------------------------------
	@echo "#ifndef __PVCOLLIBVERSION_H__" > $@
	@echo "#define __PVCOLLIBVERSION_H__" >> $@
	@echo >> $@
	@echo "#define _PVCOLLIB_MAJOR_	$(PVCOLLIB_MAJOR)" >> $@
	@echo "#define _PVCOLLIB_MINOR_	$(PVCOLLIB_MINOR)" >> $@
	@echo "#define _PVCOLLIB_PATCH_	$(PVCOLLIB_PATCH)" >> $@
	@echo >> $@
	@echo '#define _PVCOLLIB_STRING "PVCOLLIB Release '$(PVCOLLIB_MAJOR).$(PVCOLLIB_MINOR).$(PVCOLLIB_PATCH)'"' >> $@
	@echo >> $@
	@echo "#endif // __PVCOLLIBVERSION_H__" >> $@
