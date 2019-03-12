/* Linker script for STM32L476VGT6 (1024K flash, 128K RAM). */

/* Define memory regions. */
MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}


/* Include the common ld script. */
INCLUDE cortex-m-generic.ld
