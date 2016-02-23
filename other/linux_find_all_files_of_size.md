#Find All Files of a Particular Size

Let’s assume you are searching for all the files of exactly 6579 bytes 
size inside the home directory. You will just have to run something 
like:

```
find /home/ -type f -size 6579c -exec ls {} \;
```

As units you can use:

  * b – for 512-byte blocks (this is the default if no suffix is used)
  * c – for bytes
  * w – for two-byte words
  * k – for Kilobytes (units of 1024 bytes)
  * M – for Megabytes (units of 1048576 bytes)
  * G – for Gigabytes (units of 1073741824 bytes)

You can search for exact file size, or just for *bigger* (`+`) or 
*smaller* (`–`) files. For example all bigger than 512k files would be 
found with something like:

```
find /home/ -type f -size +512k -exec ls -lh {} \;
```

I have added here `-lh` to the `ls` output so it will actually show the
files with their sizes in a nice human readable format. Similar for 
smaller files you would use `-size -512k`.
