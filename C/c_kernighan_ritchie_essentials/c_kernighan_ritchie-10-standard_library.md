#Standard Library

This appendix is a summary of the library defined by the ANSI standard.

#Input and Output: `<stdio.h>`

A *stream* is a source or destination of data that may be associated 
with a disk or other peripheral. The library supports text streams and 
binary streams, although on some systems, notably UNIX, these are 
identical. A text stream is a sequence of lines; each line has zero 
or more characters and is terminated by `'\n'`. An environment may 
need to convert a text stream to or from some other representation 
(such as mapping `'\n'` to carriage return and linefeed). 
A binary stream is a 
sequence of unprocessed bytes that record internal data, with the 
property that if it is written, then read back on the same system, 
it will compare equal.

A stream is connected to a file or device by *opening* it; the 
connection is broken by *closing* the stream. Opening a file returns 
a pointer to an object of type `FILE`, which records whatever 
information is necessary to control the stream. 

When a program begins execution, the three streams `stdin`, `stdout`, 
and `stderr` are already open.
