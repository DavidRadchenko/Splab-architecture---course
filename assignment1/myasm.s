section .data                    	; data section, read-write
        an:    DD 0              	; this is a temporary var

        c:     DB 97
section .text                    	; our code is always in the .text section
        global do_Str          	; makes the function appear in global scope
        extern printf            	; tell linker that printf is defined elsewhere 							; (not used in the program)

do_Str:                        	; functions are defined as labels
        push    ebp              	; save Base Pointer (bp) original value
        mov     ebp, esp         	; use base pointer to access stack contents
        pushad                   	; push all variables onto stack
        mov ecx, dword [ebp+8]	; get function argument

;;;;;;;;;;;;;;;; FUNCTION EFFECTIVE CODE STARTS HERE ;;;;;;;;;;;;;;;; 

	mov	dword [an], 0		; initialize answer
	label_here:

   		; Your code goes somewhere around here...


		inc dword [an]
   		CMP byte [ecx], 'a'
   		JGE check_z
   		JMP Less

 	 continue:
		inc ecx      	    ; increment pointer
		cmp byte [ecx], 0   ; check if byte pointed to is zero
		jnz label_here      ; keep looping until it is null terminated

	


   
;;;;;;;;;;;;;;;; FUNCTION EFFECTIVE CODE ENDS HERE ;;;;;;;;;;;;;;;; 

         popad                    ; restore all previously used registers
         mov     eax,[an]         ; return an (returned values are in eax)
         mov     esp, ebp
         pop     ebp
         ret 

convert_Parentheses:
		


;;;;;;;;;;;;;;;; FUNCTION EFFECTIVE CODE ENDS HERE ;;;;;;;;;;;;;;;; 
check_sign:
        
	check_z:
   		CMP byte [ecx], 'z'
   		JG continue

	convert_capital:
		SUB byte[ecx], 32
		dec dword[an]
		JMP continue


	Less:
		CMP byte[ecx], ')'
		JE convert_right
		CMP byte[ecx], '('
		JE convert_left
		CMP byte [ecx], 'A'
   		JGE check_capitalZ
   		JMP continue

   	check_capitalZ:
   		CMP byte [ecx], 'Z'
   		JLE decrement
   		JMP continue

   	
   	decrement:
   		dec dword[an]
   		JMP continue

	convert_left:
		mov byte [ecx], '<'
		JMP continue

	convert_right:
		mov byte [ecx], '>'
		JMP continue



