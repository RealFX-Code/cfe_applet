/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *
    *  Mini startup module for CFE apps      Original File: minicrt0.S
    *
    *  ARM port for BCM47081 (Cortex-A7)
    *  Original MIPS version by Mitch Lichtenberg (mpl@broadcom.com)
    *
    ********************************************************************* */

#define STACK_SIZE 8192

        .bss
        .comm   stack_bottom, STACK_SIZE
        .comm   __junk, 4

        .text
        .extern appletmain
        .global __start

__start:
        /*
         * Set up the stack pointer.
         * ARM has no GP register, so skip that.
         * Leave 8 bytes at top for ABI alignment.
         */
        ldr     sp, =(stack_bottom + STACK_SIZE - 8)

        /*
         * Zero BSS, 4 bytes at a time.
         * Preserve r0-r3 throughout — CFE passed
         * handle/ept/reserved/seal in them and
         * appletmain needs all four.
         */
        ldr     r4, =_fbss
        ldr     r5, =_end
        mov     r6, #0

1:      cmp     r4, r5
        strlt   r6, [r4], #4
        blt     1b

        /*
         * Jump to appletmain.
         * r0-r3 are untouched, so handle/ept/reserved/seal
         * are passed straight through from CFE.
         */
        b       appletmain