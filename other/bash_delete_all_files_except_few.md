#Bash Delete All Files In Directory Except Few

`*.iso` - All iso images files.

`*.zip` - All zip files.

How do I delete all file except some in bash shell on a Linux, OS X or 
Unix-like systems?

Bash shell supports rich file pattern matching such as follows:

  * `*` - Match any files.
  * `?` - Matches any single character in filenames.
  * `[...]` - Matches any one of the enclosed characters.

##Method \#1: Say hello to extended pattern matching operators

You need to use the `extglob` shell option using the `shopt` builtin 
command to use extended pattern matching operators such as:

  * `?(pattern-list)` - Matches zero or one occurrence of the given 
  patterns.
  * `*(pattern-list)` - Matches zero or more occurrences of the given 
  patterns.
  * `+(pattern-list)` - Matches one or more occurrences of the given 
  patterns.
  * `@(pattern-list)` - Matches one of the given patterns.
  * `!(pattern-list)` - Matches anything except one of the given 
  patterns.

A `pattern-list` is nothing but a list of one or more patterns 
(filename) separated by a `|`. First, turn on `extglob` option:

```
shopt -s extglob
```

**Bash remove all files except \*.zip and \*.iso files**

The rm command syntax is:

```sh
## Delete all file except file1 ##
rm  !(file1)
 
## Delete all file except file1 and file2 ##
rm  !(file1|file2)
  
## Delete all file except all zip files ##
rm  !(*.zip)
   
## Delete all file except all zip and iso files ##
rm  !(*.zip|*.iso)
    
## You set full path too ##
rm /Users/vivek/!(*.zip|*.iso|*.mp3)
	 
## Pass options ##
rm [options]  !(*.zip|*.iso)
rm -v  !(*.zip|*.iso)
rm -f  !(*.zip|*.iso)
rm -v -i  !(*.php)
```

Finally, turn off `extglob` option:

```
shopt -u extglob
```

##Method \#2: Using bash GLOBIGNORE variable to remove all files except specific ones

To delete all files except zip and iso files, set `GLOBIGNORE` as 
follows:

```sh
## only works with BASH ##
cd ~/Downloads/
GLOBIGNORE=*.zip:*.iso
rm -v *
unset GLOBIGNORE
```




