SUBDIRS:= $(shell find . -type d | sed '1d')

#---------------------------------------------------------------------------------
all: examples 
	@rm -fr bin
	@mkdir -p bin
	find . -name "*.rom" -exec cp -fv {} bin \;

examples:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make -C $$i || { exit 1;} fi; done;

#---------------------------------------------------------------------------------
clean:
	@rm -fr bin
	@rm -f *.bz2
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then make -C $$i clean || { exit 1;} fi; done;

