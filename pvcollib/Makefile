ifeq ($(strip $(PVCOLLIB_HOME)),)
$(error "Please create an environment variable PVCOLLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVCOLLIB_HOME "/c/colecodev">)")
endif

export TOPDIR := $(CURDIR)

# create version number which will be used everywhere
export PVCOLLIB_MAJOR	:= 1
export PVCOLLIB_MINOR	:= 6
export PVCOLLIB_PATCH	:= 0
export PVCOLLIB_VERSION := $(PVCOLLIB_MAJOR).$(PVCOLLIB_MINOR).$(PVCOLLIB_PATCH)

# Directory with docs config and output (via doxygen)
export PVCDOCSDIR = $(TOPDIR)/docs

all: include/coleco/libversion.h pvcollibversion release docs

KEEP_LIB := 0

release: lib
	make -C source all

build:
	@$(MAKE) -C source all
    
lib:
	@mkdir -p $@

clean:
	$(MAKE) -C source clean
	@if [ "$(KEEP_LIB)" -eq 0 ]; then \
		rm -rf $(PVCDOCSDIR)/html; \
		rm -f pvcollib_version.txt; \
		rm -rf lib; \
		fi

# Check if Doxygen is installed
doxygenInstalled := $(shell command -v doxygen -q 2> /dev/null)

# The default docs target. Here we build the doxygen documentation if doxygen is installed and pdf documentation if latexn is installed.
docs:
ifndef doxygenInstalled
# If Doxygen is not installed, print an error message
	@echo "Doxygen is not installed, documentation will not be generated."
else
# Remove the existing HTML documentation
	@rm -rf $(PVCDOCSDIR)/html
# Set the PVCDOCSDIR variable and run Doxygen
	@PVCDOCSDIR=$(PVCDOCSDIR) doxygen "$(PVCDOCSDIR)/pvcollib.dox"

# If PDF output is enabled, build the LaTeX documentation and copy the resulting PDF
	@if [ "$(DOCS_PDF_ON)" = "YES" ]; then \
		$(MAKE) -C $(PVCDOCSDIR)/latex && cp $(PVCDOCSDIR)/latex/refman.pdf $(PVCDOCSDIR)/pvcollib_manual.pdf;\
	fi
# If there are warnings, print them to the console
	@if [ -f 'warn.log' ]; then \
		cat warn.log; \
	fi
# Remove the LaTeX documentation
	@rm -rf $(PVCDOCSDIR)/latex
endif

# The docs PDF target, which sets the DOCS_PDF_ON variable and runs the docs target
docspdf: DOCS_PDF_ON=YES
docspdf: docs

# This rule generates a header file that contains the version information of the library.
# It defines three variables, _PVCOLLIB_MAJOR_, _PVCOLLIB_MINOR_, and _PVCOLLIB_PATCH_ that
# correspond to the major, minor, and patch versions respectively.
# It also defines a variable _PVCOLLIB_STRING_ that contains a string representation of the
# version information.
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

# This target generates a text file containing the current version number of PVColLib. This allows for easier installation for users, as the version is no longer included in the folder name (which previously required users to update the 'pvcollib_home' variable). Instead, the version can now be found in this specific file.
pvcollibversion:
	@echo $(PVCOLLIB_VERSION) > pvcollib_version.txt

# Declare the phony targets
.PHONY: release clean all docs
