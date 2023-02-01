//arm-none-eabi-gcc -mcpu=cortex-m4 main.c -c

#include <stdbool.h>
#include <inttypes.h>

struct gpio {
	volatile uint_32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};

#define PIN(bank, num) ((((bank - 'A') << 8 | (num))))
#define PIN_NUMBER(num) (pin & 255)
#define PIN_BANK(pin) (pin >> 8)

enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG}; 

static inline void gpio_set_mode(uint8_t pin, uint8_t mode) {  
	struct gpio* gpio = GPIO(PIN_BANK(pin));
	uint8_t n = PIN_NUMBER(pin);
	gpio->MODER &= ~(3U << (n * 2)); //clear exisiting flags from specefied pin in MODER
	gpio->MODER |= (mode & 3) << (n * 2); //set new mode
}
#define GPIO (bank) ((struct gpio *) (0x4002000 + 0x400 * (bank)))


/*
		Interrupt handling on ARM Process, Nucleo Board speceficially.
		Vector Table: Concept common to all ARM MCUs. It is an array of 32-bit addresses of interrupt handlers
		All ARM MCUs reserve the first 16 entries, and the rest are platform dependent.
		The Nucleo, or more accurately, the STM32, has 91 peripheral handlers, in addition to the standard 16.
*/

int main(void) {
	return 0;		//so far do nothing
}

// Startup code. Defines a function called reset with the naked (no prolog or epilog code) and noreturn (no return to caller) attributes which loops infinitely. 
__attribute__((naked, noreturn)) void _reset(void) {
	// memset .bss to zero, and copy .data section to RAM region
	extern long _sbss, _ebss, _sdata, _edata, _sidata;
	for (long *src = &_sbss; src < &_ebss; src++) *src = 0;
	for (long *src = &_sdata, *dst = &_sidata; src < &_edata;) *src++ = *dst++;
	for (;;) (void) 0;  // Infinite loop

	main();
	for(;;) (void) 0;
}

extern void _estack(void);  // Defined in link.ld. Creates initial stack


/* !Obselete
	16 standard and 91 STM32-specific handlers. Creates an array of void pointers to functions, with each function being an IRQ handler(VECTOR TABLE for IRQ Handling)
	vector table is called "tab" and is located in .vectors. We will later tell the linker to insert this vector table at the beginning of the generated firmware (and beginning of flash)
	FIRST TWO ENTRIES [0] and [1] are the value of the stack pointer register, and the firmwares entry point respectively
	__attribute__((section(".vectors"))) void (*tab[16 + 91])(void) = {
  	_estack, _reset
};
*/

// Below is an object dump of the obselete minimal firmware
// VMA and LMA have addresses set to zero, meaning main.o is incomplete firmware
//.test contains firmware code, ours is currently just a reset function, 2 bytes long. 
// Those two-bytes are a jump instruction to its own address 

/*
			main.o:     file format elf32-littlearm

			Sections:
			Idx Name          Size      VMA       LMA       File off  Algn
			  0 .text         00000002  00000000  00000000  00000034  2**1
			                  CONTENTS, ALLOC, LOAD, READONLY, CODE
			  1 .data         00000000  00000000  00000000  00000036  2**0
			                  CONTENTS, ALLOC, LOAD, DATA
			  2 .bss          00000000  00000000  00000000  00000036  2**0
			                  ALLOC
			  3 .vectors      000001ac  00000000  00000000  00000038  2**2
			                  CONTENTS, ALLOC, LOAD, RELOC, DATA
			  4 .comment      00000034  00000000  00000000  000001e4  2**0
			                  CONTENTS, READONLY
			  5 .ARM.attributes 0000002e  00000000  00000000  00000218  2**0
			                  CONTENTS, READONLY

*/