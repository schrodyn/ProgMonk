
A div element groups other elements together into sections of the web page,
such as a navigation bar, the main body, and the footer.

```html
<!DOCTYPE html>
<html>
  <body>
  <div class="nav">
    <div class="container">
	    <ul>
	        <li>Flynston</li>
	        <li>Browse</li>
	    </ul>
	    <ul>
	        <li>Sign Up</li>
	        <li>Log In</li>
	        <li>Help</li>
	    </ul>
    </div>
  </div>    

  </body>
</html>
```

```css
h1 {
  color: red;
}
```

This code is called a CSS rule.

A CSS rule starts with a selector. (h1)
Inside the braces { }, a property and its value define what aspect of the h1
elements to style.

HTML elements can be CSS selectors
Another available selector is the class selector.

```html
<div class="header">
  <h2>Heading</h2>
  <p>Subtitle</p>
</div>

<p> Hello world</p>
```

The class can be targeted from CSS by using a dot (.)

```css
.header {
  color: blue;
}
```

The .header selector applies a blue text color only to the elements nested inside
`<div class="header">..</div>`

It's possible to be even more specific by combining classes and element names.

```css
.header p {
  color: blue;
}
```

##color

The color property sets the color of an HTML element's text.
We can use color names to change the text's color. But this only works for 140
colors.

Instead, we can use RGB values or hexadecimal numbers.

```css
.jumbotron h1 {
  color: rgb(102,153,0);
}

.jumbotron h1 {
  color: #9933CC;
}
```

##font-family

The font-family property sets the font of an HTML element's text.

```css
.jumbotron h1 {
  color: red;
  font-family: 'Shift', sans-serif;
}
```

##font-size

The font-size property sets the size of an HTML element's text.
Text size can be measured in pixels, ems, or rems. We will use pixels here.

```css
.jumbotron h1 {
  color: red;
  font-family: 'Shift', sans-serif;
  font-size: 48px;
}
```

##background-color

The background-color property sets the color for the background of an HTML
element.

```css
.jumbotron {
  background-color: #0099cc;
}
```

##background-image

Instead of a solid color, the background-image property sets an image as the
background of an HTML element.

```css
.jumbotron {
  background-image: url('https://goo.gl/04j7Nn');
}
```

##border

The border property sets the width, style, and color of an element's border

```css
.jumbotron h1 {
  border: 5px solid #cc0000;
}
```

##padding

The padding property creates space between the content and border of an element.
This whitespace is useful in order to improve readability

```css
.jumbotron h1 {
  padding: 23px;
  border: 3px solid #ffffff;
}
```

The padding property sets the padding on all sides. It's possible to set the
padding on each side.

The properties padding-top, padding-bottom, padding-left, and padding-right are
available to set the padding on each side of an HTML element.

```css
.jumbotron h1 {
  padding-top: 10px;
  padding-bottom: 23px;
  border: 3px solid #ffffff;
}
```

##margin

The margin property creates space for multiple HTML elements. The margin is a
transparent area outside the border of an element.

```css
.jumbotron h1 {
  margin: 14px;
  border: 3px solid #ffffff;
}
```

The margin property sets the margin on all sides. It's possible to set the margin
on each side.

The properties margin-top, margin-bottom, margin-left, and margin-right are
available to set the margin on each side of an HTML element.

```css
.jumbotron h1 {
  margin-top: 10px;
  margin-left: 23px;
  border: 3px solid #ffffff;
}
```

To center an element, set
`margin-right: auto` and `margin-left: auto`.

The margin to the left and right will be equal and the element will be centered.

```css
.jumbotron h1 {
  margin-left: auto;
  margin-right: auto;
  border: 3px solid #ffffff;
}
```

The <head> element contains information that the web browser needs to display the
page.

The <link> element tells the browser where to find the CSS file used to style the
page.

The <body> element contains the content of the page that shows up in the
web browser.


```css
.nav a {
  color: #5a5a5a;
  font-size: 11px;
  font-weight: bold;
  padding-top: 14px;
  padding-bottom: 14px;
  padding-left: 10px;
  padding-right: 10px;
  text-transform: uppercase;
}

.jumbotron {
    height: 500px;
    background-image: url('https://goo.gl/04j7Nn');
}

.jumbotron h1 {
    color: #fff;
    font-size: 48px;
    font-weight: bold;
}
```

Custom fonts can be loaded from CSS, like this:

```html
<!DOCTYPE html>
<html>
  <head>
     <link href="font.css" rel="stylesheet">
    <link href="main.css" rel="stylesheet">
  </head>
  ...
</html>
```

##display

CSS treats HTML elements like boxes. A box can be "block" or "inline".

