Markdown is a way to write content for the web.

It’s written in what nerds like to call “plaintext”

Plaintext is just the regular alphabet, with a few familiar symbols, 
like asterisks ( * ) and backticks ( ` ).

We'll start by learning two basic elements in text formatting: italics and bold.

To make a phrase italic in Markdown, you can surround words with an underscore 
(_ ). For example, _this_ word would become italic.

Or use single asterisks to *italicize* text.

Similarly, to make phrases bold in Markdown, you can surround words with two 
asterisks ( ** ). This will **really** get your point across.

 to make some words bold and italic:
In general, it doesn't matter which order you place the asterisks or 
underscores.
Place the asterisks **_on the outside_**, just to make it more legible.

----------------------

Let's take a look at another formatting convention: the header.
As their name implies, they act like titles or subtitles above sections.

There are six types of headers, in decreasing sizes

To make headers in Markdown, you preface the phrase with a hash mark (#).

You place the same number of hash marks as the size of the header you want.
For example, for a header one, you'd use one hash mark (# Header One), 
while for a header three, you'd use three (### Header Three).

You can't really make a header bold, but you can italicize certain words.

for example:
####Colombian Symbolism in _One Hundred Years of Solitude_


-----------------------

We'll now learn how to make links to other web sites on the world wide web.

There are two different link types in Markdown
The first link style is called an inline link.
To create an inline link, you wrap the link text in brackets ( [ ] ), and then 
you wrap the link in parenthesis ( ( ) ).

For example, to create a hyperlink to www.github.com, with a link text that says
, Visit GitHub!, you'd write this in Markdown:
[Visit GitHub!](www.github.com)

You can add emphasis to link texts, if you like.
for example:
[You're **really, really** going to want to see this.](www.dailykitten.com)

you can make links within headings, too.
####The Latest News from [the BBC](www.bbc.com/news)

to add an optional title:
This is [an example](http://example.com/ "Title") inline link.

If you’re referring to a local resource on the same server, you can use relative paths:

See my [About](/about/) page for details.   

That's all there is to writing inline links.

The other link type is called a reference link.
As the name implies, the link is actually a reference to another place in the 
document.

Here's an example of what we mean:
     Here's [a link to something else][another place].
     Here's [yet another link][another-link].
     And now back to [the first link][another place].

     [another place]: www.github.com
     [another-link]: www.google.com


An advantage of the reference link style is that multiple links to the same 
place only need to be updated once.

The following three link definitions are equivalent:

[foo]: http://example.com/  "Optional Title Here"
[foo]: http://example.com/  'Optional Title Here'
[foo]: http://example.com/  (Optional Title Here)

You now know how to make links in Markdown!

--------------------------

If you know how to create links in Markdown, you can create images, too. 
The syntax is nearly the same.

The difference is that an image is prefaced with an exclamation point ( ! ), 
followed by the same two brackets, and a pair of parentheses containing 
the image URL.

Images also have two styles, just like links. To create an inline image, you'll
use the same syntax as an inline link.

for example:
![A representation of Octdrey Catburn](http://octodex.github.com/images/octdrey-catburn.jpg)

You don't need to add alt text, but it's considered useful and polite to the 
visually impaired. You could just as well leave the boxes empty.

For a reference image, you'll follow the same pattern as a reference link. 
You'll precede the Markdown with an exclamation point, then provide two brackets
 for the alt text, and then two more for the image tag.

for example:

![The first father][First Father]
![The second first father][Second Father]

[First Father]: http://octodex.github.com/images/founding-father.jpg
[Second Father]: http://octodex.github.com/images/foundingfather_v2.png


![Funny cat](http://cats.ru/funny.png "Share this") --  on mouse over the image
 "Share this" will be displayed

Ta da! You've learned all there is to adding images in Markdown!

--------------------------

A blockquote is a sentence or paragraph that's been specially formatted to draw
attention to the reader. 

To create a block quote, all you have to do is preface a line with the "greater
than" caret (>). For example:

> In a few moments he was barefoot, his stockings folded in his pockets and his
  canvas shoes dangling by their knotted laces over his shoulders and, picking a
  pointed salt-eaten stick out of the jetsam among the rocks, he clambered down
  the slope of the breakwater.

You can also place a caret character on each line of the quote. This is 
particularly useful if your quote spans multiple paragraphs. 
For example:

> His words seemed to have struck some deep chord in his own nature. Had he 
spoken of himself, of himself as he was or wished to be? Stephen watched his 
face for some moments in silence. A cold sadness was there. He had spoken of 
himself, of his own loneliness which he feared.
>
> —Of whom are you speaking? Stephen asked at length.
>
> Cranly did not answer.

Notice that even blank lines must contain the caret character.

Block quotes can contain other Markdown elements, such as italics, images, or 
links.


Ta da! You've learned all there is to creating blockquotes in Markdown!
-------------------------------------

This tutorial is all about creating lists in Markdown.
There are two types of lists in the known universe: unordered and ordered.

To create an unordered list, you'll want to preface each item in the list with 
an asterisk ( * ).
Each list item also gets its own line.
For example, a grocery list in Markdown might look like this:

* Milk
* Eggs
* Salmon
* Butter

An ordered list is prefaced with numbers, instead of asterisks.
To write that in Markdown, you'd do this:

1. Crack three eggs over a bowl
2. Pour a gallon of milk into the bowl
3. Rub the salmon vigorously with butter
4. Drop the salmon into the egg-milk bowl

You can choose to add italics, bold, or links within lists, as you might expect.

for example, turn the latin names for the plants into italics.:
* Azalea (_Ericaceae Rhododendron_)
* Chrysanthemum (_Anthemideae Chrysanthemum_)
* Dahlia (_Coreopsideae Dahlia_)

Occasionally, you might find the need to make a list with more depth, 
or, to nest one list within another.

 All you have to do is to remember to indent each asterisk one space more than 
the preceding item.

for example:
* Tintin
 * A reporter
 * Has poofy orange hair
 * Friends with the world's most awesome dog
* Haddock
 * A sea captain
 * Has a fantastic beard
 * Loves whiskey
   * Possibly also scotch?

While you could continue to indent and add sub-lists indefinitely, it's usually
a good idea to stop after three levels; otherwise, your text becomes a mess.

Suppose you want to create a bullet list that requires some additional context 
(but not another list). For example:

1. Crack three eggs over a bowl.

 Now, you're going to want to crack the eggs in such a way that you don't make a mess.

 If you _do_ make a mess, use a towel to clean it up!

2. Pour a gallon of milk into the bowl.

 Basically, take the same guidance as above: don't be messy, but if you are, clean it up!

3. Rub the salmon vigorously with butter.

   By "vigorous," we mean a strictly vertical motion. Julia Child once quipped:
   > Up and down and all around, that's how butter on salmon goes.
4. Drop the salmon into the egg-milk bowl.

   Here are some techniques on salmon-dropping:

   * Make sure no trout or children are present
   * Use both hands
   * Always have a towel nearby in case of messes

Notice that the first two items have a single space. 
to make own paragraph you must indent one vertical space like this:

1. Cut the cheese
   
 Make sure that the cheese is cut into little triangles.

2. Slice the tomatoes
  
 Be careful when holding the knife.
 
 For more help on tomato slicing, see Thomas Jefferson's seminal essay 
_Tom Ate Those_.

---------------------------------------

Markdown has several ways of formatting paragraphs.

Let's take a few lines of poetry as an example. Suppose you want to write text 
that looks like this:

Do I contradict myself?
Very well then I contradict myself,
(I am large, I contain multitudes.)

You can accomplish this by inserting two spaces after each new line. 
This is not possible to see, since spaces are invisible, but it'd look 
something like this:

Do I contradict myself?··
Very well then I contradict myself,··
(I am large, I contain multitudes.)

Each dot ( · ) represents a space on the keyboard.

This is what's known as a soft break;

-----------------------------------------
MultiMarkdown Tutorials:

Horizontal lines are useful for breaking up the content in documents.

Type three or more dashes (- – -) between the paragraphs. You can also type 
three or more asterisks, but I find dashes are more associated with lines and 
thus easier to remember.

Hard Line Break

When you want to add a hard line break using Markdown, in some editors adding 
two or more spaces after the end of the line where you want to add the hard 
break should do the trick.

Strikethrough

To make strikethrough add two squiggly characters (~~strikethrough~~), or what 
is technically called tildes, on both sides of the last word in the sentence.

Footnotes

Footnotes are typically used in academic papers, but they can also be useful in other documents in which a list references appear at the end of the document, 
instead embedding links in the body of the document.

To type marker -- insert [^1] after word

and at the end of document add:
[^1]: Cupcake Ipsum is fun text.

[^2]: [Cupcake Ipsum](http://www.cupcakeipsum.com/#)

Each footnote must have a unique reference number, put into brackets followed 
by a colon.
 A horizontal line will automatically appear above the footnoted references.

Tables

Type the following titles for table, and separate each column with a vertical 
bar (|), or pipe.

Good Food | Bad Food

Next type multiple dashes under the titles, and add a vertical bar between 
them.

Good Food | Bad Food
--- | ---

Add items in each column by typing a vertical bar between column items.
Good Food | Bad Food

--- | ---

fruits | candy

carrots | potato chips

oily fish | fatty meat


--------------------------

to highlight text: ==highlighted text==
inline code: `code`

in github:

Fenced code blocks

```
function test() {
  console.log("notice the blank line before this function?");
}
```

Syntax highlighting

In your fenced block, add an optional language identifier

```ruby
require 'redcarpet'
markdown = Redcarpet.new("Hello World!")
puts markdown.to_html
```

Tables

You can create tables by assembling a list of words and dividing them with 
hyphens - (for the first row), and then separating each column with a pipe |:

First Header  | Second Header
------------- | -------------
Content Cell  | Content Cell
Content Cell  | Content Cell

For aesthetic purposes, you can also add extra pipes on the ends:

| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |

Note that the dashes at the top don't need to match the length of the header 
text exactly:

| Name | Description          |
| ------------- | ----------- |
| Help      | Display the help window.|
| Close     | Closes a window     |

You can also include inline Markdown such as links, bold, italics, or 
strikethrough:

| Name | Description          |
| ------------- | ----------- |
| Help      | ~~Display the~~ help window.|
| Close     | _Closes_ a window     |

Finally, by including colons : within the header row, you can define text to be
left-aligned, right-aligned, or center-aligned:

| Left-Aligned  | Center Aligned  | Right Aligned |
| :------------ |:---------------:| -----:|
| col 3 is      | some wordy text | $1600 |
| col 2 is      | centered        |   $12 |
| zebra stripes | are neat        |    $1 |

Escaping characters:

Escaping Markdown characters with a back-slash \ allows you to use any 
characters which might be getting accidentally converted into HTML.

Markdown provides backslash escapes for the following characters:

\   backslash
`   backtick
*   asterisk
_   underscore
{}  curly braces
[]  square brackets
()  parentheses
#   hash mark
+   plus sign
-   minus sign (hyphen)
.   dot
!   exclamation mark

Horizontal Rules:

You can produce a horizontal rule tag (<hr />) by placing three or more hyphens, asterisks, or underscores on a line by themselves. 

Each of the following lines will produce a horizontal rule:

* * *

***

*****

- - -

---------------------------------------



