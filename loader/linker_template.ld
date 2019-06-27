OUTPUT_ARCH(arm)

SECTIONS
{
    . = ${CODEADDR};
    .text : {
        __text_start = . ;
        *(.text)
        *(.text.*)
        __text_end  = . ;
    }

    . = ${DATAADDR};
    .data : {
        __data_start = . ;
        *(.rodata)
        *(.data)
        *(.bss)
        *(COMMON)
        __data_end  = . ;
    }
}
