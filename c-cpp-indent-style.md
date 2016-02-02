As wikipedia says, in computer programming, an **indent style** is a 
convention governing the indentation of blocks of code to convey the 
program's structure. 

I've decided to use K&R style for function definition and inner blocks. 
Each function has its opening brace at the next line on the same 
indentation level as its header, the statements within the braces are 
indented, and the closing brace at the end is on the same indentation 
level as the header of the function at a line of its own. 

The blocks inside a function, however, have their opening braces at the 
same line as their respective control statements; closing braces remain 
in a line of their own, unless followed by an else or while keyword.

```c
int main(int argc, char *argv[])
{
    ...
    while (x == y) {
        something();
        somethingelse();

        if (some_error)
            do_correct();
        else
            continue_as_usual();
    }

    finalthing();
    ...
}
```

However, for `if` statement I prefer Bjarne Stroustrup's style. `else` 
statement has its own line on the same identation level as its `if` 
statement.

```c
if (x < 0) {
    puts("Negative");
    negative(x);
}
else {
    puts("Non-negative");
    nonnegative(x);
}
```

Instead of 

```c
if (x < 0) {
    puts("Negative");
    negative(x);
} else {
    puts("Non-negative");
    nonnegative(x);
}
```

It's more readable in my opinion.

Stroustrup extends K&R style for classes, writing them as follows:

```cpp
class Vector { 
public:
    Vector(int s) :elem(new double[s]), sz(s) { }   // construct a Vector
    double& operator[](int i) { return elem[i]; }   // element access: subscripting
    int size() { return sz; } 
private:
    double elem[lowast];    // pointer to the elements
    int sz;                 // the number of elements
};
```

Note that Stroustrup does not indent the labels `public:` and `private:`.

Also note that in Stroustrup style, even though the opening brace of 
a *function* starts on a new line, the opening brace of a *class* is on 
the same line as the class name.


