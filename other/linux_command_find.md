#35 Practical Examples of Linux Find Command

The Linux Find Command is one of the most important and much used 
command in Linux sytems. Find command used to search and locate list of
files and directories based on conditions you specify for files that 
match the arguments.

##1. Find Files Using Name in Current Directory

Find all the files whose name is `tecmint.txt` in a current working 
directory.

```
# find . -name tecmint.txt

./tecmint.txt
```

##2. Find Files Under Home Directory

Find all the files under `/home` directory with name `tecmint.txt`.

```
# find /home -name tecmint.txt

/home/tecmint.txt
```

##3. Find Files Using Name and Ignoring Case

Find all the files whose name is `tecmint.txt` and contains both 
capital and small letters in `/home` directory.

```
# find /home -iname tecmint.txt

./tecmint.txt
./Tecmint.txt
```

##4. Find Directories Using Name

Find all directories whose name is `Tecmint` in `/` directory.

```
# find / -type d -name Tecmint

/Tecmint
```

##5. Find PHP Files Using Name

Find all `php` files whose name is `tecmint.php` in a current working 
directory.

```
# find . -type f -name tecmint.php

./tecmint.php
```

##6. Find all PHP Files in Directory

Find all `php` files in a directory.

```
# find . -type f -name "*.php"

./tecmint.php
./login.php
./index.php
```

##7. Find Files With 777 Permissions

Find all the files whose permissions are 777.

```
# find . -type f -perm 0777 -print
```

`-print` option prints the full file name on the standard output, 
followed by a newline. If you are piping the output of `find` into 
another program then you should seriously consider using the  
`-print0` option instead of `-print`.

`-print0` prints the full file name on the standard output, 
followed by a null character (instead of the newline character that 
`-print` uses).

##8. Find Files Without 777 Permissions

Find all the files without permission 777.

```
# find / -type f ! -perm 777
```

##9. Find SGID Files with 644 Permissions

Find all the `SGID bit` files whose permissions set to 644.

```
# find / -perm 2644
```

##10. Find Sticky Bit Files with 551 Permissions

Find all the `Sticky Bit` set files whose permission are 551.

```
# find / -perm 1551
```

When a directory's sticky bit is set, the filesystem treats the 
files in such directories in a special way so 
**only the file's owner**, **the directory's owner**, or root user can
rename or delete the file. Without the sticky bit set, any user with 
write and execute permissions for the directory can rename or delete 
contained files, regardless of the file's owner. 

```
"The sticky bit applied to executable programs flagging the system to keep an image of the program in memory after the program finished running."
```

That's quite obsolete info, today most modern Unixes ignore that. In 
Linux, the sticky bit is only relevant for directories.

Sticky-bit programs were meant to stay resident in RAM, not in swap 
(loading an image from a swap file isn't that much faster than 
loading it from a filesystem disk). It was intended for essential 
OS-level commands like `ls`. Obviously, only the superuser could set 
the sticky bit on a file. It became less important after virtual 
memory and shared libraries were introduced, and especially as pagers 
became smarter and can dynamically decide which pages to keep 
resident.

##11. Find SUID Files

Find all `SUID` set files.

```
# find / -perm /u=s
```

##12. Find SGID Files

Find all `SGID` set files.

```
# find / -perm /g+s
```

##13. Find Read Only Files

Find all **Read Only** files.

```
# find / -perm /u=r
```

##14. Find Executable Files

Find all **Executable** files.

```
# find / -perm /a=x
```

##15. Find Files with 777 Permissions and Chmod to 644

Find all 777 permission files and use `chmod` command to set 
permissions to 644.

```
# find / -type f -perm 0777 -print -exec chmod 644 {} \;
```

##16. Find Directories with 777 Permissions and Chmod to 755

Find all 777 permission directories and use chmod command to set permissions to 755.

```
# find / -type d -perm 777 -print -exec chmod 755 {} \;
```

##17. Find and remove single File

To find a single file called `tecmint.txt` and remove it.

```
# find . -type f -name "tecmint.txt" -exec rm -f {} \;
```

##18. Find and remove Multiple File

To find and remove multiple files such as `.mp3` or `.txt`, then use.

```
# find . -type f -name "*.txt" -exec rm -f {} \;

OR

# find . -type f -name "*.mp3" -exec rm -f {} \;
```

##19. Find all Empty Files

To find all empty files under certain path.

```
# find /tmp -type f -empty
```

##20. Find all Empty Directories

To find all empty directories under certain path.

```
# find /tmp -type d -empty
```

##21. Find all Hidden Files

To find all hidden files, use below command.

```
# find /tmp -type f -name ".*"
```

##22. Find Single File Based on User

To find all or single file called `tecmint.txt` under `/` root 
directory of **owner root**.

```
# find / -user root -name tecmint.txt
```

##23. Find all Files Based on User

To find all files that belongs to user `Tecmint` under `/home` 
directory.

```
# find /home -user tecmint
```

##24. Find all Files Based on Group

To find all files that belongs to group `Developer` under `/home` 
directory.

```
# find /home -group developer
```

##25. Find Particular Files of User

To find all `.txt` files of user `Tecmint` under `/home` directory.

```
# find /home -user tecmint -iname "*.txt"
```

##26. Find Last 50 Days Modified Files

To find all the files which are modified 50 days back.

```
# find / -mtime 50
```

##27. Find Last 50 Days Accessed Files

To find all the files which are accessed 50 days back.

```
# find / -atime 50
```

##28. Find Last 50-100 Days Modified Files

To find all the files which are modified more than 50 days back and 
less than 100 days.

```
# find / -mtime +50 –mtime -100
```

##29. Find Changed Files in Last 1 Hour

To find all the files which are changed in last `1 hour`.

```
# find / -cmin -60
```

##30. Find Modified Files in Last 1 Hour

To find all the files which are modified in last `1 hour`.

```
# find / -mmin -60
```

##31. Find Accessed Files in Last 1 Hour

To find all the files which are accessed in last `1 hour`.

```
# find / -amin -60
```

##32. Find 50MB Files

To find all `50MB` files, use.

```
# find / -size 50M
```

##33. Find Size between 50MB – 100MB

To find all the files which are greater than `50MB` and less 
than `100MB`.

```
# find / -size +50M -size -100M
```

##34. Find and Delete 100MB Files

To find all `100MB` files and delete them using one single command.

```
# find / -size 100M -exec rm -rf {} \;
```

##35. Find Specific Files and Delete

Find all `.mp3` files with more than `10MB` and delete them using one 
single command.

```
# find / -type f -name *.mp3 -size +10M -exec rm {} \;
```





