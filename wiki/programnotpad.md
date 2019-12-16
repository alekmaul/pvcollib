## Configuring tools

We will begin with a Programmer's Notepad Tools menu configuration to have the make command with **Alt+&** shortcut on our keyboard (but you are free to use something else :-D).
First of all, go to **Tools/Options** Menu and select Tools entry to the left. On the **Scheme** dropdown list, select **None** to have the "global tools" selections.

![PNT1](http://www.portabledev.com/wp-content/uploads/2018/02/pn_tools_01.jpg)

Click on **Add** button to the right and type text as in the following screen.  

![PNT2](http://www.portabledev.com/wp-content/uploads/2018/02/pn_tools_02.jpg)

Do same thing for the **make clean** entry, you just have to add the word **clean** in **parameters** text box, and add shortcut **Alt+é** on your keyboard (or what you want ;-)).  
That's all, now you have the _make_ and _make clean_ command defined.  

![PNT3](http://www.portabledev.com/wp-content/uploads/2018/02/pn_tools_03.jpg)

## Editing Path and compiling

With Programmer's Notepad menu **File/Open project(s)**, open the //helloworld.pnproj// file that is in the pvcollib **helloworld** directory example.  
You will see 3 files on the Project Window.  
* helloworld.c is the C source file.
* helloworld.h is the header file of the helloworld.c source file.
* Makefile is the file used to make the .rom file.

Open **Makefile** and change the path to use the correct directory for your colecodev installation. Example below show a **c:/colecodev/** root entry for my coleco developments, if you have **c:\colecodev**, you just have to change it to **/c/colecodev**. Same thing for your devkitcol entry.  

```
  # path to colecodev root directory (for emulators, devkitcol, pvcollib)
  export DEVKITCOL := /c/colecodev/
  
  # path to devkitcol root directory for compiler
  export DEVKITSDCC := /c/colecodev/devkitcol
```

Now, just do a //make clean// command (with the shortcut you configured below, for example Alt+é for me).
You must see a new Output window with the result of your make clean command.

```
  > "make" clean
  clean ...
  
  > Process Exit Code: 0
  > Time Taken: 00:00
```

If an error occurs, that's because your installation is not good, sorry about that. You can post your problem in our 
[Portabledev Forum](http://www.portabledev.com/smf/index.php) or here in [Issues Part](https://github.com/alekmaul/pvcollib/issues), we will help you as soon as possible.

Ok, now your template directory is cleaned, you can run the //make// command (with shortcut, remember ).
You will have the following things in your Programmer's Notepad output window.

```
> "make.exe" 
Compiling C to .rel ... helloworld.c
sdcc -mz80 -c -I/c/colecodev//include -I/c/colecodev/devkitcol/include --std-c99 --opt-code-size --fverbose-asm --max-allocs-per-node 20000 --vc   -I/c/colecodev/coleco-examples/helloworld/  helloworld.c
Linking ... helloworld.rom
sdcc -mz80 --no-std-crt0  --code-loc 0x8048 --data-loc 0x7000   /c/colecodev//lib/collib.lib /c/colecodev//lib/crtcol.rel  helloworld.rel 
sdobjcopy -R .sec5 -I ihex -O binary --pad-to 0xffff --gap-fill 0xff crtcol.ihx helloworld.rom
_CODE                               00008048    000003B5 =         949. bytes (REL,CON)
_GSINIT                             000083FD    0000000F =          15. bytes (REL,CON)
_GSFINAL                            0000840C    00000001 =           1. bytes (REL,CON)
_DATA                               00007000    000000F5 =         245. bytes (REL,CON)
_HEADER0                            00000000    00000024 =          36. bytes (ABS,CON)
_CODE = 0x8048
_DATA = 0x7000
Generate rom ... all

> Process Exit Code: 0
> Time Taken: 00:06
```

That's all, you've compiled your first program that does nothing except display the fantastic "Hello World!" message but it's a first step !.  

Welcome to PVcollib world and enjoy doing some homebrews for your Colecovision :-P !