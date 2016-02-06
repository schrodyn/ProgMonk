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

