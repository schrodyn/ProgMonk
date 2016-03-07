#Loop Exercise

Welcome back! Now in this exercise we will look at how the `loop`
instruction works.

Let me directly go to the code and the file we are going to be using
is called `Loop.nasm`. And I made just a little small change by
using `loop` instruction, however, a copule of other interesting
things. 

Now, the `loop` instruction works in the following way. It basically
is depended on `ecx` register which is your **counter** register. So
you can load up value into `ecx` and then you can call the `loop`
instruction with the label. Till the time `ecx` is not zero the 
`loop` instruction will continue to call label and keep looping.

```
Begin:
        mov ecx, 0x5

PrintHW:

        push ecx

        ; Print hello world using write syscall
        mov eax, 0x4
        mov ebx, 1
        mov ecx, message
        mov edx, mlen
        int 0x80

        pop ecx
        loop PrintHW
```

note that the `loop` instruction automatically decrements `ecx` as 
well. And that's the reason why we do not require `dec ecx` like we
did in the previous case where we used `dec eax`. So basically the
only changes I am moving the value 0x5 into `ecx` register, I push it
because I use the `ecx` register for syscall, then I pop it back
from the stack and then I call `loop` which decrements the `ecx` 
register, checks if the value is non zero, if it is not zero, calls
the instruction at label `PrintHW` which is back again `push ecx`.
If it is zero It basically moves onto the next instruction. 

The variants of `loop` it is `loope`, `loopne` both of this 
additionally use the zero flag which again you have to look at. And see
how this variations work. 

