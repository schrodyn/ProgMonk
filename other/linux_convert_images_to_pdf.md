#How can I convert a series of images to a PDF from the command line on linux?

Using `imagemagick`, you can try:

```
convert page.png page.pdf
```

Or for multiple images:

```
convert page*.png mydoc.pdf
```


