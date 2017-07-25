section .data                     ; data section, read-write    
      cleared    DD   0
      length_flag   DB  0
      ShlFlag   DB  0
      Correct_print   DB  0
      Counter    DD   0
      PointerToNode DD 0
      PointerToLst DD 0
      container1 DB 0
      container2 DB 0
      Temp DD 0
      numLen DD 0
      digit1 DB 0
      digit2 DB 0
      carry db 0
      ThereIsNoZeroes db 0 
      OperationsCounter  DD   0
      ShiftCounter  DD  0
      numOfNodes1 DB 0
      numOfNodes2 DB 0



section .rodata
  StackSize equ 5
  calc DB ">>calc: ",0 ,0
  result DB ">>" ,0
  ERORR1 DB ">>Error: Operand Stack Overflow" ,10,0
  ERORR2 DB ">>Error: Insufficient Number of Arguments on Stack" ,10,0
  ERORR3 DB ">>Error: Illegal Input" ,10,0
  ERORR4 DB ">>Error: exponent too large" ,10,0

  Ok DB "ok",10,0
  as1 DB "arg1",10,0
  as2 DB "arg2",10,0


  FORMAT_WRL:
    DB 10,0
  FORMAT:
    DB "%d", 10,0
  FORMAT1:
    DB "%02x", 0
  FORMAT2:
    DB "%0x", 0

section .bss 
  buf1:   
      RESB 80
  stack:
      RESD StackSize
  res : resd 1
section .text

     align 16
     global main
     extern printf
     extern fprintf
     extern malloc
     extern free
     extern exit
     extern fgets
     extern stderr
     extern stdin
     extern stdout


;******** main **********

main:
        push ebp
        mov ebp, esp  
        pushad
  
;***********my code*************  
  
EnterArg:
        push calc
        call printf
        add esp,4
        
        mov byte[container1], 0
        push dword[stdin]
        push 80
        push buf1
        call fgets
        add esp, 12

        
        mov ebx, buf1
        jmp CheckOp


CheckOp: 
        cmp   byte[ebx],'q'
        je Quit

        cmp  byte[ebx] ,'p'
        je PopAndPrint
            
        cmp byte[ebx],'+'
        je Addition
            
        cmp  byte[ebx],'d'
        je Duplicate
        
        cmp  byte[ebx],'r'
        je ShiftRight
        
        cmp byte[ebx],10
        je EnterArg
        cmp  byte[ebx],'l'
        je ShiftLeft

        jmp Check_stack

Check_stack:
        mov dword[PointerToNode], 0
        mov dword[PointerToLst], 0
        mov dword[numLen],0
        cmp dword[Counter], StackSize
        je Error1
        jmp Remove_leading_zeroes 

Remove_leading_zeroes: 
        cmp byte[ebx], 10   ;newline
        je Combine_zer
        cmp byte[ebx], 48   ;0
        jne SAVEvalue 
        inc dword[Temp]
        inc ebx
        jmp Remove_leading_zeroes

Combine_zer:
        cmp dword[Temp],0
        jg Combine_containters
        jmp EnterArg
SAVEvalue:
        mov dword[cleared], ebx
        jmp Check_number

Check_number: 
        cmp byte[ebx], 10
        je before_Num_to_lst
        cmp byte[ebx], 48
        jge CheckSmallerThanNine
        jmp Error3

before_Num_to_lst: 
		mov eax, 0
		mov al, byte[numOfNodes1]
		mov byte[numOfNodes2], al 
		mov byte[numOfNodes1], 0
        sub ebx, 1
        mov eax, 0
        mov ecx, 0
        mov eax ,dword[numLen]
        mov edx, 0
        mov ecx ,2

        div ecx

        cmp edx, 1
        je changeFlag

        jmp Num_to_lst

changeFlag:
        inc byte[length_flag]
        jmp Num_to_lst

