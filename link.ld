ENTRY(loader)                /* Entry point: label "loader" */

SECTIONS {
    . = 0x00100000;          /* Load the code starting at 1 MB */

    .text ALIGN(0x1000) :    /* Text (code) section, aligned to 4 KB */
    {
        *(.text)             /* Include all .text sections from all object files */
    }

    .rodata ALIGN(0x1000) :  /* Read-only data section, aligned to 4 KB */
    {
        *(.rodata*)          /* Include all .rodata* sections */
    }

    .data ALIGN(0x1000) :    /* Data section, aligned to 4 KB */
    {
        *(.data)             /* Include all .data sections */
    }

    .bss ALIGN(0x1000) :     /* Uninitialized data section, aligned to 4 KB */
    {
        *(COMMON)            /* Include all COMMON symbols */
        *(.bss)              /* Include all .bss sections */
    }
}
