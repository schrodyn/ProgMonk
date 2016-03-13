#Using Metasploit's Encoders

Hello, students! In this video we will look at how to use encoders
with Metasploit. 

Now, metasploit is a versatile tool with which you can do vulnerability
assesment, it can help you exploit research and specifically for our
purpose it can help us create arbitrary payloads as well as encode
payloads with different encoders.

To look at existing payloads in Metasploit, of course, you need to
install metasploit. Please download the latest version of linux
installer from their website and run installer.

Now, if you want to look at all different payloads offered by 
metasploit we can use the `msfpayload` utility which shifts with it.

```
sudo msfpayload -l | grep "linux/x86"
```

Now, there are 3 utilities you have `msfpayload`, `msfencode` and
`msfvenom`. You can see a lot of payloads and you create shellcode
and use it in your programs. Now, I will look at disassembling
`msfpayload` outputs in a later video where we are going to look at 
how to disassemble, understand and analyze shellcode which you might
receive.

In this video what I want to focus on is if you create your own 
shellcode how you can use the different encoders in metasploit to 
encode it **without having to write** your own encoders for this job.

So, what I've done is I have `execve-stack.nasm` copied into 
`Metasploit` directory. Let's compile it and get shellcode.

**This lecture is not completed since It seemed me too boring**