CheckSmallerThanNine:
        cmp byte[ebx], 57
        jle Add_num_length
        jmp Error3   
        cmp dword[Counter], StackSize
        je Error1

Add_num_length:
        add byte[numLen], 1
        inc ebx
        jmp Check_number


Num_to_lst:
        mov byte[container1], 0
        mov byte[container2], 0
        cmp dword[numLen], 0
        je Push
        mov eax, 0
        mov al, byte[ebx]
        sub al, 48
        
        mov byte[container1], al
        
        cmp dword[numLen], 1
        je Switch_containers

        sub ebx, 1
        mov al, byte[ebx]
        sub al, 48
        mov byte[container2], al
        sub ebx , 1
     
        mov ecx, dword[numLen]
        sub ecx, 2
        mov dword[numLen], ecx
        jmp Combine_containters 

Switch_containers:
        mov byte[length_flag], 0
        mov al, byte[container1]
        mov byte[container2], al
        call Create_node
        call Push_to_stk
        jmp EnterArg

Combine_containters:
        shl byte[container2], 4      
        mov al, byte[container1]
        OR  byte[container2], al 
        mov al, byte[container2]
        call Create_node
        jmp Num_to_lst

Push:
        call Push_to_stk
        jmp EnterArg

Create_node:
		inc byte[numOfNodes1]
        push ebp
        mov ebp, esp  
        pushad
        push 5
        call malloc
        add esp, 4
        
        cmp dword[PointerToLst], 0
        je rola
        
        mov edx,dword[PointerToNode]
        mov dword[edx], eax
con:
        mov dword[PointerToNode],eax
        mov cl,byte[container2]
        mov byte[eax], cl
        inc dword[PointerToNode]
        popad
                    
        mov  esp, ebp
        pop  ebp
        ret      
        
rola:
        mov dword[PointerToLst], eax
        jmp con

Push_to_stk:
		
        push ebp
        mov ebp, esp  
        pushad
        mov ebx, 0
        add ebx, dword[Counter]   ;the current pointer into the stack 
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack

        mov eax, 0
        mov eax, dword[PointerToLst]
        mov dword[ebx], eax   

        mov byte[container2], 0
        mov byte[container1], 0
        inc dword[Counter]
        mov  esp, ebp
        pop  ebp
        ret   

PopAndPrint:
		cmp byte[numOfNodes1], 0
		je rty
zxc:
		mov eax, 0
		mov al, byte[numOfNodes2]
		mov byte[numOfNodes1], al

        pushad
        mov ebx, 0
        cmp dword[Counter], 0
        je Error2
        dec dword[Counter]
        add ebx, dword[Counter]   ;the current pointer into the stack 
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack

        mov byte[ThereIsNoZeroes],0
        push result
        call printf
        add esp, 4
        mov eax, dword[ebx] 
        mov byte[Correct_print], 1

        mov ebx, eax
        push ebx
        call recursive
        add esp, 4
        inc dword[OperationsCounter]
        cmp byte[ThereIsNoZeroes], 0
        je biadsy
biadsyKing:       
        push FORMAT_WRL
        call printf
        add esp,4
        popad
        jmp EnterArg

biadsy:     
        push ecx
        push FORMAT2
        call printf
        add esp,8  
        jmp biadsyKing
        
recursive:
        push ebp
        mov ebp, esp  
        pushad
        mov eax,dword   [ebp+8]
        mov ebx,eax
        inc ebx
        cmp dword[ebx],0
        je print
        push dword[ebx] 
        call recursive
        add esp,4

print:
        cmp byte[eax],0x0
        je checkBeforeRet
        mov byte[ThereIsNoZeroes],1
re1:
        mov ecx,0
        mov cl, byte[eax]
        cmp byte[Correct_print], 1
        je Last_label_for_this_assignment
        pushad
        push ecx
        push FORMAT1
        call printf
        add esp,8
        popad
        jmp ret1

Last_label_for_this_assignment:
        cmp cl, 10
        jle print1234
