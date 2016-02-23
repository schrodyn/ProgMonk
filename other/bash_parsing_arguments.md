#Bash: parsing arguments with getopts

When you want to parse command line arguments in a professional way, 
`getopts` is the tool of choice. 

**Note that** `getopts` is not able to parse GNU-style long options 
(`--myoption`) or XF86-style long options (`-myoption`)!

The option flags can be **upper- and lowercase** characters, or 
**digits**. It may recognize other characters, but that's not 
recommended (usability and maybe problems with special characters).

Let me show you how useful it can be:

Let’s suppose that I’m writing a test script, that needs, as argument, 
the type of the test, the server, the server root password and for 
debugging purpose we’re going to have a verbose flag too. So, 
putting it down:

  * **-t** – the type of the test, let’s suppose we have “test1” and 
  “test2”

  * **-s** – the server

  * **-p** – the root password of the server

  * **-v** – a flag just to let the script run in a verbose mode

Ok, but how can you deal with arguments not worrying about the order 
and if needs an argument or not? **Getopts** is the answer ;)

Let’s see how we can write the script using **getopts** and them we 
explain how it works.

```bash
#!/bin/bash
# Argument = -t test -r server -p password -v

usage()
{
cat << EOF
usage: $0 options

This script run the test1 or test2 over a machine.

OPTIONS:
 -h      Show this message
 -t      Test type, can be ‘test1’ or ‘test2’
 -r      Server address
 -p      Server root password
 -v      Verbose
 EOF
}

TEST=
SERVER=
PASSWD=
VERBOSE=
while getopts “ht:r:p:v” OPTION
do
	case $OPTION in
		h)
			usage
			exit 1
			;;
		t)
			TEST=$OPTARG
			;;
		r)
			SERVER=$OPTARG
			;;
		p)
			PASSWD=$OPTARG
			;;
		v)
			VERBOSE=1
			;;
		?)
			usage
			exit
			;;
	esac
done

if [[ -z $TEST ]] || [[ -z $SERVER ]] || [[ -z $PASSWD ]]
then
	usage
	exit 1
fi
```

In this script I created a **usage** function, just to help you 
explaining all arguments.

Then, we can see the **getopts** call `while getopts "ht:r:p:v" OPTION`
, this is the main point of the script, it’s how we deal with arguments
using **getopts**. Getopts require an optstring and a var name, just to
help you checking the arguments.

When you call **getopts**, it will walk in your optstring argument, 
identifying which argument needs a value and which don’t. After getting
an argument, getopts set the OPTION var, so you can check it using a 
**case** code block, or something like that. If your argument needs a 
value, getopts will set the var **$OPTARG** with the value, so you can 
check and see if it’s what you were expecting.

`$OPTARG` variable is set to any argument for an option found by 
`getopts`. It also contains the option flag of an unknown option.

#Specify what you want

The base-syntax for `getopts` is:

```
getopts OPTSTRING VARNAME [ARGS...]
```

where:

`OPTSTRING` tells getopts which options to expect and where to expect 
arguments (see below)

`VARNAME` tells getopts which shell-variable to use for option 
reporting

`ARGS` tells getopts to parse these optional words instead of the 
positional parameters

##The option-string

The option-string tells getopts which options to expect and which of 
them must have an argument. The syntax is very simple — every option 
character is simply named as is, this example-string would tell 
getopts to look for `-f`, `-A` and `-x`:

```
getopts fAx VARNAME
```

When you want `getopts` to expect an argument for an option, just 
place a `:` (colon) after the proper option flag. If you want `-A` to 
expect an argument (i.e. to become `-A SOMETHING`) just do:

```
getopts fA:x VARNAME
```

If the **very first character** of the option-string is a 
`:` (colon), which would normally be nonsense because there's no 
option letter preceding it, `getopts` switches to **"silent error 
reporting mode"**. In productive scripts, this is usually what you 
want because it allows you to handle errors yourself without being 
disturbed by annoying messages.

#Error Reporting

Regarding error-reporting, there are two modes `getopts` can run in:

  * verbose mode
  * silent mode

For productive scripts I recommend to use the silent mode, since 
everything looks more professional, when you don't see annoying 
standard messages. Also it's easier to handle, since the failure cases
are indicated in an easier way.

