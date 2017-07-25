global main
extern init_co, start_co, resume
extern scheduler, printer, cell_func
                            
;;/usr/include/asm/unistd_32.h
sys_exit:  equ   1
sys_open:  equ   5
sys_close:  equ   6
O_RDONLY:  equ   0
sys_read: equ   3
sys_write: equ   4
STDOUT: equ   1

stdout:    equ   1
section .bss 

global Dflag, WorldLength, WorldWidth, t, k, state

Dflag: RESB 4
WorldLength: RESB 4
WorldWidth: RESB 4
t: RESB 4
k: RESB 4
state: RESB 10000  
filename: RESB 4 
tmp: RESB 10000 
index: RESD 0
fileDescriptor: RESD 1 


char_WorldLength: RESB 4
char_WorldWidth: RESB 4
char_t: RESB 4
char_k: RESB 4


section .rodata
Ok DB "ok",10,0

len DB "length=",0 ,0
wid DB "width=",0 ,0
gen DB "number of generations=",0 ,0
fre DB "print frequency=",0 ,0
space DB " ",0 ,0
newline DB "",10 ,0


one DB "1 ", 0, 0
zero DB "0 ", 0, 0
two DB "2 ", 0, 0
three DB "3 ", 0, 0
four DB "4 ", 0, 0
five DB "5 ", 0, 0
six DB "6 ", 0, 0
seven DB "7 ", 0, 0
eight DB "8 ", 0, 0
nine DB "9 ", 0, 0


Error DB "error",10,0

FORMAT: DB "%d", 10, 0

FORMAT1: DB "%c", 0, 0

FORMAT2: DB "%s", 10, 0

section .data
	    ten DD 10  
        printlength DD 2 
        stateSize DD 0
        stkCounter  DD   0

main:	
        push ebp
        mov ebp, esp 
        mov eax, dword [ebp + 8]
        mov ebx, dword [ebp + 8 + 4]

        add ebx, 4

        ; check if -d or not

        mov ecx, dword[ebx]
        mov cl, byte[ecx]
        cmp cl, "-"
        je init_debug

Read_File_Name:
        mov ecx, 0
        mov ecx, dword[ebx]
        mov dword[filename], ecx

Read_WorldLength:
        add ebx, 4
        mov ecx, 0
        mov ecx, dword[ebx]
        mov dword[char_WorldLength], ecx

        push ebx
        call atoi
        add esp, 4

        mov dword[WorldLength], eax

Read_WorldWidth:
        add ebx, 4
        mov ecx, 0
        mov ecx, dword[ebx]
        mov dword[char_WorldWidth], ecx

        push ebx
        call atoi
        add esp, 4

        mov dword[WorldWidth], eax

Read_t:
	add ebx, 4
        mov ecx, 0
        mov ecx, dword[ebx]
        mov dword[char_t], ecx

        push ebx
        call atoi
        add esp, 4

        mov dword[t], eax

Read_k:
	add ebx, 4
        mov ecx, 0
        mov ecx, dword[ebx]
        mov dword[char_k], ecx

        push ebx
        call atoi
        add esp, 4

        mov dword[k], eax

init_state:
        ;;open file
        pushad
        mov eax, sys_open
        mov ebx, dword[filename]
        mov ecx, 0
        int 80h
        mov dword[fileDescriptor], eax
        cmp eax, 0
        popad

        cmp eax, -1
        jle error

        ;;read from file
        pushad
        mov eax, sys_read
        mov ebx, dword[fileDescriptor]
        mov ecx, tmp
        mov edx, 10000
        int 80h
        cmp eax, -1
        jle error
        popad

        mov eax, 0
        mov eax, dword[WorldLength]
        mov ecx, 0
        mov ecx, dword[WorldWidth]
        mul ecx
        mov dword[stateSize], eax

        mov ebx, 0
        mov ecx, 0  


loop:     
        cmp dword[stateSize], 0
        je end_loop

        cmp byte[tmp + ecx], 49
        je init1
        cmp byte[tmp + ecx], 32
        je init0
        cmp byte[tmp + ecx], 10
        je loop1
        jmp exit
