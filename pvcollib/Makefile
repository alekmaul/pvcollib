ifeq ($(strip $(PVCOLLIB_HOME)),)
$(error "Please create an environment variable PVCOLLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVCOLLIB_HOME "/c/snesdev">)")
endif

export TOPDIR	:=	$(CURDIR)

# create version number which will be used everywhere
export PVCOLLIB_MAJOR	:= 1
export PVCOLLIB_MINOR	:= 6
export PVCOLLIB_PATCH	:= 0

# Directory with docs config and output (via doxygen)
export PVCDOCSDIR = $(TOPDIR)/docs

.PHONY: release clean all docs

all: include/coleco/libversion.h pvcollibversion release docs

#-------------------------------------------------------------------------------
release: lib
	make -C source all

#-------------------------------------------------------------------------------
lib:
	mkdir lib

#-------------------------------------------------------------------------------
examples:
	cd ../coleco-examples && \
	make && \
	make install

#-------------------------------------------------------------------------------
clean:
	make -C source clean
	rm -rf $(PVCDOCSDIR)/html
	rm -f pvcollib_version.txt

#---------------------------------------------------------------------------------
doxygenInstalled := $(shell command -v doxygen 2> /dev/null)
docs:
ifndef doxygenInstalled
	@echo "doxygen is not installed, documentation will be not generated.";
else
	@rm -rf $(PVCDOCSDIR)/html; \
	PVCDOCSDIR="$(PVCDOCSDIR)" PVCVERSION="$(PVCOLLIB_MAJOR).$(PVCOLLIB_MINOR).$(PVCOLLIB_PATCH)" doxygen "$(PVCDOCSDIR)/pvcollib.dox"
	@if [ "$(DOCS_PDF_ON)" = "YES" ]; then\
		$(MAKE) -C $(PVCDOCSDIR)/latex;\
		cp $(PVCDOCSDIR)/latex/refman.pdf $(PVCDOCSDIR)/PVCOLLIB_manual.pdf;\
	fi
	@if [ -f 'warn.log' ]; then \
			@cat warn.log; \
	fi	
	@rm -rf $(PVCDOCSDIR)/latex
endif

# Turn on Latex -> PDF conversion to run run at end of regular docs build
# (which includes latex output but deletes it at the end).
#
# The conversion process requires a Latex install.
# For Windows there are various Latex packages to choose from.
# For Linux this appears to be the minimum:
#   sudo apt install texlive-latex-base
#   sudo apt install texlive-latex-recommended
#   sudo apt install texlive-latex-extra
#
docspdf: DOCS_PDF_ON=YES
docspdf: docs

#---------------------------------------------------------------------------------
include/coleco/libversion.h : Makefile
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

# to ease installation for users, the version is no more in folder name (which require to update PVCOLLIB_HOME) but in specific file
pvcollibversion:
	@echo $(PVCOLLIB_MAJOR).$(PVCOLLIB_MINOR).$(PVCOLLIB_PATCH) > pvcollib_version.txt