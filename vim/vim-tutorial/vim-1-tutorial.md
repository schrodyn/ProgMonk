#About Vim

Vim is a computer program used for writing, and it provides a range 
of features that help you **write better**.

#What can Vim do?

Let's see some random examples to compare Vim with your current 
choice of editor.

How do you move the cursor down by 7 lines? 
	
```
Press 7j
```

How do you delete a word? Yes, a "word".

```
Press dw
```

How do you search the file for the current word that the
cursor is currently placed on?

```
Press *
```

How to do a find and replace only in lines 50-100? 

```
Run :50,100s/old/new/g
```

What if you wanted to view two different parts of the same file 
simultaneously?

```
Run :sp to 'split' the view
```

What if you wanted to choose a better color scheme of the display?

```
Run `:colorscheme desert` to choose the 'desert' color scheme
```

What if you wanted to map the keyboard shortcut `ctrl-s` to save 
the file?

```
Run `:nmap <c-s> :w<CR>`. Note that <CR> means a `c`arriage `r`eturn 
i.e. the enter key
```

What if you wanted to save the current set of open files
as well as any settings you have changed, so that you
can continue editing later? 

```
Run `:mksession ~/latest_session.vim`, and open Vim next time as 
`vim -S ~/latest_session.vim`
```

What if you wanted to see colors for different parts of your code?

```
Run `:syntax on`
```

What if you wanted to open multiple files in tabs?

```
Use `:tabedit <file>` to open multiple files in "tabs" (just like 
browser  tabs), and use `ctrl-pgup/ctrl-pgdn` to switch between the tabs.
```

You use some words frequently in your document
and wish there was a way that it could be quickly filled
in the next time you use the same word?

```
Press `ctrl-n` and see the list of "completions" for the current word, 
based on all the words that you have used in the current document.
```

You have some data where only the first 10 characters in
each line are useful and the rest is no longer useful for
you. How do you get only that data?

```
Press `ctrl-v`, select the text and press `y` to copy the selected rows 
and columns of text.
```

What if you received a document from someone
which is in all caps, find it irritating and want to convert
it to lower case?

In Vim, run the following:

```
:for i in range(0, line('$'))
: call setline(i,
tolower(getline(i)))
:endfor
```

In these examples, you can see the power of Vim in action. Any other 
editor would make it insanely hard to achieve the same level of 
functionality. And yet, amazingly, all this power
is made as understandable as possible.


