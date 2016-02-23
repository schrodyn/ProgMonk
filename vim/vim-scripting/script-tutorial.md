#Vim Scripting

There are two approaches to scripting in Vim - using the built-in Vim 
scripting language, or using a full-fledged programming language such as Python or Perl which have access to the 
Vim internals via modules (provided that Vim has been compiled with these options enabled).

There are two ways of creating reusable functionality in Vim - using 
macros and writing scripts.

#Basics of Scripting

Vim has a built-in scripting language using which you can write your own 
scripts to take
decisions, "do" stuff, and manipulate the text.

##Actions

How do you change the theme i.e. colors used by Vim? Just run:

```
:colorscheme desert
```

What if you wanted to know how many characters are in the current line?

```
:echo strlen(getline("."))
```

getline function fetches a line and we are indicating which line by
the `.` (dot) which means the current line. We are passing the result returned by the getline
function to the `strlen` function which counts the number of characters in the text and then
we are passing the result returned by the strlen function to the 
`:echo` command which simply displays the result. Notice how the information flows in this command.

The `strlen(getline("."))` is called an expression. We can store the results of such expressions by using variables.

```
:let len = strlen(getline("."))
:echo "We have" len "characters in this line."
```

Vim has many types of variables available via prefixes such as `$` for environment variables, `&` for options, and `@` for registers:

```
:echo $HOME
:echo &filetype
:echo @a
```

You can create your own functions as well:

```
:function CurrentLineLength()
:	let len = strlen(getline("."))
:	return len
:endfunction
```

Now position your cursor on any line and run the following command:

```
:echo CurrentLineLength()
```

You should see a number printed. Function names have to start with an
upper case. This is to differentiate that built-in
functions start with a lower case and user-defined functions start with an upper case. If you want to simply "call" a function to run but not display the contents, you can use `:call CurrentLineLength()`

##Decisions

Suppose you want to display a different color schemes based on whether Vim is running in
the terminal or is running as a GUI i.e. you need the script to take decisions. Then, you can use:

```
:if has("gui_running")
:	colorscheme desert
:else
:	colorscheme darkblue
:endif
```

`has()` is a function which is used to if a specified feature is supported in Vim installed on the current computer. See `:help feature-list` to see what kind of features are available in Vim.

The looping statements 'for' and 'while' are also available in Vim:

```
:let i = 0
:while i < 5
:	echo i
:	let i += 1
:endwhile
```

Output:

```
0
1
2
3
4
```
Using Vim's built-in functions, the same can also be written as:
```
:for i in range(5)
:	echo i
:endfor
```

The `continue` and `break` statements are also available.

##Data Structures

Vim scripting also has support for lists and dictionaries. Using 
these, you can build up complicated data structures and programs.

```
:let fruits = ['apple', 'mango', 'coconut']

:echo fruits[0]
" apple

:echo len(fruits)
" 3

:call remove(fruits, 0)
:echo fruits
" ['mango', 'coconut']

:call sort(fruits)
:echo fruits
" ['coconut', 'mango']

:for fruit in fruits
:	echo "I like" fruit
:endfor
" I like coconut
" I like mango
```

There are many functions available - see 'List manipulation' and 
'Dictionary manipulation' sections in :help function-list.


