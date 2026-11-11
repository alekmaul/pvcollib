# do not forget that to build pvcollib, you have to install compiler and tools first !
#---------------------------------------------------------------------------------
# define variables
COMPILER_PATH       := compiler
TOOLS_PATH          := tools
COL_EXAMPLES_PATH  := coleco-examples
COL_VSCODE_PATH 	:= vscode-template
PVCOLLIB_PATH      := pvcollib
RELEASE_PATH        := release/pvcollib

DOXYGEN_INSTALLED   := $(shell command -v doxygen 2> /dev/null)
ARCH := 64b
# Define variables for System
UNAME := $(shell uname -s)

# Set default operating system
ifeq ($(OS),Windows_NT)
	OPERATING_SYSTEM := windows
else ifneq ($(findstring MINGW64_NT,$(UNAME)),)
	OPERATING_SYSTEM := mingw
else ifeq ($(UNAME), Darwin)
	OPERATING_SYSTEM := darwin
else ifeq ($(UNAME), Linux)
	OPERATING_SYSTEM := linux
else ifeq ($(UNAME), FreeBSD)
	OPERATING_SYSTEM := freebsd
else
	$(error Unsupported operating system)
endif

# SDCC compiler, downloaded from the pvcollib "sdcc-x.y.z" GitHub release
SDCC_VERSION := 3.9.0
SDCC_URL     := https://github.com/alekmaul/pvcollib/releases/download/sdcc-$(SDCC_VERSION)
ifneq ($(filter windows mingw,$(OPERATING_SYSTEM)),)
SDCC_OS := windows
else
SDCC_OS := $(OPERATING_SYSTEM)
endif
SDCC_ARCHIVE := sdcc-$(SDCC_VERSION)-$(SDCC_OS).tar.gz
SDCC_STAMP   := devkitcol/bin/.sdcc-$(SDCC_VERSION)

# default target - dependencies allow parallel builds with make -j
all: sdcc tools pvcollib examples
	@echo
	@echo "* Build finished successfully !"
	@echo

# download and install SDCC in devkitcol/bin (only once per SDCC version)
sdcc: $(SDCC_STAMP)

$(SDCC_STAMP):
	@echo "* Installing SDCC $(SDCC_VERSION) for $(SDCC_OS) in devkitcol ..."
	@curl -fsSL -o $(SDCC_ARCHIVE) $(SDCC_URL)/$(SDCC_ARCHIVE)
	@tar -xzf $(SDCC_ARCHIVE) -C devkitcol
	@chmod +x devkitcol/bin/*
	@rm -f $(SDCC_ARCHIVE)
	@touch $@

# build pvcollib (requires compiler to be installed)
pvcollib: sdcc
	$(MAKE) -C $(PVCOLLIB_PATH)

# build tools (independent of compiler, can run in parallel)
tools:
	$(MAKE) -C $(TOOLS_PATH)
	$(MAKE) -C $(TOOLS_PATH) install

# build coleco-examples (requires pvcollib and tools)
examples: sdcc pvcollib tools
	$(MAKE) -C $(COL_EXAMPLES_PATH)
	$(MAKE) -C $(COL_EXAMPLES_PATH) install

# clean everything
clean:
	$(MAKE) -C $(TOOLS_PATH) clean
	$(MAKE) -C $(PVCOLLIB_PATH) clean
	$(MAKE) -C $(COL_EXAMPLES_PATH) clean
	@rm -rf release

#---------------------------------------------------------------------------------	
# to create the release version for github containing binaries and all coleco-examples :
release: all
ifndef DOXYGEN_INSTALLED
	$(error "doxygen is not installed but is mandatory to create the release version.")
endif
ifeq ($(OPERATING_SYSTEM),)
	$(error "Unable to detect your operating system to create the release version.")
endif
	@mkdir -p $(RELEASE_PATH)/$(PVCOLLIB_PATH)
	@cp -r devkitcol $(RELEASE_PATH)
	@cp -r $(PVCOLLIB_PATH)/include $(RELEASE_PATH)/$(PVCOLLIB_PATH)
	@cp -r $(PVCOLLIB_PATH)/lib $(RELEASE_PATH)/$(PVCOLLIB_PATH)
	@cp -r $(PVCOLLIB_PATH)/docs/html $(RELEASE_PATH)/$(PVCOLLIB_PATH)
	@cp pvcollib_logo.png $(RELEASE_PATH)/$(PVCOLLIB_PATH)/pvcollib_logo.png
	@cp $(PVCOLLIB_PATH)/pvcollib_Logo.png $(RELEASE_PATH)/$(PVCOLLIB_PATH)/pvcollib_Logo.png
	@cp $(PVCOLLIB_PATH)/pvcollib_license.txt $(RELEASE_PATH)/$(PVCOLLIB_PATH)/pvcollib_license.txt
	@cp $(PVCOLLIB_PATH)/pvcollib_version.txt $(RELEASE_PATH)/$(PVCOLLIB_PATH)/pvcollib_version.txt
	@cp -r $(COL_EXAMPLES_PATH) $(RELEASE_PATH)/coleco-examples
	@cp -r $(COL_VSCODE_PATH) $(RELEASE_PATH)/vscode-template
	@cd release && zip -q -y -r -m pvcollib_$(ARCH)_$(OPERATING_SYSTEM).zip pvcollib

	@echo "* Release pvcollib_$(ARCH)_$(OPERATING_SYSTEM) created successfully !"
	@echo

.PHONY: all clean release tools sdcc pvcollib examples
