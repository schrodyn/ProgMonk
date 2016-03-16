#Writing Custom Crypters

Hello all! In this video we will look at **crypters**. So what are 
crypters? Well, typically crypters are programs which encprypt
executables or shellcode and decrypt them at runtime and then run
the decrypted executable or shellcode. 

Now, basically if you look at coding a crypter itself and you want
to use powerful crypto techniques like RC4, AES etc. it would take
a lot of assembly code. And, unfortunately, the byte product is going
to be that the shellcode size would end up be too large to be useful.

So typically the way crypters are written is that we go ahead and
write them in high-level language and that high-level language in
turn encrypt and decrypt the executable and then run it. In our case
because we are talking about shellcoding what I will do is that I
will write RC4 crypter which will first go ahead and give us the
encrypted shellcode. And then we can go ahead and use that encrypted
shellcode in another program which would decrypt it at runtime and
execute it.

As you can imagine because the shellcode is encrypted with a key which
can keep changing you know at almost all invocation. It would also be
impossible for AV or IDS or anything to fingerprint it.

Now, the algorithm we will use is RC4 which is Symmetric Stream Cipher.
The RC4 has two steps: the first is KSA or **Key Scheduling Algorithm**,
and the second is PRNG or **Pseudo Random Number Generation** 
algorithm. Now, most details can be easily found on wikipedia.

Now, how do you go ahead and write an RC4 Shellcode Crypter in C?
So what I am going to do is I am going to write a simple C program
which will take the RC4 key as input and we can embed the shellcode
inside it and then it would output the encrypted shellcode.

After that I take encrypted shellcode, put it inside the decrypter 
routine and then compile it to an executable so that at runtime that
executable can decrypt the RC4 encrypted shellcode and then run it.
