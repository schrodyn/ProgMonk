#Analyzing Shellcode with Libemu

Hello students! In this video let's take shellcode analysis further.
So using `ndisasm` as well as `gdb` was actually good for shellcode
which is easily to analyze, small in size. But what about shellcode
which is probably much much larger and complicated? For example,
Shell Bind TCP and Shell Reverse TCP. You know, how you analyze such 
one?

Now, this is where we look at a tool called **Libemu** which is 
specifically created for shellcode emulation.

Now, in Shell Bind TCP or Reverse TCP binding a shell is the most 
common way to get a command line access to a compromised system.
Once the attacker has command line access he cun run other commands
and do whatever he likes on the victim system.

There are two variants of such binding a shell to a port. The first is
when you are listening on victim's port itself. This is a pure **Bind 
Shell TCP**. Other is connect back Reverse Shell which is from the 
victim machine connects back to the Attacker machine. And this is 
called **Reverse Shell**.

Now, let's go ahead and analyze both of these using `libemu`. You
can download `libemu` from the link given in the slides but better
to [clone github repo](https://github.com/buffer/libemu).

Inside `libemu` directory once you compiled it go to the `tools`
directory and then to `sctest`. And what we are going to do is use
`sctest` to go ahead and figure out what is happening.

**Note**

Unfortunately, I couldn't install `libemu`. So I couldn't go further :)
But if you try and get it, please, say me how you did this.



