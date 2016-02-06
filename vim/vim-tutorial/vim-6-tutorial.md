#Multiplicity

In this chapter, let's explore how Vim helps us work between different parts of the file,
different files, different 'windows' and different tabs to help us to handle more
simultaneously. After all, an important part about good editing is managing files.

#Multiple Buffers

Suppose you wanted to edit more than one file at a time using the same Vim, what do you do?
Remember that files are loaded into buffers in Vim. Vim can also load multiple buffers at
the same time. So, you can have multiple files open at the same time and you can keep
switching between them.

Let's say you have two files, part1.txt and part2.txt

Now, run `:e part1.txt` and then run `:e part2.txt`. Observe that you have the second file
now open for editing. How do you switch back to the first file? In this particular case, you
can just run `:b 1` to switch to 'b'uffer number '1'. You can also run `:e part1.txt` to open the existing buffer into view.

You can see what buffers has been loaded and correspondingly, which files are being edited
by running `:buffers` or a shorter form, `:ls` which stands for 'l'i's't buffers.

Buffers will be automatically removed when you close Vim, so you don't have to do anything
special other than making sure you save your files. However, if you really want to remove a
buffer, for example in order to use less memory, then you can use `:bd 1` to 'd'elete the
'b'uffer numbered '1', etc.

#Multiple Tabs

If you use Firefox, you might have used the tabs feature which allows you to open multiple websites in a single Firefox window so that you can 
switch between them without having the headache of switching between 
multiple windows. Well, tabs work exactly the same way in Vim also. 
Except that they are called "tab pages."

Run `:tabnew` to open a new tab with
a new buffer (analogous to `:new`).
How do you switch between the
tabs? By pressing `gt` to 'g'o to the
next 't'ab and `gT` to 'g'o in the
opposite direction i.e. the previous
't'ab.

I personally prefer to use the keys
`ctrl-h` and `ctrl-l` for the same
operations analogous to how the `h`
and `l` keys work for characters.
To enable this, add the following lines to
your vimrc file:

```
" Shortcuts for moving between tabs.
" Ctrl-h to move to the tab to the left
noremap <C-h> gT
" Ctrl-l to move to the tab to the right
noremap <C-l> gt
```

To 'c'lose a 'tab', run `:tabc` or `:q`.


