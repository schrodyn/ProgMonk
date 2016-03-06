#Save State Exercise

Ok, so in the exercise I gave you what I wanted you to do was save
the state of the registers, flags and frame pointer before calling
your procedure. And then recover them after the procedure has done
executing.

Now, the refference I would give you here is from Wikipedia which is
**Function Prologue** and **Epilogue**. So visit this page after view
this video. I'll go back to our code and I have other file called
`Procedure2.nasm` in the same directory and I've basically changed
the previous program in some very simple ways to add the exercise
solution.

Now, to preserve the registers and the flags I use

```
pushad
pushfd
```

to push them on the stack and then I am going ahead and call the 
procedure with the `call` instruction and then I pop the flags and the 
registers back from the stack.

```
PrintHelloWorld:

        ; preserve registers and flags
        pushad
        pushfd

        call HelloWorldProc

        ; restore registers and stack

        popfd
        popad

        loop PrintHelloWorld
```

Now, let's come to the frame pointer or `ebp`. So this is really what
is called **function prologue** and **function epilogue**. And the
prologue is nothing but

```
push ebp
mov ebp, esp
```

And when you **leave** or finish the procedure you do exactly the
opposite which is

```
mov esp, ebp
pop ebp
```

This two instructions can also be written using the `leave` instruction
set. And this actually what constitutes your function prologue and
epilogue.

```
HelloWorldProc:

        push ebp
        mov ebp, esp

        ; Print hello world using write syscall


        mov eax, 0x4
        mov ebx, 0x1
        mov ecx, message
        mov edx, mlen
        int 0x80

        ; mov esp, ebp
        ; pop ebp

        leave
        ret   ; signifies end of procedure
```
