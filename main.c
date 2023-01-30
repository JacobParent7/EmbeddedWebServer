//arm-none-eabi-gcc -mcpu=cortex-m4 main.c -c

//#include <stdint.h>
/*
struct gpio {
	volatile uint_32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};


#define GPIO (bank) ((struct gpio *) (0x4002000 + 0x400 * (bank)))
#define PIN(bank, num) ((((bank - 'A') << 8 | (num))))
#define PIN_NUMBER(num) (pin & 255)
#define PIN_BANK(pin) (pin >> 8)

enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG}; 

/*
Input (gpio) -> pointer to desired register (in this case it will always be MODER)
Input (pin) -> Which pin of moder is being targeted (0-15)
Input (mode) -> Which mode (specefied in the enum above) will that pin be set to


static inline void gpio_set_mode(uint8_t pin, uint8_t mode) {  
	struct gpio* gpio = GPIO(PIN_BANK(pin));
	uint8_t n = PIN_NUMBER(pin);
	gpio->MODER &= ~(3U << (n * 2)); //clear exisiting flags from specefied pin in MODER
	gpio->MODER |= (mode & 3) << (n * 2); //set new mode
}

*/


/*	Interrupt handling on ARM Process, Nucleo Board speceficially.
		Vector Table: Concept common to all ARM MCUs. It is an array of 32-bit addresses of interrupt handlers
		All ARM MCUs reserve the first 16 entries, and the rest are platform dependent.
		The Nucleo, or more accurately, the STM32, has 91 peripheral handlers, in addition to the standard 16.
*/


// Startup code. Defines a function called reset with the naked (no prolog or epilog code) and noreturn (no return to caller) attributes which loops infinitely. 
__attribute__((naked, noreturn)) void _reset(void) {
  for (;;) (void) 0;  // Infinite loop
}

extern void _estack(void);  // Defined in link.ld. Creates initial stack

// 16 standard and 91 STM32-specific handlers. Creates an array of void function pointers
__attribute__((section(".vectors"))) void (*tab[16 + 91])(void) = {
  _estack, _reset
};