Block elements display on a new line (e.g., h1, p, ul, li).
Inline elements display on the same line as their neighboring elements
(e.g., img, a)

It's possible to change whether a box is block or inline by using the display
property

```css
.nav li {
  display: inline;
}
```

##position

The position property is used to move an HTML element to a precise position on
the page.

```css
.jumbotron h1 {
  position: relative;
  top: 91px;
  left: 55px;
}
```

This will shift the h1 element 91px away from the top and 55px away from the left
of its original position on the page.

##float

The float property moves an element to the far left or far right of the page.

```css
img {
 float: right;
}
```

Therefore, we can select the <div class="container">..</div> inside
<div class="jumbotron">..</div> in CSS like this:

```css
.jumbotron .container {
  ...
}
```

Bootstrap is a collection of prewritten CSS rules used
to build web pages faster.

Bootstrap provides styles out of the box for several common components on a web
page, such as grid layouts, navigation, showcases, and much more.

Bootstrap comes with a grid that is made up of 12 equal-sized columns.
HTML elements are arranged to span different numbers of columns in order to create
 custom page layouts.

```html
<div class="row">

  <div class="col-md-6">
    <h4>Content 1</h4>
  </div>

  <div class="col-md-6">
    <h4>Content 2</h4>
  </div>

</div>
```

Here each piece of content spans six columns, so it uses the .col-md-6 class.
Both columns are wrapped in a .row class to create a horizontal group.

```html
<div class="row">

	<div class="col-md-2">
		<h4>Content 1</h4>
	</div>

	<div class="col-md-8">
		<h4>Content 2</h4>
	</div>

	<div class="col-md-2">
		<h4>Content 3</h4>
	</div>

</div>
```

##Tabs

Tabs are a common navigation technique that give links to different parts of a
site.

Bootstrap makes it easy to create tabbed navigation. Let's see how to do this.
To do so, make the class equal nav nav-tabs.

```html
<ul class="nav nav-tabs">
	<li><a href="#">Primary</a></li>
	<li class="active"><a href="#">Social</a></li>
	<li><a href="#">Promotions</a></li>
	<li><a href="#">Updates</a></li>
</ul>
```

The open tab is specified by the class active.

```html
<ul class="nav nav-tabs">
	<li><a href="#">Primary</a></li>
	<li><a href="#">Social</a></li>
	<li><a href="#">Promotions</a></li>
	<li  class="active"><a href="#">Updates</a></li>
</ul>
```

##Pills

Pills are a set of buttons that give links to different parts of a site.

To change the tab navigation to pill navigation, replace the class nav-tabs with
nav-pills

```html
<ul class="nav nav-pills">
	<li><a href="#">Primary</a></li>
	<li class="active"><a href="#">Social</a></li>
	<li><a href="#">Promotions</a></li>
	<li><a href="#">Updates</a></li>
</ul>
```

##Jumbotron

Many sites have a large showcase area featuring important content.
Bootstrap calls this large showcase a jumbotron.

```html
<div class="jumbotron">
	<h1>Find a place to stay.</h1>
	<p>Rent from people in over 34,000 cities and 192 countries.</p>
</div>
```

The order of the CSS files tell the browser that your styles in main.css
are more important than the styles in bootstrap.css,
so the final page that loads is the customized web page.

Bootstrap provides the .pull-left class to left-align an element, and the
.pull-right class to right-align an element.

```html
<div class="nav">
      <div class="container">
        <ul class="pull-left">
          <li><a href="#">Name</a></li>
          <li><a href="#">Browse</a></li>
        </ul>
        <ul class="pull-right">
          <li><a href="#">Sign Up</a></li>
          <li><a href="#">Log In</a></li>
          <li><a href="#">Help</a></li>
        </ul>
      </div>
</div>
```

Bootstrap provides the ".thumbnail" class to easily display grids of images.
Here's how to create a thumbnail:

```html
<div class="thumbnail">
  <img src="..." >
</div>
```

An img element is wrapped in a div element with the class "thumbnail".

```html
<div class="row">
                <div class="col-md-4">
                    <div class="thumbnail">
                        <img src="http://goo.gl/0sX3jq">
                    </div>
                    <div class="thumbnail">
                        <img src="http://goo.gl/an2HXY">
                    </div>
                </div>
                <div class="col-md-4">
                    <div class="thumbnail">
                        <img src="http://goo.gl/Av1pac">
                    </div>
                    <div class="thumbnail">
                        <img src="http://goo.gl/vw43v1">
                    </div>
                </div>
                <div class="col-md-4">
                 <div class="thumbnail">
                        <img src="http://goo.gl/0Kd7UO">
                    </div>
                </div>
</div>
```