print1234:
        mov byte[Correct_print], 0
        pushad
        push ecx
        push FORMAT2
        call printf
        add esp,8
        popad
        jmp ret1

checkBeforeRet:
         cmp byte[ThereIsNoZeroes],1   ; id its 1 that means there is a non zeroes nodes s
         je re1

ret1:
        popad                         
        mov       esp, ebp
        pop         ebp
        ret      
rty:
		mov byte[numOfNodes2], 0
		jmp zxc
Quit:   
        mov eax, 0
        mov eax, dword[OperationsCounter]
        pushad
        push eax
        push FORMAT
        call printf
        add esp, 8
        popad
        mov eax, 0
        push eax
        call exit
        add esp, 4

Duplicate:
        pushad
        mov ebx, 0
        cmp dword[Counter] , 0
        je Error2
        cmp dword[Counter], StackSize
        je Error1
        dec dword[Counter]
        add ebx, dword[Counter]   ;the current pointer into the stack 
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack
        inc dword[Counter]
        mov dword[PointerToLst],0
        mov dword[PointerToNode],0
        push dword[ebx]
        call DupFunction
        add esp, 4
        popad
        inc dword[OperationsCounter]

        cmp byte[ShlFlag], 0
        je EnterArg
        jmp dav

DupFunction:      
        push ebp
        mov ebp, esp  
        pushad
        mov ebx, dword   [ebp+8]

continue:
        mov ecx, 0
        mov cl, byte[ebx]
        mov byte[container2], cl
        inc ebx
        cmp dword[ebx], 0
        je Create_Last_Node
        call Create_node
        mov eax, 0
        mov eax, dword[ebx]
        mov ebx, eax
        jmp continue

Create_Last_Node:
        call Create_node
        call Push_to_stk

end_func:
        popad                         
        mov esp, ebp
        pop ebp
        ret         
       
ShiftRight: 
        pushad
        mov ebx, 0
        mov eax, 0
        cmp dword[Counter] , 1
        jle Error2

        cmp byte[numOfNodes1], 1
		jg Error4
       
        dec dword[Counter]
        mov ebx, dword[Counter]  
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack
  
        mov eax, 0
	    inc dword[OperationsCounter]
        mov ebx,dword[ebx]
        mov al,byte[ebx]
        dec dword[Counter]
        mov ebx, dword[Counter]   ;the current pointer into the stack 
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack
        inc dword[Counter]

        mov byte[digit1], 0
        mov byte[digit2], 0

NTimes:
        cmp al,0
        je endShiftR
        mov byte[carry],0
        push dword[ebx]
        call ShiftRightOne
        add esp,4
        dec al
        das
        jmp NTimes
        
endShiftR:
        jmp EnterArg
        popad
        mov  esp, ebp
        pop  ebp
        ret
         
ShiftRightOne:
        push ebp
        mov ebp, esp  
        pushad
        mov ebx, dword   [ebp+8]
        mov cl,byte[ebx]
        inc ebx
        cmp dword[ebx],0
        je shift
        push dword[ebx]
        call ShiftRightOne
        add esp,4
        
shift:
        mov dl,cl
        shr dl,4    ; dl=5
        and cl,0x0F ; cl=6
        mov dh, 2
        mov eax,0
        mov al,dl
        mov ah,0
        div dh  
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        mov byte[digit2],al
        mov byte[carry],ah
        mov ah,0
        mov al,cl
        div dh
test1:
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        add al,byte[carry]
        mov byte[digit1],al
        mov byte[carry],ah
test2:
        shl byte[digit2], 4      
        mov al, byte[digit1]
        OR  byte[digit2], al 
        mov al, byte[digit2]
        dec ebx
        mov byte[ebx],al
test3:
        popad
        mov  esp, ebp
        pop  ebp
        ret

