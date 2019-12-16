Here, You will find  detailed instruction for installing PVcollib  
To use PVcollib, you will need:  
  * Last version from [Download section](https://github.com/alekmaul/pvcollib/wiki/Download)
  * An editor like Programmer's Notepad or Eclipse (if your PC is strong enough ;-))  

Additional sections below will cover others OS installation like Linux and Mac OS X.  

Good luck!  

## Installing PVcollib

### Step 1: Installing the toolchain

The first thing you need to do to get you started is downloading the latest versions of PVcollib and the tools that come with it. This is the core to all/most homebrew programs on the Colecovision, as it provides the C compiler and linker and various tools.  

Put it wherever you like – it doesn’t affect the compilation (you will only need to define it in your PATH), as long as you don’t extract it in a directory that contains spaces (eg, ‘**C:/colecodev/**’ would be fine).  

#### Msys

You can use msys to have a Unix like environment, but it is not mandatory.  

Download **msys** to use Unix like environment and extract it in your coleco directory. (eg, **C:\colecodev\** would be fine). You will have a subdirectory name **msys** with all msys distribution in it.  

[msys 1.0.17](http://www.portabledev.com/download/11/)

Msys needs to be added to Windows Path because lots of msys binary files are needed when we are going to compile.  

To add the **msys\bin** directory to your PATH environment variable (eg,  you will add  **c:\colecodev\msys\bin** in our example).  
I'm French with a Windows 7 computer, so the name will not reflect your exact configuration. The goal is to have the Windows Path text box to add the msys/bin directory. Do a Right Click on "Ordinateur" icon, choose "Paramètres système avancés" and then, click on "Variables d'environnement" button.  

![Path](http://www.portabledev.com/wp-content/uploads/2018/02/pn_tools_04.jpg)

Choose the Path entry to add **c:\colecodev\msys\bin** at the end of the line.  

![Path2](https://www.portabledev.com/wp-content/uploads/2019/11/pn_tools06.png)

#### Emulators  

Download emulators to test your homebrews and put them in the emulators directory (see unzipped devkitcol, you will have the directory **C:/colecodev/emulators**).  

[bluemsx](http://www.bluemsx.com/)

[bluemsx Fullpack from Emu-France](http://www.emu-france.com/?wpfb_dl=2557)

[EmulTwo](https://github.com/alekmaul/pvcollib/tree/master/emulators/emultwo)

[ColEM](http://fms.komkon.org/ColEm/)

At the end, you must have something like that :  
```
  c:\colecodev
  .........\coleco-examples
  .........\devkitcol
  .................\bin
  .................\include
  .................\lib
  .................\tools
  .................col_rules
  .........\emulators
  .........\include
  .........\lib
  .........\bin
```

### Step 2: Installing for windows

If you're using a Windows environment, you will need to compile again the library.  
Go to your SDK folder and type   
<pre>
C:\colecodev>build_pvcollib.bat
</pre>
When the library is compiled you should obtain the following files
<pre>
C:\colecodev>dir lib
Répertoire de C:\colecodev\lib
collib.lib
crtcol.rel
</pre>

##### Let's start compiling with Programmer's Notepad

We will use the hello world example to test how our PVcollib library is installed. The helloworld directory is shipped with PVcollib examples.  
