A file is essentially a serial sequence of bytes

A file has a beginning and an end, and it has a current position, typically 
defined as so many bytes from the beginning

The current position is where any file action (a read
from the file or a write to the file) will take place.

A new current position can be specified as an offset from the beginning of the 
file or

as a positive or negative offset from the previous current position

File Streams
The C library provides functions for reading and writing to or from data streams.
A stream is an abstract representation of any external source or destination for 
data, so the keyboard, the command line on your
display, and files on disk are all examples of streams.

There are two ways of writing data to a stream that is a disk file.
Firstly, you can write a file as a text file. Secondly you can write a file as 
a binary file.

In text file binary data such as values of type int or type double have to be
 converted to characters to allow them to be written to a text file

Data that is written to a binary file is always written as a series of bytes, 
exactly as it appears in memory.

When you process a file in C, your program references a file through a 
file pointer.

A file pointer is an abstract pointer that is associated with a particular file 
when the program is run so that the program can work with different files on 
different occasions.

Opening a File
You associate a specific external file name with an internal file pointer variable
 through a process referred to as opening a file.

You open a file by calling the standard library function fopen(), which
returns the file pointer for a specific external file.

The function fopen() is defined in <stdio.h>, and it has this prototype:
FILE *fopen(char *name, char *mode);

The first argument is the name of the external file
The second argument is a character string called the "file mode" that specifies
what you want to do with the file.

Mode	Description
---------------------------
"w"	Open a text file for write operations. If the file exists, its current 
	contents are discarded.
"a"	Open a text file for append operations. All writes are to the end of the 
	file.
"r"	Open a text file for read operations.

These three modes only apply to text files that are files that are written as 
characters.

If the call to fopen() is successful, the function returns a pointer of type 
File *
If the file cannot be opened for some reason, fopen() returns a null pointer.

There’s a limit to the number of files 
you can have open at one time that will be determined by the value of the 
constant FOPEN_MAX that’s defined in <stdio.h>.

FOPEN_MAX is an integer that specifies the maximum number of streams that
can be open at one time.

The C language standard requires that the value of FOPEN_MAX be at least 8,
including stdin, stdout and stderr. Thus, as a minimum, you will be able to be 
working with up to 5 files simultaneously.

FILE *pfile = fopen("myfile.txt", "w");	/* Open file myfile.txt to write it */

If a file with the name myfile.txt does not already exist, the call to the 
function fopen() in the previous statement will create a new file with this name.

to open the file to add data to the end, use the following statement:
pfile = fopen("myfile.txt", "a");	/* Open file myfile.txt to add to it */

If you want to read a file:
pfile = fopen("myfile.txt", "r");

The file position will be set to the beginning of the data in the file.

If you inadvertently try to open a file for reading that doesn’t exist, 
fopen() will return NULL.

-----------------------
Renaming a File

Renaming a file is very easy. You just use the rename() function, which has the
following prototype:
int rename(const char *oldname, const char *newname);

The integer that’s returned will be 0 if the name change is successful, and 
nonzero otherwise.
The file must be closed when you call rename(), otherwise the operation will fail.

if(rename( "myfile.txt", "myfile_copy.txt"))
  printf("Failed to rename file.");
else
  printf("File renamed successfully.");

Obviously, if the file path is incorrect or the file doesn’t exist, the renaming 
operation will fail.

----------------------
Closing a File

You do this by calling the fclose() function
which accepts a file pointer as an argument and returns a value of type int, 
which will be EOF if an error occurs and 0 otherwise.

fclose(pfile);	/* Close the file associated with pfile */

Note: EOF is a special character called the end-of-file character.
In fact, the symbol EOF is defined in <stdio.h> and is usually equivalent to the 
value –1. However, this isn’t necessarily always the case, so you should
use EOF in your programs rather than an explicit value. EOF generally indicates 
that no more data is available from a stream.

It’s good programming practice to close a file as soon as you’ve finished with it.

You must also close a file before attempting to rename it or remove it.

There is a function in <stdio.h> that will force any unwritten data left in a 
buffer to be written to a file. This is the function fflush(), which you’ve 
already used in previous chapters to flush the input buffer.

