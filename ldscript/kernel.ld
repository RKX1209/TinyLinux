SECTIONS
{
    /* . = 0xC0100000; */
       . = 0x00100000;
    .text :
    {
        _text_start = .;
        *(.text)
        _text_end = .;
    }
    
    .rodata :
    {
        _rodata_start = .;
        *(.rodata)
        _rodata_end = .;
    }

    .data :
    {
        _data_start = .;
        *(.data)
        _data_end = .;
    }
    . += 0x3000; /* font data area */
    . = ALIGN(4096);
    .bss :
    {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    }
    .fin :
    {
        _fin_start = .;
        *(.fin)
        _fin_end = .;
    }
}
