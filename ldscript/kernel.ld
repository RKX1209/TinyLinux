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

    . = ALIGN(4096);

    .data.idt :
    {
        _data_idt_start = .;
        *(.data.idt)
        _data_idt_end = .;
    }    

    . = ALIGN(8192);	/* init_task (THREAD_SIZE = 8192)*/

    .data.init_task :
    {
	_data_init_task_start = .;
	*(.data.init_task)
	_data_init_task_end = .;
    }

    _font_data = .;
    . += 0x3000; /* font data area */
    . = ALIGN(4096);

    _initramfs_start = .;
    . += 0x3000; /* font data area */
    . = ALIGN(4096);
    _initramfs_end = .;
    
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
