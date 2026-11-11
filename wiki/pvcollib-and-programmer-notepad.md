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

With Programmer's Notepad menu **File/Open**, open the 3 files of the **coleco-examples/helloworld** directory:  
* helloworld.c is the C source file.
* helloworld.h is the header file of the helloworld.c source file.
* Makefile is the file used to make the .rom file.

You don't need to change anything in the **Makefile**: it uses the **PVCOLLIB_HOME** environment variable to find PVcollib (see [Installation & basic example](https://github.com/alekmaul/pvcollib/wiki/Installation-and-helloworld-example), step 3).

```
  include ${PVCOLLIB_HOME}/devkitcol/col_rules
```

If Programmer's Notepad was open when you defined **PVCOLLIB_HOME**, close it and open it again so that it sees the variable. `make` must also be in your Windows Path (**C:\msys64\usr\bin**, see the installation page).

Now, just do a //make clean// command (with the shortcut you configured above, for example Alt+é for me).
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
/c/colecodev/pvcollib/devkitcol/bin/sdcc -mz80 -I/c/colecodev/pvcollib/pvcollib/include -I/c/colecodev/pvcollib/devkitcol/include --std-c99 --opt-code-size --fverbose-asm --max-allocs-per-node 20000 --vc -c helloworld.c -o helloworld.rel
Linking ... helloworld.rom
/c/colecodev/pvcollib/devkitcol/bin/sdcc -mz80 --no-std-crt0 -L/c/colecodev/pvcollib/devkitcol/lib/z80 --code-loc 0x8048 --data-loc 0x7000 /c/colecodev/pvcollib/pvcollib/lib/collib.lib /c/colecodev/pvcollib/pvcollib/lib/crtcol.rel helloworld.rel
/c/colecodev/pvcollib/devkitcol/bin/sdobjcopy -R .sec5 -I ihex -O binary --pad-to 0xffff --gap-fill 0xff crtcol.ihx helloworld.rom
...

Build finished successfully !

> Process Exit Code: 0
> Time Taken: 00:06
```

That's all, you've compiled your first program that does nothing except display the fantastic "Hello World!" message but it's a first step !.  

Welcome to PVcollib world and enjoy doing some homebrews for your Colecovision :-P !