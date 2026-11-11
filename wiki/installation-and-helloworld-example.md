Here, you will find detailed instructions for installing PVcollib on **Windows, Linux and macOS**.

To use PVcollib, you will need:
  * The latest version for your system from the [Download section](https://github.com/alekmaul/pvcollib/wiki/Download)
  * `make`, to compile your programs (see below for your system)
  * An editor like Programmer's Notepad, Visual Studio Code, ...
  * An emulator to test your homebrews

Good luck!

## Installing PVcollib

### Step 1: Unzip PVcollib

The archive contains PVcollib, the SDCC 3.9.0 compiler and the tools for your system: there is nothing else to install for the compiler.

Unzip it wherever you like, **in a directory without spaces** (eg, `C:\colecodev` on Windows or `~/colecodev` on Linux/macOS). You will get a **pvcollib** folder like this:

```
  colecodev
  .....\pvcollib
  ..........\coleco-examples      examples (helloworld, graphics, audio, games, ...)
  ..........\devkitcol
  ...............\bin             SDCC 3.9.0 compiler (sdcc, sdasz80, ...)
  ...............\include         SDCC headers
  ...............\lib             SDCC z80 library
  ...............\tools           gfx2col, cvmkcart, bin2txt
  ...............col_rules        compilation rules used by the Makefiles
  ..........\pvcollib
  ...............\include         PVcollib headers
  ...............\lib             PVcollib library (collib.lib, crtcol.rel)
  ...............\html            PVcollib documentation (open index.html)
```

### Step 2: Install `make`

#### Windows

Install [MSYS2](https://www.msys2.org/) (keep the default folder `C:\msys64`), then open the **MSYS2 UCRT64** terminal and type:
```
pacman -S make
```
To use `make` from other programs (Programmer's Notepad, a Windows command prompt, ...), add **C:\msys64\usr\bin** to your Windows **Path**: search for "environment variables" in the Windows start menu, click on "Environment Variables", select the **Path** entry of your user, click on **Edit**, then **New**, and type `C:\msys64\usr\bin`.

#### Linux

Install `make` with your package manager, for example on Ubuntu/Debian:
```
sudo apt install make
```

#### macOS

Install the Xcode command line tools (they contain `make`):
```
xcode-select --install
```
SDCC 3.9.0 only exists for Intel Macs. On a Mac with an Apple chip (M1, M2, ...), install Rosetta once:
```
softwareupdate --install-rosetta --agree-to-license
```

### Step 3: Define the PVCOLLIB_HOME variable

The Makefiles use the **PVCOLLIB_HOME** environment variable to find PVcollib. It must contain the path of the **pvcollib** folder, **in Unix style, also on Windows** (`/c/colecodev/pvcollib`, not `c:\colecodev\pvcollib`).

#### Windows

In a Windows command prompt:
```
setx PVCOLLIB_HOME "/c/colecodev/pvcollib"
```
Then close and reopen your terminals and editors so that they see the new variable.

#### Linux

Add this line at the end of your `~/.bashrc` file, then open a new terminal:
```
export PVCOLLIB_HOME=$HOME/colecodev/pvcollib
```

#### macOS

Add this line at the end of your `~/.zshrc` file, then open a new terminal:
```
export PVCOLLIB_HOME=$HOME/colecodev/pvcollib
```

### Step 4: Compile the hello world example

We will use the hello world example to test that PVcollib is correctly installed. Go to the helloworld example folder and type `make` (on Windows, use the **MSYS2 UCRT64** terminal):

```
cd $PVCOLLIB_HOME/coleco-examples/helloworld
make
```

You should get a **helloworld.rom** file, with something like this at the end:
```
Linking ... helloworld.rom
...
Build finished successfully !
```

If an error occurs, check that `PVCOLLIB_HOME` is correct (`echo $PVCOLLIB_HOME`) and that it doesn't contain spaces or `\`. You can post your problem in our [Portabledev Forum](http://www.portabledev.com/smf/index.php) or here in the [Issues Part](https://github.com/alekmaul/pvcollib/issues), we will help you as soon as possible.

### Step 5: Test with an emulator

Run **helloworld.rom** with a Colecovision emulator, for example:

[bluemsx](http://www.bluemsx.com/)

[bluemsx Fullpack from Emu-France](http://www.emu-france.com/?wpfb_dl=2557)

[ColEM](http://fms.komkon.org/ColEm/)

That's all, you've compiled your first program that does nothing except display the fantastic "Hello World!" message but it's a first step !

## Building PVcollib from sources (optional)

Only needed if you want to modify PVcollib itself. You need `git`, `make`, `gcc`, `curl`, `tar` and `doxygen` (on Windows, from the **MSYS2 UCRT64** terminal: `pacman -S git base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-doxygen zip`).

```
git clone https://github.com/alekmaul/pvcollib.git
cd pvcollib
export PVCOLLIB_HOME=$(pwd)
make
```

The first `make` downloads **SDCC 3.9.0** automatically into `devkitcol/bin` (from the [sdcc-3.9.0 release](https://github.com/alekmaul/pvcollib/releases/tag/sdcc-3.9.0)), then builds the tools, the library and all the examples. `make release` also creates the archive for your system in the `release` folder.

Welcome to PVcollib world and enjoy doing some homebrews for your Colecovision :-P !
