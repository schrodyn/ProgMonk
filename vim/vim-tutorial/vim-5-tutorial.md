#Editing Basics

Let's learn the basic editing commands in Vim for reading/writing files, 
cut/copy/paste, undo/redo and searching.

##Buffers

Buffers
When you edit a file, Vim brings the text in the file on the hard disk to the computer's RAM. 
This means that a copy of the file is stored in the computer's memory and any changes you make 
is changed in the computer's memory and immediately displayed. Once you have finished editing 
the file, you can save the file which means that Vim writes back the text in
the computer's memory to the file on the hard disk. The computer memory used here to
store the text temporarily is referred to as a "buffer".

##Swap

Now you will notice that another file has been created in the same directory as this file, the
file would be named something like `.hello.txt.swp`. Run the following command to find
out the exact name of the file:

```
:swapname
```

t is this file? Vim maintains a backup of the buffer in a file which it saves regularly to
the hard disk so that in case something goes wrong (like a computer crash or even Vim
crashes), you have a backup of the changes you have made since you last saved the original
file. This file is called a "swap file" because Vim keeps swapping the contents of the buffer
in the computer memory with the contents of this file on the hard disk.

##Save my file

We can write back the buffer to the original file in the hard disk by running:

```
:write
```

To make saving easier, add the following line to your vimrc file:

```
" To save, ctrl-s.
nmap <c-s> :w<CR>
imap <c-s> <Esc>:w<CR>a
```
Now you can simply press `ctrl-s` to save the file.

#Cut, Copy and Paste

desktop world | vim world | operation
-------------|------------|-----------
cut			|		delete | d
copy 		| 		yank  | y
paste		| 		paste | p

In normal desktop terminology, 'cut'ting text means removing the text and putting it into
the clipboard. The same operation in Vim means it deletes the text from the file buffer and
stores it in a 'register' (a part of the computer's memory). Since we can choose the register
where we can store the text, it is referred to as the "delete" operation.

Similarly, in normal desktop terminology, 'copy' text means that a copy of the text is placed
on the clipboard. Vim does the same, it "yanks" the text and places it in a register.
"Paste" means the same in both terminologies.

We have seen how you can do cut/copy/paste operations in Vim. But how do you specify
which text that these operations should work on? Well, we have already explored that in the
previous Text Objects section.

Combining the operation notation and the text object notation means we have innumerable ways of manipulating the text.

Key `|` -- moves to the first column respectively

So, we have to just press `dl` and we delete one character! 

press `dw` and you delete a word.

How do we achieve the same operation for lines?

if you repeat the operation name twice, it will operate on the line. So, 
`dd` will delete the current line and `yy` will yank the current line.

Let's see a bigger case: How do we yank the current paragraph?

Operation | Text Object/Motion
----------|--------------------
Yank	|		Current paragraph
y		| 		ap

So, `yap` will copy the current paragraph.

Now that we have done copying the text, how do we paste it? Just 'p' it.

Notice that the blank line is also copied when we do yap, so `p` adds that extra blank line as well.

There are two types of paste that can be done exactly like the two types 
of inserts we have seen before:

		|
--------|-----------
p(lower case) | paste after current position
P(upper case) | paste before current position

Taking the idea further, we can combine these into more powerful ways.

How to swap two characters? Press `xp`.

  * x -> delete one character at current cursor position
  * p -> paste after current cursor position
  
How to swap two words? Press `dwwP`.

  * d -> delete
  * w -> one word
  * w -> move to the next word
  * P -> paste before the current cursor position

The important thing is not to learn these operations by rote. These combinations of
operations and text objects/motions should be automatic for your fingers, without you needing to put in mental effort. This happens when you make using these a habit.

Press `S` to 's'ubstitute the whole line.

#Time machine using undo/ redo

Press `u` to undo, and `ctrl-r` to redo changes. 

It is important to note that Vim gives unlimited history of undo/redo changes, but it is
usually limited by the `undolevels` setting in Vim and the amount of memory in your
computer.

Now, let's see some stuff that really shows off Vim's advanced undo/redo functionality,
some thing that other editors will be jealous of: Vim is not only your editor, it also acts as a
time machine.

For example,

```
:earlier 4m
```

will take you back by 4 minutes i.e. the state of the text 4 minutes "earlier".

The power here is that it is superior to all undoes and redoes. For example, if you make a
change, then you undo it, and then continue editing, that change is actually never
retrievable using simple u again. But it is possible in Vim using the `:earlier` command.

You can also go forward in time:

```
:later 45s
```

Or if you want the simpler approach of going back by 5 changes:

```
:undo 5
```

You can view the undo tree using:

```
:undolist
```

#Search engine

Suppose we want to search for the word "Step". In normal mode, type `/Step<cr>` (i.e.
`/Step` followed by enter key). This will take you to the first occurrence of those set of
characters. Press `n` to take you to the 'n'ext occurrence and `N` to go in the opposite
direction i.e. the previous occurrence.

What if you knew only a part of the phrase or don't know the exact spelling? Wouldn't it be helpful if Vim could start searching as and when you type the search phrase? You can enable this by running:

```
set incsearch
```

You can also tell Vim to ignore the case (whether lower or upper case) of the text that you are searching for:

```
set ignorecase
```

or you can use

```
set smartcase
```

When you have smartcase on:

  * If you are searching for /step i.e. the text you enter is in lower 
    case, then it will search for any combination of upper and lower case 
	text. For example, this will match all the following four - "Step", 
	"Stephen", "stepbrother", "misstep."
  
  * If you are searching for `/Step` i.e. the text you enter has an upper 
  case, then it will search for ONLY text that matches the exact case. For   example, it will match "Step" and

Now that we have understood the basics of searching, let's explore the real power of
searching. The first thing to note that what you provide Vim can not only be a simple
phrase, it can be a "expression". An expression allows you to specify the 'kinds' of text to search for, not just the exact text to look.

For example, you will notice that `/step` will take you to steps as well as step and even
footstep if such a word is present. What if you wanted to look for the exact word step and
not when it is part of any other word? Then you can search using `/\<step\>`. The `\<` and `\>` indicate the start and end positions of a "word" respectively.

Similarly, what if you wanted to search for any number? Searching for `/\d` will look for a
'digit'. But a number is just a group of digits together. So we specify "one or more" digits
together as `/\d\+`. If we were looking for zero or more characters, we can use the `*` instead of the `+`.

There are a variety of such magic stuff we can use in our search patterns.
See `:help pattern` for details.


