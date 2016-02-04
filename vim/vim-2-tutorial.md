#First Steps

Vim has modes. For example, Vim has a mode for writing text, a mode for
running commands, etc. They are all related to the main purpose of 
editing text, but you switch context on whether you want to simply type 
stuff or you want to run some commands

So why does Vim have modes?

Let's take an example - one of the key goals in Vim is to make it
fully accessible from the keyboard without ever having to need to use a 
mouse. In such a scenario, how would you distinguish between the text 
you want to write, and the commands that you want to run?

Vim's solution is to have a "normal" mode where you can execute commands 
and an "insert" mode where you are simply writing text. You can switch 
between the two modes any time.
For example, pressing `i` switches Vim to insert mode, and pressing 
`<Esc>` switches Vim back to normal mode.

Suppose you want to change all occurrences of the word "from" to the 
word "to" in a document. In Vim you simply run `:%s/from/to/g` in the 
normal mode. The `:s` is the "substitute" command.

What if you want to now run this substitution only in the first 10 lines 
of the text and you want to have a yes/no confirmation for each 
replacement?

In Vim, you can simply run `:0,10s/from/to/gc`. The new c option we are
using means we want a 'c'onfirmation message for every replace.

By separating the writing (insert) and command (normal) modes, Vim makes 
it easy for us to switch the two contexts easily.

**Note**
All commands in the normal mode should end with the enter key to signal 
Vim that we have written the full command.

#Open a file

  * Type `:edit hello.txt` or simply `:e hello.txt`
  
  * Press `i` -- This switches Vim to the insert mode
  
  * Type the text
  
  * Press `<Esc>` -- This escapes Vim back to normal mode
  
  * `:write` or simply `:w` -- write to the hard disk

  * `:quit` or simply `:q` -- to quit the file 


`:help` -- to get help. For example, run `:help :edit`






