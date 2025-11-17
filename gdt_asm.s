global load_gdt             ; make the label load_gdt visible outside this file

; load_gdt - Loads the GDT into the processor
; stack: [esp + 4] address of the GDT structure
;        [esp    ] return address
load_gdt:
    mov eax, [esp + 4]      ; get the address of the GDT structure
    lgdt [eax]              ; load the GDT

    ; Load data segment selectors
    mov ax, 0x10            ; 0x10 is the offset of the data segment selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Load code segment selector with far jump
    jmp 0x08:.flush_cs      ; 0x08 is the offset of the code segment selector

.flush_cs:
    ret                     ; return to the calling function