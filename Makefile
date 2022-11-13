# do not forget that to build pvcollib, you have to install compiler and tools first !

#---------------------------------------------------------------------------------
all:
	cd tools && \
	make && \
	make install && \
	cd ../pvcollib && \
	make && \
	cd ../coleco-examples && \
	make && \
	make install && \
	echo && \
	echo Build finished successfully ! && \
	echo

clean:
	cd tools ; \
	make clean ; \
	cd ../pvcollib ; \
	make clean ; \
	cd ../devkitcol ; \
	rm -f ./tools/* ; \
	cd ../coleco-examples ; \
	make clean ; \
	rm -rf release

# requirements to launch the "release" rule
doxygenInstalled := $(shell command -v doxygen 2> /dev/null)
operatingSystem=
ifeq ($(OS),Windows_NT)
operatingSystem=windows
else
# only for linux platforms, we use the os-release file
ifneq (,$(wildcard /etc/os-release))
include /etc/os-release
# convert to lower case the result
operatingSystem=linux_$(shell echo $(NAME) | tr A-Z a-z)
else
ifeq ($(shell uname -s),Darwin)
operatingSystem=darwin
else
$(error "Unable to detect your operating system, please update the code in global pvcollib Makefile to continue")
endif
endif
endif

#---------------------------------------------------------------------------------	
# to create the release version for github containing binaries and all coleco-examples :
release: all
ifndef doxygenInstalled
	$(error "doxygen is not installed but is mandatory to create the release version.")
endif
ifeq ($(operatingSystem),)
	$(error "Unable to detect your operating system to create the release version.")
endif
	rm -rf release && mkdir -p release/pvcollib && \
	cp -r devkitcol release/pvcollib/devkitcol && \
	mkdir release/pvcollib/pvcollib && cp -r pvcollib/include release/pvcollib/pvcollib/include && \
	cp -r pvcollib/lib release/pvcollib/pvcollib/lib && \
	mkdir release/pvcollib/pvcollib/docs && cp -r pvcollib/docs/html release/pvcollib/pvcollib/docs/html && \
	cp pvcollib/pvcollib_Logo.png release/pvcollib/pvcollib/pvcollib_Logo.png && \
	cp pvcollib/pvcollib_license.txt release/pvcollib/pvcollib/pvcollib_license.txt && \
    cp pvcollib/pvcollib_version.txt release/pvcollib/pvcollib/pvcollib_version.txt && \
	cp -r coleco-examples release/pvcollib/coleco-examples && \
	cd release && zip -r -m pvcollib_32b_$(operatingSystem).zip pvcollib && \
	echo && echo Release created successfully ! && echo

.PHONY: all
