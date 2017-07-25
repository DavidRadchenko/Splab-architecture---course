;;; This is a simplified co-routines implementation:
;;; CORS contains just stack tops, and we always work
;;; with co-routine indexes.
global init_co, start_co, end_co, resume, cell_func
extern WorldWidth, WorldLength, t, k, state, computeState



maxcors:        equ 100*100+2         ; maximum number of co-routines
stacksz:        equ 16*1024     ; per-co-routine stack size


section .bss

stacks: resb maxcors * stacksz  ; co-routine stacks
cors:   resd maxcors            ; simply an array with co-routine stack tops
curr:   resd 1                  ; current co-routine
origsp: resd 1                  ; original stack top
tmp:    resd 1                  ; temporary value


section .rodata
Ok DB "ok",10,0
FORMAT: DB "val %d", 10, 0
FORMAT1: DB "ADDR %p", 10, 0
one DB "1 ", 0, 0
two DB "2", 0, 0
newline DB "",10 ,0


section .text

        ;; ebx = co-routine index to initialize
        ;; edx = co-routine start
        ;; other registers will be visible to co-routine after "start_co"

init_co:
        push eax                ; save eax (on caller-s stack)
        push edx

        mov edx,0
        mov eax, stacksz
        imul ebx                ; eax = co-routine-s stack offset in stacks
        pop edx
        add eax, stacks + stacksz ; eax = top of (empty) co-routine-s stack
        mov [cors + ebx*4], eax ; store co-routine-s stack top
        pop eax                 ; restore eax (from caller-s stack)

        mov [tmp], esp          ; save caller-s stack top
        mov esp, [cors + ebx*4] ; esp = co-routine-s stack top

        cmp ebx, 0
        jne not_scheduler
        mov eax, 0
        mov eax, dword[t]

        mov [esp + 4], eax

        mov eax, 0
        mov eax, dword[k]
        mov [esp + 8], eax
        
not_scheduler:
		
	cmp ebx, 2
        jl ndone

        mov eax, ebx
ndone:
        push edx                ; save return address to co-routine stack
        pushf                   ; save flags
        pusha                   ; save all registers
        mov [cors + ebx*4], esp ; update co-routine-s stack top

        mov esp, [tmp]          ; restore caller-s stack top
        ret                     ; return to caller

        ;; ebx = co-routine index to start
start_co:
        pusha                   ; save all registers (restored in "end_co")
        mov [origsp], esp       ; save caller-s stack top
        mov [curr], ebx         ; store current co-routine index
        jmp resume.cont         ; perform state-restoring part of "resume"

        ;; can be called or jumped to
end_co:
        mov esp, [origsp]       ; restore stack top of whoever called "start_co"
        popa                    ; restore all registers
        ret                     ; return to caller of "start_co"

        ;; ebx = co-routine index to switch to
resume:                         ; "call resume" pushed return address
        pushf                   ; save flags to source co-routine stack
        pusha                   ; save all registers
        xchg ebx, [curr]        ; ebx = current co-routine index
        mov [cors + ebx*4], esp ; update current co-routines stack top
        mov ebx, [curr]         ; ebx = destination co-routine index
.cont:
        mov esp, [cors + ebx*4] ; get destination co-routine-s stack top
        popa                    ; restore all registers
        popf                    ; restore flags
        ret                     ; jump to saved return address


cell_func:
		
        ;;;;;;;;;save eax value;;;;;;;;
        mov edx , 0 
        mov eax , 0
        mov ecx , 0
        mov ecx , [WorldWidth]
        mov eax ,[curr]
        sub eax , 2
        div ecx
 Bpoint :
        push edx 
        push eax 
        call computeState
        add esp, 4+4 
        ; there is value in eax 
        xor ebx, ebx
        call resume  ; resume scheduler

        ;;;;;;;;;;;update state;;;;;;;
       
        mov ecx , [curr]
        sub ecx , 2
        push ecx 
        push eax 
        call update_state
        add esp , 4+4
        xor ebx, ebx
        call resume  ; resume scheduler

        jmp cell_func

end_func:
        popad                         
        mov esp, ebp
        pop ebp
        ret         


update_state:
        push ebp
        mov ebp, esp  
        pushad
        mov ecx, dword[ebp+8]
        mov eax, dword[ebp+8+4]

        cmp ecx, 1
        jge alive
        jmp dead

alive:  
        cmp byte[state + eax], 9
        je d
        inc byte[state + eax]
        jmp d

dead:
        mov byte[state + eax], 0

d:
        popad                         
        mov esp, ebp
        pop ebp
        ret  