ShiftLeft: 
        pushad
        mov ebx, 0
        mov eax, 0
        cmp dword[Counter] , 1
        jle Error2

        cmp byte[numOfNodes1], 1
        jg Error4
        
        dec dword[Counter]
        add ebx, dword[Counter]   
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack

        mov eax, 0
        mov ecx, 0
        mov ecx, dword[ebx]
        mov eax, dword[ecx]
        mov dword[ShiftCounter], eax

        mov eax, dword[ShiftCounter] 
        mov byte[ShlFlag], 1

looper:
        cmp dword[ShiftCounter], 0
        je endLoop
        jmp Duplicate
dav:        
        jmp Addition
dav2:   
        dec dword[ShiftCounter]
        jmp looper

endLoop:
        mov byte[ShlFlag], 0    
        inc dword[OperationsCounter]

        jmp EnterArg


Error4 :
        inc dword[Counter]
        push ERORR4
        call printf
        add esp,4
        jmp EnterArg

Error3 :
        push ERORR3 
        call printf
        add esp,4
        jmp EnterArg

Error2 :
        push ERORR2 
        call printf
        add esp,4
        jmp EnterArg

Error1 :
        push ERORR1 
        call printf
        add esp,4
        jmp EnterArg

OKEY: 
        push Ok
        call printf
        add esp,4
        jmp done

Addition:
        pushad
        mov ebx, 0
        mov eax, 0
        cmp dword[Counter] , 1
        jle Error2

        dec dword[Counter]
        add ebx, dword[Counter]   
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, dword[Counter]
        add ebx, stack

        dec dword[Counter]
        add eax, dword[Counter]   
        add eax, dword[Counter]
        add eax, dword[Counter]
        add eax, dword[Counter]
        add eax, stack

        mov dword[PointerToLst], 0
        mov dword[PointerToNode], 0

        push dword[ebx]
        push dword[eax]
        call addFunction
        add esp, 4
        
        popad
        inc dword[OperationsCounter]

        cmp byte[ShlFlag], 0
        je EnterArg
        jmp dav2

addFunction:
        push ebp
        mov ebp, esp  
        pushad
        mov ecx, dword [ebp+8]
        mov ebx, dword [ebp+8+4]

continue1:
        mov edx, 0
        mov eax, 0
        mov al, byte[ecx]
        mov dl, byte[ebx]
        jmp ComputeSum

loop:
        inc ebx
        inc ecx
        cmp dword[ebx], 0
        je end_of_Num1
        cmp dword[ecx], 0
        je end_of_Num2
        call Create_node
        mov eax, 0
        mov eax, dword[ebx]
        mov ebx, eax
        mov eax, 0
        mov eax, dword[ecx]
        mov ecx, eax
        jmp continue1

ComputeSum:
        add al, dl
        daa 
        jc carry_flag
        add al, byte[container1]
        daa 
        jc carry_flag1
        mov byte[container1], 0
qwe:
        mov byte[container2], al
        jmp loop

carry_flag:
        add al, byte[container1]
        daa 
        jc carry_flag1
        mov byte[container1], 1
        jmp qwe

carry_flag1:
        mov byte[container1], 1
        jmp qwe

end_of_Num1:
        cmp dword[ecx], 0
        je Create_Last_Node1
        call Create_node
        mov eax, 0
        mov eax, dword[ecx]
        mov ecx, eax
        mov al, byte[ecx]
        mov dl, 0
        jmp ComputeSum
        jmp end_of_Num1

end_of_Num2:
        cmp dword[ebx], 0
        je Create_Last_Node1
        call Create_node
        mov eax, 0
        mov eax, dword[ebx]
        mov ebx, eax
        mov al, byte[ebx]
        mov dl, 0

        jmp ComputeSum

        jmp end_of_Num2

Create_Last_Node1:
        call Create_node
        cmp byte[container1], 1
        je Carry_Node
        call Push_to_stk
        jmp done

Carry_Node:
        mov byte[container2], 1
        jmp Create_Last_Node

done:
        popad                         
        mov       esp, ebp
        pop         ebp
        ret      