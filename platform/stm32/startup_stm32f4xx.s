.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global g_pfnVectors

.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
g_pfnVectors:
    .word _estack
    .word Reset_Handler

.section .text, "ax", %progbits
.type Reset_Handler, %function
Reset_Handler:
    /* Copier .data de Flash vers RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_etext
    movs r3, #0
LoopCopy:
    cmp r0, r1
    bge EndCopy
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4
    adds r0, r0, #4
    b LoopCopy
EndCopy:

    /* Remplir .bss avec des zéros */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
LoopZero:
    cmp r0, r1
    bge EndZero
    str r2, [r0]
    adds r0, r0, #4
    b LoopZero
EndZero:

    /* Appeler main() */
    bl main
    b .
