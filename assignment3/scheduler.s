global scheduler
extern resume, end_co
extern WorldWidth, WorldLength, t, k, state

section .rodata
Ok DB "ok",10,0
newline DB "",10,0

FORMAT: DB "val %d", 10, 0
FORMAT1: DB "ADDR %p", 10, 0

section .data                     ; data section, read-write    
    counter  DD   0
    iterator  DD   2
    stateSize DD 0
    doubleT DD  0


section .text

scheduler:
        push ebp
        mov ebp, esp
        pushad
        mov ecx, dword[ebp + 8]
        mov eax, dword[ebp + 8 + 4]

   
        mov eax, 0
        mov eax, dword[WorldLength]
        mov ecx, 0
        mov ecx, dword[WorldWidth]
        mul ecx
        mov dword[stateSize], eax
        add dword[stateSize], 2

        push eax 

        mov eax , dword[t]
        
        add eax , eax
        inc eax 
        mov dword[doubleT],eax 
        pop eax 


scheduler_loop:
        cmp dword[doubleT], 0
        je finito

        mov eax, dword[stateSize]
        cmp eax, dword[iterator]
        je complete_gen
		
        inc dword[counter]

        mov eax, dword[k]
        cmp eax, dword[counter]
        je resume_print
        jne resume_cell

resume_print:
        mov ebx, 1
        mov dword[counter], 0
        call resume
        jmp scheduler_loop
       

resume_cell:
		mov ebx, dword[iterator]
		inc dword[iterator]
		call resume  

		jmp scheduler_loop


complete_gen:
        dec dword[doubleT]
        mov dword[iterator], 2
        jmp scheduler_loop

finito:
        call end_co             ; stop co-routines