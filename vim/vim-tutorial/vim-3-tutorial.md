#Vim Modes

There are three basic modes in Vim - normal, insert and visual.

  * Normal mode is where you can run commands. This is the default mode 
  in which Vim starts up.

  * Insert mode is where you insert i.e. write the text.

  * Visual mode is where you visually select a bunch of text so that you 
	can run a command/operation only on that part of the text.

#Normal mode

Type `:echo "hello world"` and press enter. You should see the famous 
words `hello world` echoed back to you.

What you just did was run a Vim command called `:echo` and you 
supplied some text to it which was promptly printed back.

Type `/hello` and press the enter key. Vim will search for that phrase 
and will jump to the first occurrence.

`:help usr_toc` -- takes us to the table of contents of the reference 
manual. You can search here, for example, run `/insert mode` to see
the relevant information regarding insert mode.

#Insert Mode

To switch to insert mode you can press `i` or

Press `I` -- it moves the cursor to the start of the line and then 
switches to the insert mode.

Now, let's take a different variation of the `i` command. Notice that 
pressing `i` will place your cursor before the current position and 
then switch to insert mode. To place the cursor 'a'fter the current 
position, press `a`.

Similar to the relationship between `i` and `I` keys, there is a 
relationship between the `a` and `A` keys - if you want to append text 
at the end of the line, press the `A` key.


Now that we are proficient in quickly moving in the current line, let's 
see how to move to new lines. If you want to 'o'pen a new line to start 
writing, press the `o` key:

  * Press `o`
  * Type `I'm a rapper.`
  * Press `<Esc>` to switch back to the normal mode.

Press `O` (upper case 'O') -- to insert ("open") a new line above

To summarize keys we have learnt so far:

  * `i` -- insert text just before the cursor
  * `I` -- insert text at the start of the line
  * `a` -- append text just after the cursor
  * `A` -- append text at the end of the line
  * `o` -- open a new line below
  * `O` -- open a new line above

Press `b` -- to move 'b'ack to the start of the word

Press `s` -- for 's'ubstitute character under the cursor and stay in 
insert mode

Again, there is a bigger version of the `s` key, `S` which substitutes 
the whole line instead of the current character.

Press `r` -- to 'r'eplace just a single character and back in the normal
mode

There's a bigger version of `r` called `R` which will replace continuous 
characters.

#Visual mode

Suppose that you want to select a bunch of words and replace them 
completely with some new text that you want to write. What do you do?

The most efficient way would be to position the cursor at the start of 
the text, press `v` to start the visual mode, use arrow keys or any text 
movement commands to the move to the end of the relevant text 
(for example, press `5e` to move to the end of the 5th word counted
from the current cursor position) and then press `c` to 'c'hange the text. 
Notice the improvement in efficiency.

The `v` command works on a character basis. If you want to operate in terms of lines, use the upper case `V`.
 