##Verbose Mode

 |
--|--
**invalid option** | `VARNAME` is set to `?` (question-mark) and `OPTARG` is unset
**required argument not found** | `VARNAME` is set to `?` 
(question-mark), `OPTARG` is unset and an *error message* is printed

##Silent Mode

 |
--|--
**invalid option** | `VARNAME` is set to `?` (question-mark) and 
`OPTARG` is set to the (invalid) option character
**required argument not found** | VARNAME is set to `:` (colon) and 
`OPTARG` contains the option-character 

#Using it

##A first example

Enough said - action!

Let's play with a very simple case: only one option (`-a`) expected, 
without any arguments. Also we disable the verbose error handling by 
preceding the whole option string with a colon (`:`):

```bash
#!/bin/bash
 
while getopts ":a" opt; do
	case $opt in
		a)
			echo "-a was triggered!" >&2
			;;
		\?)
			echo "Invalid option: -$OPTARG" >&2
			;;
	esac
done
```

I put that into a file named `go_test.sh`, which is the name you'll 
see below in the examples.

Let's do some tests:

##Calling it without any arguments

```
$ ./go_test.sh
$ 
```
Nothing happened? Right. `getopts` didn't see any valid or invalid 
options (letters preceded by a dash), so it wasn't triggered.

##Calling it with non-option arguments

```
$ ./go_test.sh /etc/passwd
$ 
```

Again — nothing happened. The **very same** case: `getopts` didn't see
any valid or invalid options (letters preceded by a dash), so it 
wasn't triggered.

The arguments given to your script are of course accessible as `$1` - 
`${N}`.

##Calling it with option-arguments

Now let's trigger `getopts`: Provide options.

First, an **invalid** one:

```
$ ./go_test.sh -b
Invalid option: -b
$ 
```

As expected, `getopts` didn't accept this option and acted like told 
above: It placed `?` into `$opt` and the invalid option character 
(`b`) into `$OPTARG`. With our `case` statement, we were able to 
detect this.

Now, a **valid** one (`-a`):

```
$ ./go_test.sh -a
-a was triggered!
$ 
```

You see, the detection works perfectly. The a was put into the 
variable `$opt` for our case statement.

Of course it's possible to **mix valid and invalid** options when 
calling:

```
$ ./go_test.sh -a -x -b -c
-a was triggered!
Invalid option: -x
Invalid option: -b
Invalid option: -c
$ 
```

Finally, it's of course possible, to give our option **multiple 
times**:

```
$ ./go_test.sh -a -a -a -a
-a was triggered!
-a was triggered!
-a was triggered!
-a was triggered!
$ 
```

The last examples lead us to some points you may consider:

  * **invalid options don't stop the processing**: If you want to stop
  the script, you have to do it yourself (`exit` in the right place)

  * multiple identical options are possible: If you want to disallow 
  these, you have to check manually (e.g. by setting a variable or so)

#An option with argument

Let's extend our example from above. Just a little bit:

  * `-a` now takes an argument
  * on an error, the parsing exits with `exit 1`

```bash
#!/bin/bash
 
while getopts ":a:" opt; do
	case $opt in
		a)
			echo "-a was triggered, Parameter: $OPTARG" >&2
			;;
		\?)
			echo "Invalid option: -$OPTARG" >&2
			exit 1
			;;
		:)
			echo "Option -$OPTARG requires an argument." >&2
			exit 1
			;;
	esac
done
```

Let's do the very same tests we did in the last example:

##Calling it without any arguments

```
$ ./go_test.sh
$ 
```

As above, nothing happened. It wasn't triggered.

##Calling it with non-option arguments

```
$ ./go_test.sh /etc/passwd
$ 
```

The **very same** case: It wasn't triggered.

##Calling it with option-arguments

**Invalid** option:

```
$ ./go_test.sh -b
Invalid option: -b
$ 
```

As expected, as above, `getopts` didn't accept this option and acted 
like programmed.

**Valid** option, but without the mandatory **argument**:

```
$ ./go_test.sh -a
Option -a requires an argument.
$ 
```

The option was okay, but there is an argument missing.

Let's provide **the argument**:

```
$ ./go_test.sh -a /etc/passwd
-a was triggered, Parameter: /etc/passwd
$
```

