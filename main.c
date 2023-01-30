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
*/

static inline void gpio_set_mode(struct gpio* gpio, uint8_t pin, uint8_t mode) {  
	struct gpio* gpio = GPIO(PIN_BANK(pin));
	uint8_t n = PIN_NUMBER(pin);
	gpio->MODER &= ~(3U << (n * 2)); //clear exisiting flags from specefied pin in MODER
	gpio->MODER |= (mode & 3) << (n * 2); //set new mode
}

