/* Linker script pour STM32F4xx */
MEMORY
{
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
}

/* Sections */
SECTIONS
{
    /* Code et constantes en Flash */
    .text :
    {
        *(.isr_vector)
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Données initialisées en RAM (copiées depuis Flash) */
    .data : AT(_etext)
    {
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    /* Données non initialisées en RAM (BSS) */
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Pointeur de pile (fin de la RAM) */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}