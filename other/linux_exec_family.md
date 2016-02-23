#Why are the exec family of functions (execl, execvp, etc) named the way they are?

The *exec family of functions* are used to replace the currently 
executing process image with an image from a different binary. That is,
they *execute* a new program.

Here are the standardized POSIX functions:

```c
int execl (const char *path, const char *arg, ...);
int execlp (const char *file, const char *arg, ...);
int execle (const char *path, const char *arg, ..., char *const envp[]);
int execv (const char *path, char *const argv[]);
int execvp (const char *file, char *const argv[]);
int execve (const char *file, char *const argv[], char *const envp[]);
```

Some systems offer one or two more:

```c
int execlpe (const char *file, const char *arg, ..., char *const envp[]);
int execvpe (const char *file, char *const argv[], char *const envp[]);
```

The name is a mnemonic. The function starts with *exec*, for obvious 
reasons.

Next is either `l` or `v`, which delineates whether the function 
receives command-line arguments via a variable-argument *list* or via 
an array (*vector*).

Then, optionally, is a `p`, delineating whether the function accepts a 
fully-qualified path (e.g. `/usr/bin/ls`) or whether it accepts a 
filename (e.g. `ls`), which it searches for in the user's *path*.

Finally, there is an optional `e`, indicating whether the function 
accepts an array indicating a new set of *environment* variables for 
the new process.

For example, `execvpe()` accepts arguments via *vector*, searches the 
user's *path*, and accepts a new set of *environment* variables.

When in doubt, use `execl()`.
