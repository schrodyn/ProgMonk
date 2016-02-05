#Moving Around

Let's see a few examples of how Vim makes this fast.

  * Want to move the cursor to the next word? Press `w`.
  * Want to move to the next paragraph? Press `}`.
  * Want to move to the 3rd occurrence of the letter 'h'? Press `3fh`.
  * Want to move 35 lines downwards? Press `35j`.
  * After one of the above movements, want to jump back to the previous 
	location? Press `ctrl-o`

#Move your cursor

The most basic keys that you should use are the 'hjkl' keys.

Remember, that the right hand fingers should always be placed on `jkl;`

Note that we can repeat the operation by prefixing a count. For example, 
`2j` will repeat the `j` operation 2 times.

Key `^` -- moves to the end of the line

Key `$` -- moves to the start of the line

`ctrl-b` -- which means move one screen 'b'ackward

`ctrl-f` -- which means move one screen 'f'orward

If you know the absolute line number that you want to jump to, say line 50, press 50G and Vim will jump to the 50th line. If no number is specified,G will take you to the last line of the file. How do you get to the top of
the file? Simple, press 1G. Notice how a single key can do so much.

What if you wanted to the middle of the text that is currently being shown
in the window?

  * Press `H` to jump as 'h'igh as possible (first line of the window)
  * Press `M` to jump to the 'm'iddle of the window
  * Press `L` to jump as 'l'ow as possible (last line being displayed)

#Words, sentences, paragraphs

Want to move to the next 'w'ord? Press `w`.

To move one word 'b'ackward, press `b`. By prefixing a count, `2b` will go back by 2 words.

To move to the next sentence, press `)`.

To move to the previous sentence, press `(`.

Go ahead, try it out and see how fast you can move. Again, you can prefix 
a count such as `3)` to move forward by 3 sentences.

Now, use the whole text and try out moving by paragraphs. Press `}` to move to the next paragraph and `{` to move to the previous paragraph.

Notice that the 'bigger' brackets is for the bigger text object. If you 
had already noticed this, then congratulations, you have already started 
to think like a winner, err, "think like a Vimmer".

#Make your mark

Use `ma` to create a mark named 'a'.

Press `'a` (i.e. single quote followed by the name of the mark) and voila, Vim jumps (back) to the line where that mark was located.

You can use any alphabet (a-zA-Z) to name a mark which means you can have up to 52 named marks for each file.

#Jump around

In the various movements that we have learned, we might often want to jump back to the
previous location or to the next location after a movement. To do this, 
simply press `ctrl-o` to jump to the previous location and `ctrl-i` to 
jump forward to the next location again.

#Parts of the text

Press `v` to start the visual mode, and press `ap` to select 'a' 'p'aragraph. Press `~` to flip the case of the text. If you want to cancel the selection, simply press `<Esc>`.

Other text object mnemonics are `aw` which means 'a' 'w'ord, a" means a 
quoted string (like "this is a quoted string"), ab means 'a' 'b'lock which
means anything within a pair of parentheses, and so on.
