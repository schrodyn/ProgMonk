
JavaScript and jQuery are used to make web pages interactive.

to include JavaScript files in HTML, we use the script element.

```html
<!DOCTYPE html>
<html>
  <head>
    <link href="style.css" rel="stylesheet">
  </head>
  <body>
    ...

    <script src="jquery.min.js"></script>
    <script src="app.js"></script>
  </body>
</html>
```

```html
<!DOCTYPE html>
<html>
  <body>
    ...

    <script src="https://goo.gl/1yIJUX">
    </script>

    <script src="https://goo.gl/fxp6xh">
    </script>

  </body>
</html>
```


The code starts with a JavaScript function called main().
A function is a set of instructions that tells the computer
to do something.

```js
var main = function() {
  $('.dropdown-toggle').click(function() {
    $('.dropdown-menu').toggle();
  });
}

$(document).ready(main);
```

The code then uses jQuery to run the main() function once the web page has
fully loaded.

First, the code selects the `<a class="dropdown-toggle">..</a>` element.

jQuery uses selectors to target HTML elements.
jQuery selectors are based on CSS selectors.


The `$(document).ready()` waits for the HTML document to load completely before
running the main() function.

This is important because JavaScript should only run after the web page
has loaded completely in the browser - otherwise there wouldn't be any HTML
elements to add interactivity to.


##Summary

JavaScript is a programming language used to add interactivity to a web page.
jQuery simplifies JavaScript syntax and makes it easier to build interactive web
pages that work across multiple browsers.

jQuery enables us to do three main things on a web page:

  1. Events. Respond to user interactions.
  2. DOM Manipulation. Modify HTML elements on the page.
  3. Effects. Add animations.

```js
var main = function(){
    $('.icon-menu').click(function() {
        $('.menu').animate({
            left: '0px'
        }, 200);

        $('body').animate({
            left: '285px'
        }, 200);
    });
}

$('document').ready(main)
```

##Close Menu Icon

```js
var main = function(){
    $('.icon-menu').click(function() {
        $('.menu').animate({
            left: '0px'
        }, 200);

        $('body').animate({
            left: '285px'
        }, 200);
    });

    $('.icon-close').click(function(){
        $('.menu').animate({
            left: '-285px'
        }, 200);
        $('body').animate({
            left: '0px'
        }, 200);
    });
}

$('document').ready(main)
```


#JavaScript

##Variables

```js
var counter = 140;
```

##Some Math:

```js
var score1 = 23;
var score2 = 25;
var score3 = 21;

var sum = score1 + score2 + score3;
var average = sum / 3;

var range = score2 - score3;

var fraction = 23 / 30;
var percentage = fraction * 100;
```

Variables can also store pieces of text, called strings. A string is written by
surrounding text with quotes.

Single quotes (' ') or double quotes (" ") can be used to write a string.

```js
var tweet = "Hiking trip on Saturday";
```

We can find out how long strings are using the .length method.

```js
var tweet = "Hiking trip on Saturday";
var tweetLength = tweet.length;
```

It's possible to compare variables using comparisons.

```
> - Greater than
< - Less than
>= Greater than or equal to
<= Less than or equal to
=== Equal to
!== Not equal to
```

```js
var score1 = 23;
var score2 = 6;
var counter = 140;
var tweet = "Hiking trip on Saturday";

score1 > score2;
tweet.length <= counter;
score1 === score2;
```

```js
var myName = "Jordan";
var status = "";

if( myName.length >= 7 ) {
    status = "You have a long name!";
}
```

ifelse:

```js
if( myName.length >= 7 ) {
    status = "You have a long name!";
}
else {
    status = "You have a short name!";
}
```

elseif:

```js
if( myName.length >= 15 ) {
    status = "You have a very long name!";
}
else if( myName.length >= 7 ) {
    status = "You have a long name!";
}
else {
    status = "You have a short name!";
}
```

##Functions

```js
var percentage = function(num, denom) {
    var result = (num / denom) * 100;
    return result;
};
```

The name of this function is percentage

```js
var p1 = percentage(23, 30);
```

##Events

User interactions with a web page are called events.
For example, when you click the Like button

We can write a function that specifies what to do when an event occurs.
This function is called an event handler.

```js
var main = function() {
  $(".like-button").click(function() {
    $(this).toggleClass("active");
  });
};

$(document).ready(main);
```

When a user clicks the .like-button element, the event handler toggles the
"active" class.

This alternates the button between the normal gray view and the selected blue
view.

```js
$('.social li').click(function() {
  $(this).toggleClass('active');
});
```

The `.click()` method attaches an event handler to each button.

The event handler adds the CSS class ".active", which changes the button's
background-color to red.

We use $(this) to refer to the HTML element that was clicked on.

##.keypress()

A keypress event occurs when a user types a key on the keyboard.

```js
$(document).keypress(function() {
  $('.dropdown-menu').toggle();
});
```

$(document) selects the whole web page.
The .keypress() method attaches an event handler to document
When any keyboard key is pressed, the event handler toggles the dropdown menu.

##Summary:

when you press any keyboard key, a keypress event is triggered by the browser.
When the keypress event occurs, the .keypress() method runs the event handler.

Triggering a keypress event handler when any key is typed isn't particularly
useful. For more control, we can use the "event" object.

The "event" object contains more information about an event, such as which mouse
button was clicked or which key was pressed.

```js
$(document).keypress(function(event) {
  if(event.which === 109) {
    $('.dropdown-menu').toggle();
  }
});
```

The event handler takes event as a parameter.
event.which contains which key was pressed. Keyboard keys are identified by
key codes. The m key is identified by the key code 109.

```html
<div class="article">
  <div class="item">
    ...
  </div>

  <div class="description">
    ...
  </div>
</div>
```

