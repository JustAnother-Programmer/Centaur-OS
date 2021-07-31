[BITS 32]

section .asm

global restore_general_purpose_registers
global task_return
global user_registers

task_return:
    mov ebp, esp
    mov ebx, [ebp + 4]
    push dword [ebx + 44]
    push dword [ebx + 40]
    pushf
    pop eax
    or eax, 0x200
    push eax
    push dword [ebx + 32]
    push dword [ebx + 28]
    mov ax, [ebx + 44]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push dword [ebp + 4]
    call restore_general_purpose_registers
    add esp, 4
    iretd

restore_general_purpose_registers:
    push ebp
    mov ebp, esp
    mov ebx, [ebp + 8]
    mov edi, ebx
    mov esi, [ebx + 4]
    mov ebp, [ebx + 8]
    mov edx, [ebx + 16]
    mov ecx, [ebx + 20]
    mov eax, [ebx + 24]
    mov ebx, [ebx + 12]
    pop ebp
    ret

user_registers:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret