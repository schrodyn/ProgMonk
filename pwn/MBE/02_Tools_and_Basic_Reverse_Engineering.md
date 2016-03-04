#Tools and Basic Reverse Engineering

##Hex Editor / Viewers

`xxd` -- “-i” option is C include style 

Challenge: **crackme0x00a** 

##ASCII Readable Hex 

`strings` -- Displays ACII strings > 4 characters long 

Challenge: **crackme0x00a**, **crackme0x00b**

Hint: `strings –e ? crackme0x00b` 

##File Formats on Disk 

`readelf` for Linux, `Peview.exe` for Windows. 

`file` -- For unknown files / binaries 

##Hashing 

`md5sum` -- Do we have the same file?

Upload hash to virustotal.com, Google search hash 

##Command Line Disassembly 

Challenge: **crackme0x01**

Hint: `objdump –d`

`echo $((0xDEADBEEF))` -- Convert hex to decimal

##Patching Binaries

It’s your binary, you can patch it if you want to

```
objdump –d crackme0x00a | grep –A 30 ‘<main>’ 
```

##Disassembly 

```
objdump –d
```

On windows: `IDA Pro.exe`

Challenges: **crackme0x01**, **crackme0x02**

**My Comment**

To run 32-bit applications on 64-bit Arch Linux you should install

```
pacman -S lib32-gcc-libs
```