In jQuery $(this) refers to the current element or object selected.

children() allows you to search through elements nested directly under the
current element.

show() is jQuery's way of saying list or display these.

```js
var main = function(){
    $('.article').click(function(){
        $(this).children('.description').show()
    });
}

$(document).ready(main)
```

When an article is clicked, a click event occurs. This runs the event handler,
which shows the clicked article's description.

However, when you click on more articles in the newsfeed, all descriptions remain
 open.
To Hide the descriptions of all articles except the article we clicked:
On the first line of the event handler, add a line that selects the 'description' class and hides it using .hide().

```js
var main = function(){
    $('.article').click(function(){
        $('.description').hide();
        $(this).children('.description').show()
    });
}

$(document).ready(main)
```

To make more clear which article is currently open, let's add the class 'current'
to it. This is a CSS class in style.css that will shade it gray to indicate it is
open.

```js
var main = function(){
    $('.article').click(function(){
        $('.description').hide();
        $('.article').removeClass('.current')
        $(this).children('.description').show()
        $(this).addClass('.current')
    });
}

$(document).ready(main)
```

##Adding Keyboard Handler

```js
var main = function(){
    $(document).keypress(function(event){
       if(event.which === 111){
           $('.current').children('.description').toggle();
       }
    });

    $('.article').click(function(){
        $('.description').hide();
        $('.article').removeClass('.current')
        $(this).children('.description').show()
        $(this).addClass('.current')
    });
}

$(document).ready(main)
```

##Select New Article by pressing "n"

```js
var main = function(){
    $(document).keypress(function(event){
       if(event.which === 111){
           $('.current').children('.description').toggle();
       }else if(event.which === 110){
          var currentArticle = $('.current');
          var nextArticle = currentArticle.next();
          currentArticle.removeClass('current');
          nextArticle.addClass('current');
       }
    });

    $('.article').click(function(){
        $('.description').hide();
        $('.article').removeClass('.current')
        $(this).children('.description').show()
        $(this).addClass('.current')
    });
}

$(document).ready(main)
```

##DOM Manipulation

In jQuery, we often see $( ). It has two main uses:

  1. To select existing HTML elements on the page.
  `$('p')` selects all p elements on the page.

  2. To create new HTML elements to add to the page.
  `$('<h1>')` creates a new h1 element. The `< >` indicates that we want to create a
  new HTML element.

##.text()

The .text() method adds text to an HTML element.

The .appendTo() method adds HTML elements to the end of the selected element.

```js
$('.btn').click(function() {
  $('<li>').text('New item').appendTo('.items');
});
```

The li element is added as the last item inside <ul class="items"> .. </ul> on
the page.

The .prependTo() method adds HTML elements to the beginning of the selected
element.

The .remove() method removes the selected HTML element from the web page.

```js
$('.btn').click(function() {
  $('.selected').remove();
});
```

These six jQuery methods let you control more aspects of HTML elements:

.hide() hides the selected HTML element
.show() displays an element
.toggle() alternates hiding and showing an element
.addClass() adds a CSS class to an element
.removeClass() removes a class from an element
.toggleClass() alternates adding and removing a class from an element

```js
$('.social li').click(function() {
  $(this).toggleClass('active');
});
```

HTML elements are nested inside other elements, forming a hierarchy.
This hierarchy can be visualized as a tree.

This representation of HTML is called the Document Object Model (DOM).

##.next()
The .next() method gets the next sibling of the selected element.



##.prev()

The .prev() method gets the previous sibling of the selected element.

##.children()

The .children() method gets the children of the selected element.
If provided a selector, the .children() method can get a specific child.


We want to store what is typed in the status-box in this variable. How? Select
the class status-box. Use .val() after status-box is selected. This .val() is like
 .text() but for form inputs.

To clear out the status box, use jQuery to select the 'status-box' class.
Then set its value to empty by using .val('');.

 The .keyup() is similar to .keypress(). Therefore, this function is called a
keyup event handler.

##.slideDown()

The .slideDown() method shows the selected HTML element by sliding it down

```js
$('body').click(function() {
  $('.slide').slideDown(600).addClass('active-slide');
});


var main = function() {
  $(".btn").click(function(event) {
    $(".container").hide().slideDown(800);
  });
};

$(document).ready(main);
```

##.slideUp()

The .slideUp() method hides the selected HTML element by sliding it up.

```js
$('body').click(function() {
  $('.slide').slideUp(600).addClass('active-slide');
});
```

##.fadeIn()

The .fadeIn() method shows the selected HTML element by fading it in.

```js
$('body').click(function() {
  $('.slide').fadeIn(600).addClass('active-slide');
});
```

##.fadeOut()

The .fadeOut() method hides the selected HTML element by fading it out.

```js
$('body').click(function() {
  $('.slide').fadeOut(600).addClass('active-slide');
});
```

##.animate()

The .animate() method lets you create your own custom animations.
HTML elements can be animated by specifying the CSS properties to change to over
a time duration.


```js
$('.icon-menu').click(function() {
  $('.menu').animate({
      width: "193px"
    },
    300);
});
```

.animate() takes two parameters:
  1. A set of CSS properties,
  2. A time duration over which to change them.

Here, we want to change the width of the .menu element from its current width to
193px.

This change will occur over a time duration of 300 milliseconds.

```js
$('.arrow-next').click(function(){
        var currentSlide = $('.active-slide');
        var nextSlide = currentSlide.next();

        currentSlide.fadeOut(600).removeClass('active-slide');
        nextSlide.fadeIn(600).addClass('active-slide');
    });
```

to check last slide to move on the first:

```js
 if(nextSlide.length === 0){
            nextSlide = $('.slide').first();
        }
```
