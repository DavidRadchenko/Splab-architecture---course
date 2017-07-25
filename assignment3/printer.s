global printer
extern resume, WorldWidth, WorldLength, t, k, state

sys_write:      equ   4
stdout:         equ   1

sys_exit:  equ   1
sys_open:  equ   5
sys_close:  equ   6
O_RDONLY:  equ   0
sys_read: equ   3
STDOUT: equ   1

section .data
	stateSize DD 0

section .rodata
	newline DB "",10 ,0
        one DB "1", 0, 0
        zero DB "0", 0, 0
        two DB "2", 0, 0
        three DB "3", 0, 0
        four DB "4", 0, 0
        five DB "5", 0, 0
        six DB "6", 0, 0
        seven DB "7", 0, 0
        eight DB "8", 0, 0
        nine DB "9", 0, 0

section .text

printer:
        ;;;;;;;;;;;;;;;;print board;;;;;;;;;;;;;;;

        mov eax, 0
        mov eax, dword[WorldLength]
        mov ecx, 0
        mov ecx, dword[WorldWidth]
        mul ecx
        mov dword[stateSize], eax

        mov ebx, 0
        mov ecx, 0
        mov ecx, dword[WorldWidth]
        
iterator:
        cmp dword[stateSize], 0
        je end_iterator
        
        cmp byte[state + ebx], 1
        je print1
        cmp byte[state + ebx], 0
        je print0
        cmp byte[state + ebx], 2
        je print2
        cmp byte[state + ebx], 3
        je print3
        cmp byte[state + ebx], 4
        je print4
        cmp byte[state + ebx], 5
        je print5
        cmp byte[state + ebx], 6
        je print6
        cmp byte[state + ebx], 7
        je print7
        cmp byte[state + ebx], 8
        je print8
        cmp byte[state + ebx], 9
        je print9
cont:
        inc ebx
        sub dword[stateSize], 1   
        sub ecx, 1
        cmp ecx, 0
        je printNewLine
        jmp iterator

print0:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, zero
        mov edx, 3
        int 80h
        popad
        jmp cont

print1:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, one
        mov edx, 3
        int 80h
        popad
        jmp cont

print2:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, two
        mov edx, 3
        int 80h
        popad
        jmp cont

print3:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, three
        mov edx, 3
        int 80h
        popad
        jmp cont

print4:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, four
        mov edx, 3
        int 80h
        popad
        jmp cont

print5:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, five
        mov edx, 3
        int 80h
        popad
        jmp cont

print6:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, six
        mov edx, 3
        int 80h
        popad
        jmp cont
print7:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, seven
        mov edx, 3
        int 80h
        popad
        jmp cont
print8:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, eight
        mov edx, 3
        int 80h
        popad
        jmp cont

print9:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, nine
        mov edx, 3
        int 80h
        popad
        jmp cont

printNewLine:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad
        mov ecx, dword[WorldWidth]
        jmp iterator

end_iterator:
		pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad

        
        xor ebx, ebx
        call resume             ; resume scheduler

        jmp printer