loop1:
        inc ecx
        jmp loop

init1:   
        mov edx, 0
        mov dl, 1
        mov byte[state + ebx], dl

        inc ebx
        inc ecx
      
        sub dword[stateSize], 1
        jmp loop

init0:   
        mov edx, 0
        mov dl, 0
        mov byte[state + ebx], dl

        inc ebx
        inc ecx
        
        sub dword[stateSize], 1
        jmp loop


end_loop:
        ;;close filed
        pushad
        mov eax, sys_close
        mov ebx, dword[fileDescriptor]
        int 80h
        popad

	cmp dword[Dflag], 1
        je debug_mode
        jmp _start       

debug_mode:
        ;;print WorldLength
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, len
        mov edx, 7
        int 80h
        popad   

        mov dword[printlength], 2
        cmp dword[WorldLength], 100
        jge print332
        jmp asd3

print332:
        mov dword[printlength], 3

asd3:      
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, dword[char_WorldLength]
        mov edx, 2
        int 80h
        popad

        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad

        ;;print WorldWidth
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, wid
        mov edx, 6
        int 80h
        popad   


        mov dword[printlength], 2
        cmp dword[WorldWidth], 100
        jge print321
        jmp asd21

print321:
        mov dword[printlength], 3

asd21:      
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, dword[char_WorldWidth]
        mov edx, dword[printlength]
        int 80h
        popad

        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad

        ;;print t
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, gen
        mov edx, 22
        int 80h
        popad   
        mov dword[printlength], 2
        cmp dword[t], 100
        jge print32
        jmp asd2

print32:
        mov dword[printlength], 3

asd2:      
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, dword[char_t]
        mov edx,  dword[printlength]
        int 80h
        popad

        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad

        ;;;;;;;;;;;;;;;print k;;;;;;;;;;;;;;;
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, fre
        mov edx, 17
        int 80h
        popad   

        mov dword[printlength], 2
        cmp dword[k], 100
        jge print31
        jmp asd1

print31:
        mov dword[printlength], 3

asd1:      
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, dword[char_k]
        mov edx, dword[printlength]
        int 80h
        popad

        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, newline
        mov edx, 1
        int 80h
        popad

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

_start:
        push ebp
        mov ebp, esp
        xor ebx, ebx            ; scheduler is co-routine 0
        
        mov edx, scheduler
        call init_co            ; initialize scheduler state

        inc ebx                 ; printer i co-routine 1
        mov edx, printer
        call init_co            ; initialize printer state

        ;;;;;;;;;;;;;;;;init cells co-routines;;;;;;;;;;;;;;;
        mov eax, 0
        mov eax, dword[WorldLength]
        mov ecx, 0
        mov ecx, dword[WorldWidth]
        mul ecx
        mov dword[stateSize], eax

init_cell_cos:
        cmp dword[stateSize], 0
        je end_init

        inc ebx                 ; printer i co-routine 1
        mov edx, cell_func
        call init_co 

        dec dword[stateSize]
        jmp init_cell_cos

end_init:
        xor ebx, ebx            ; starting co-routine = scheduler
        call start_co           ; start co-routines

exit:
        mov eax, sys_exit
        xor ebx, ebx
        int 80h

error:
        pushad
        mov eax, sys_write
        mov ebx, STDOUT
        mov ecx, Error
        mov edx, 7
        int 80h
        popad
        jmp exit

init_debug:
	mov dword[Dflag], 1
        add ebx, 4
	jmp Read_File_Name

atoi:
        push ebp
        mov ebp, esp        
        push ecx
        push edx
        push ebx
        mov ecx, dword [ebp+8]
        mov ecx, dword[ecx]
        xor eax, eax
        xor ebx, ebx

atoi_loop:
        xor edx, edx
        cmp byte[ecx], 0
        jz atoi_end
        mul dword[ten]
        mov bl, byte[ecx]
        sub bl, '0'
        add eax, ebx      
        inc ecx
        jmp atoi_loop

atoi_end:
	pop ebx
        pop edx
        pop ecx
        mov esp, ebp
        pop ebp
        ret


