#Checking for empty string in Bash

In Bash you quite often need to check to see if a variable has been set
or has a value other than an empty string. This can be done using the 
`-n` or `-z` string comparison operators.

The `-n` operator checks whether the string is not null. Effectively, 
this will return true for every case except where the string contains 
no characters. ie:

```bash
VAR="hello"
if [ -n "$VAR" ]
then
	echo "VAR is not empty"
fi
```

Similarly, the `-z` operator checks whether the string is null. ie:

```bash
VAR=""
if [ -z "$VAR" ] 
then
	echo "VAR is empty"
fi
```

Note the spaces around the square brackets. Bash will complain if the 
spaces are not there.
