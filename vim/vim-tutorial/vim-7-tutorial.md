#Programmers Editor

Vim tends to be heavily used by programmers. The features, ease of use and flexibility that
Vim provides makes it a good choice for people who write a lot of code. This should not
seem surprising since writing code involves a lot of editing.

For those who love programming, let's dive in and see Vim can help you in writing code.

##Shell commands

You can run a shell command from within Vim using the `:!` command.

For example, if the date command is available on your operating system, run `:!date` and
you should see the current date and time printed out.

This is useful in situations where you want to check something with the file system, for
example, quickly checking what files are in the current directory using `:!ls` and so on.

If you want access to a full-fledged shell, run `:sh`.

We can use this facility to run external filters for the text being 
edited. For example, if you have a bunch of lines that you want to 
sort, then you can run `:%!sort`, this passes the
current text to the `sort` command in the shell and then the output of 
that command replaces the current content of the file.

##Jumping around

There are many ways of jumping around the code.

Position your cursor on a variable name and press `gd` to move the 
local definition of the variable name. `gD` achieves the same for the 
global declaration by searching from the start of the file.

##Browsing parts of the code

Use `:Vex` or `:Sex` to browse the file system within Vim and 
subsequently open the required files.

##Easy writing

One of the most-requested features which was added in Vim 7 is "omnicompletion" where the text can be auto-completed based on the current 
context.
For example, if you are using a long variable name and you are using 
the name repeatedly, you can use a keyboard
shortcut to ask Vim to auto-complete and it'll figure out the rest.

Press `ctrl-n` which will show you suggestions for the autocompletion.

Vim automatically uses the first suggestion, you can change to the 
next or previous selection using `ctrl-N` and `ctrl-P` respectively.

In case you want to abort using the omnicompletion, simply press `esc`.

If you are more comfortable in using the arrow keys to select your 
choice from the omnicompletion list add the following to your vimrc:

```
inoremap <C-k> <C-P>
inoremap <C-j> <C-N>
```

##Don't type it twice

There is a limited set of words we type. And even a limited number of 
phrases and sentences. Especially in computer programs.  Obviously, 
you don't want to type the same thing twice.

Very often you will want to change one word into another. If this is 
to be done in the whole file, you can use the `:s` (substitute) 
command. If only a few locations needs changing, a quick method is to 
use the `*` command to find the next occurrence of the word and use 
`cw` to change the word. Then type `n` to find the next word and `.`
(dot) to repeat the `cw` command.

The `.` command repeats the last change. A change, in this context, is 
inserting, deleting or replacing text. Being able to repeat this is a 
very powerful mechanism. If you organise your editing around it, many 
changes will become a matter of hitting just that `.` key. 

When you are typing a phrase or sentence multiple times, there is an 
even quicker approach. Vim has a mechanism to record a macro. You type 
`qa` to start recording into register 'a'. Then you type your commands 
as usual and finally hit `q` again to stop recording. When you want to 
repeat the recorded commands you type `@a`. There are 26 registers 
available for this.

With recording you can repeat many different actions, not just 
inserting text. Keep this is mind when you know you are going to 
repeat something.

##Fix it when it's wrong

It's normal to make errors while typing. Nobody can avoid it. 
The trick is to quickly spot and correct them. The editor should be 
able to help you with this. But you need to tell it what's wrong and 
what's right.

Very often you will make the same mistake again and again. Your 
fingers just don't do what you intended. This can be corrected with 
abbreviations. A few examples:

```
:abbr Lunix Linux
:abbr accross across
:abbr hte the
```

The words will be automatically corrected just after you typed them.

The same mechanism can be used to type a long word with just a few 
characters. Especially useful for words that you find hard to type, 
and it avoids that you type them wrong. Examples:

```
:abbr pn penguin
:abbr MS Mandrake Software
```