With your file pointer pfile, you could force any data left in the output buffer 
to be written to the file by using this statement:

fflush(pfile);

The fflush() function returns a value of type int, which is normally 0 but will 
be set to EOF if an error occurs.


-----------------
Deleting a File

The remove() function that’s declared in <stdio.h> does this. You
use it like this:

remove("pfile.txt");

This will delete the file from the current directory that has the name pfile.txt.
Note that the file should not be open when you call remove() to delete it.

------------------
Writing to a Text File

The simplest write operation is provided by the function fputc(), which writes a 
single character to a text file. It has the following prototype:

int fputc(int c, FILE *pfile);

If the write is successful, it returns the character that
was written. Otherwise it returns EOF.

In practice, characters aren’t written to the physical file one by one. This would
 be extremely inefficient. Hidden from your program and managed by the output 
routine, output characters are written to an area of memory called a "buffer" 
until a reasonable number have been accumulated; they are then all written to the
file in one go.

-----------------------
Reading from a Text File

The fgetc() function is complementary to fputc() and reads a character from a 
text file that has been opened for reading.
It takes a file pointer as its only argument and returns the character read as
type int if the read is successful; otherwise, it returns EOF.

mchar = fgetc(pfile);	/* Reads a character into mchar */

Behind the scenes, the actual mechanism for reading a file is the inverse of 
writing to a file. A whole block of characters is read into a buffer in one go.
The characters are then handed over to your program one at a time as you request 
them, until the buffer is empty, whereupon another block is read.

good practice to open files:

if(!(pfile = fopen(filename, "w")))
{
  printf("Error opening %s for writing. Program terminated.", filename);
  exit (1);
}

The block of statements will be executed if fopen() returns NULL, so in this case 
you display a message and call the exit() function that is declared in <stdlib.h>
for an abnormal end to the program.

good practice to read from file by character:

while((mychar = fgetc(pfile)) != EOF)
  putchar(mychar);	/* Output character from the file */


Writing Strings to a Text File

you have the fputs() function for writing a string to a text file. Its prototype:

int fputs(char *pstr, FILE *pfile);

The first argument is a pointer to the character string that’s to be written to 
the file, and the second argument is a file pointer.

it continues to write characters from a string until it reaches a '\0' character,
which it doesn’t write to the file.

fputs("The higher the fewer", pfile);

----------------------
Reading Strings from a Text File

Complementing fputs() is the function fgets() for reading a string from a text 
file. It has the following prototype:

char *fgets(char *pstr, int nchars, FILE *pfile);

Characters are read from the file until either a '\n' is read or nchars-1 
characters have been read from the file, whichever occurs first.

If a newline character is read, it’s retained in the string. A '\0' character 
will be appended to the end of the string in any event.

---------------------------
array of pointers:

char *proverbs[] =
{ "Many a mickle makes a muckle.\n",
  "Too many cooks spoil the broth.\n",
  "He who laughs last didn't get the joke in"
   " the first place.\n"
};

The number of proverbs in the array is calculated by the following expression:

int count = sizeof proverbs/sizeof proverbs[0];

After prompting for input, you read more proverbs from the keyboard and write 
them to the file with the following statements:

while(true)
{
  fgets(more, LENGTH, stdin);
  if(more[0] == '\n')
    break;
  fputs(more, pfile);
}

good practice to read strings from file:

while(fgets(more, LENGTH, pfile))
  printf("%s", more);

---------------------------------
Formatted Output to a File

fprintf(pfile, "%12d%12d%14f", num1, num2, fnum1);

As you can see, the function name has an additional f (for file), compared with 
printf(), and the first argument is a file pointer that specifies the destination
of the data to be written.

Formatted Input from a File

fscanf(pfile, "%12d%12d%14f", &num1, &num2, &fnum1);


rewind(pfile);	/* Go to the beginning of the file */

This statement calls the rewind() function, which simply moves the current 
position back to the beginning of the file so that you can read it again.

You could have achieved the same thing by closing the file then reopening it
again, but with rewind() you do it with one function call and the operation will 
be a lot faster.



